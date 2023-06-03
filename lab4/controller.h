#pragma once

#include <string>
#include <bitset>
#include <boost/asio.hpp>

#include "cabin.h"
#include "writer.h"
#include "timer.h"

#define NUMBER_OF_FLOORS 12

class Controller
{
public:
    enum State
    {
        Idling,
        Updating,
        Serving
    };

    Controller(boost::asio::io_context &ioContext) : Controller(ioContext, Writer()) {}
    Controller(boost::asio::io_context &ioContext, Writer &&writer) : Controller(ioContext, writer) {}
    Controller(boost::asio::io_context &ioContext, Writer &writer)
        : m_context(ioContext),
            m_cabin(ioContext, writer),
            m_writer(writer)
    {
        m_cabin.set_on_cabin_moved_up_callback([this](){
                inc_current_floor();

                if (!is_goal_floor_reached())
                    m_cabin.move_up();
                else
                    m_cabin.stop();
            });

        m_cabin.set_on_cabin_moved_down_callback([this](){
                dec_current_floor();

                if (!is_goal_floor_reached())
                    m_cabin.move_down();
                else
                    m_cabin.stop();
            });

        m_cabin.set_on_cabin_stopped_callback([this](){
                m_cabin.open_doors();
            });
    }

    void activate_floor_button(int button_number)
    {
        if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
        {
            std::string s;
            s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
            write(s);
        }

        m_floor_btns |= (int)(std::pow(2, button_number - 1));

        update_current_goal();
    }

    void activate_cabin_button(int button_number)
    {
        // XXX: DRY
        if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
        {
            std::string s;
            s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
            write(s);
        }

        m_cabin_btns |= (int)(std::pow(2, button_number - 1));

        update_current_goal();
    }

    void cancel_cabin_buttons()
    {
        m_cabin_btns = 0;
        write("Кнопки кабины сброшены.");

        update_current_goal();
    }

    void inc_current_floor()
    {
        m_cur_floor += 1;
    }

    void dec_current_floor()
    {
        m_cur_floor -= 1;
    }

    void update_current_goal()
    {
        Cabin::State cabin_state = m_cabin.get_state();

        bool is_moving = false;
        bool up = false;

        switch (cabin_state)
        {
            case Cabin::Idling:
                is_moving = false;
                break;
            case Cabin::Waiting:
                is_moving = false;
                break;
            case Cabin::MovingUp:
                is_moving = true;
                up = true;
                break;
            case Cabin::MovingDown:
                is_moving = false;
                up = false;
                break;
        }

        int next_goal = calculate_next_goal_floor(m_cur_floor, m_floor_btns, m_cabin_btns, is_moving, up);

        if (m_cur_goal != next_goal)
        {
            std::string s = "Текущая цель обновлена: " + std::to_string(m_cur_goal) + " -> " + std::to_string(next_goal);
            m_cur_goal = next_goal;
            write(s);
        }
    }

    int calculate_next_goal_floor(int current_floor, int floor_btns_bin, int cabin_btns_bin, bool is_moving, bool up) {
        int next_floor = 0;
        int next_floor_up = 0;
        int next_floor_down = 0;

        for (int floor = current_floor + 1; 1 <= floor && floor <= NUMBER_OF_FLOORS; floor += 1) {
            if (floor_btns_bin & (1 << (floor - 1)) || cabin_btns_bin & (1 << (floor - 1))) {
                next_floor_up = floor;
                break;
            }
        }

        for (int floor = current_floor - 1; 1 <= floor && floor <= NUMBER_OF_FLOORS; floor -= 1) {
            if (floor_btns_bin & (1 << (floor - 1)) || cabin_btns_bin & (1 << (floor - 1))) {
                next_floor_down = floor;
                break;
            }
        }

        // Take next floor on the way up/down if the elevator is moving
        if (is_moving)
        {
            next_floor = up ? next_floor_up : next_floor_down;
        }
        // If elevator is idling/waiting, take the nearest floor
        else
        {
            if (next_floor_up && next_floor_down)
            {
                int cond = (next_floor_up - current_floor) < (current_floor - next_floor_down);

                next_floor = cond ? next_floor_up : next_floor_down;
            }
            else if (next_floor_up)
            {
                next_floor = next_floor_up;
            }
            else if (next_floor_down)
            {
                next_floor = next_floor_down;
            }
            // If no buttons are pressed, go down on the first floor
            else if (current_floor != 1)
            {
                next_floor = 1;
            }
            else
            {
                next_floor = 0;
            }
        }

        return next_floor;
    }

    bool is_goal_floor_reached()
    {
        return m_cur_floor == m_cur_goal;
    }

    State get_state() { return m_state; }

    void print_state()
    {
        switch (m_state)
        {
            case Idling:
                write("Состояние: Простаивает.");
                break;
            case Updating:
                write("Состояние: Обновляет цель.");
                break;
            case Serving:
                write("Состояние: Обслуживает.");
                break;
        }

        std::bitset<NUMBER_OF_FLOORS> floor_btns(m_floor_btns);
        std::bitset<NUMBER_OF_FLOORS> cabin_btns(m_cabin_btns);

        std::string s;

        s = "Кнопки на этажах: " + floor_btns.to_string();
        write(s);

        s = "Кнопки в кабине: " + cabin_btns.to_string();
        write(s);

        s = "Текущий этаж: " + std::to_string(m_cur_floor);
        write(s);

        std::string goal;

        if (m_cur_goal)
            goal = std::to_string(m_cur_goal);
        else
            goal = "Отсутствует.";

        s = "Целевой этаж: " + goal;
        write(s);

        m_cabin.print_state();
    }

private:
    void update(State state)
    {
        m_state = state;
    }

    void write(std::string message)
    {
        message = "Ctrler: " + message;
        m_writer.write(message);
    }

    State m_state = Idling;
public:
    Cabin m_cabin;
private:
    Writer &m_writer;
    boost::asio::io_context &m_context;

    int m_floor_btns = 0b0000'0000'0000;
    int m_cabin_btns = 0b0000'0000'0000;
    int m_cur_floor  = 1;
    int m_cur_goal   = 0;
    /* int m_people_in_cabin = 0; */
};
