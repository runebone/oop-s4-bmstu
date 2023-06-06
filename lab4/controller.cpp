#include "controller.h"

Controller::Controller(boost::asio::io_context &ioContext, Writer &writer)
    : m_context(ioContext),
    /* m_cabin(ioContext, writer), */
    m_writer(writer)
{
    m_cabin = std::make_shared<Cabin>(ioContext, writer);

    m_cabin->set_on_cabin_moved_up_callback([this](){
            inc_current_floor();

            if (!is_target_floor_reached() && target_exist())
            m_cabin->move_up();
            else if (target_exist())
            m_cabin->stop();
            else
            {
            update(Idling);
            m_cabin->set_idling();
            }
            });

    m_cabin->set_on_cabin_moved_down_callback([this](){
            dec_current_floor();

            if (!is_target_floor_reached() && target_exist())
            m_cabin->move_down();
            else if (target_exist())
            m_cabin->stop();
            else
            {
            update(Idling);
            m_cabin->set_idling();
            }
            });

    // Goal is reached
    m_cabin->set_on_cabin_stopped_callback([this](){
            on_cabin_stopped_callback();
            });

    m_cabin->m_doors.set_on_closed_callback([this](){
            m_cabin->set_idling();
            go_for_the_next_target();
            });
}

void Controller::on_cabin_stopped_callback()
{
    deactivate_floor_button(m_cur_floor);
    deactivate_cabin_button(m_cur_floor);

    m_cabin->open_doors();
}

void Controller::activate_floor_button(int button_number)
{
    std::string s;

    if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return;
    }

    m_floor_btns |= (int)(std::pow(2, button_number - 1));

    s = "Кнопка " + std::to_string(button_number) + " этажа активирована (на этаже)";
    write(s);

    update_current_target();
}

void Controller::activate_cabin_button(int button_number)
{
    std::string s;
    // XXX: DRY
    if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return;
    }

    m_cabin_btns |= (int)(std::pow(2, button_number - 1));

    s = "Кнопка " + std::to_string(button_number) + " этажа активирована (в кабине)";
    write(s);

    update_current_target();
}

// XXX
void Controller::deactivate_floor_button(int button_number)
{
    std::string s;

    if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return;
    }

    int bin_button = (int)(std::pow(2, button_number - 1));

    // If button is active, deactivate it
    if (m_floor_btns & bin_button)
    {
        m_floor_btns ^= bin_button;

        s = "Кнопка " + std::to_string(button_number) + " этажа деактивирована (на этаже)";
        write(s);

        update_current_target();
    }
}

// XXX
void Controller::deactivate_cabin_button(int button_number)
{
    std::string s;

    if (button_number < 1 || button_number > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1-" + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return;
    }

    int bin_button = (int)(std::pow(2, button_number - 1));

    // If button is active, deactivate it
    if (m_cabin_btns & bin_button)
    {
        m_cabin_btns ^= bin_button;

        s = "Кнопка " + std::to_string(button_number) + " этажа деактивирована (в кабине)";
        write(s);

        update_current_target();
    }
}

void Controller::cancel_cabin_buttons()
{
    m_cabin_btns = 0;
    write("Кнопки кабины сброшены.");

    update_current_target();
}

void Controller::print_state()
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

    s = "Кнопки в кабине : " + cabin_btns.to_string();
    write(s);

    s = "Текущий этаж: " + std::to_string(m_cur_floor);
    write(s);

    std::string target;

    if (m_cur_target)
        target = std::to_string(m_cur_target);
    else
        target = "Отсутствует.";

    s = "Целевой этаж: " + target;
    write(s);

    m_cabin->print_state();
}

void Controller::inc_current_floor()
{
    m_cur_floor += 1;
}

void Controller::dec_current_floor()
{
    m_cur_floor -= 1;
}

void Controller::go_for_the_next_target()
{
    if (m_cur_target)
    {
        update(Serving);
        if (m_cur_target > m_cur_floor)
            m_cabin->move_up();
        else if (m_cur_target < m_cur_floor)
            m_cabin->move_down();
        else
        {
            on_cabin_stopped_callback();
        }
    }
    else
    {
        update(Idling);
    }
}

static int calculate_next_target_floor(int current_floor, int floor_btns_bin, int cabin_btns_bin, bool is_moving, bool up)
{
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
        /* else if (current_floor != 1) */
        /* { */
        /*     next_floor = 1; */
        /* } */
        else
        {
            if (floor_btns_bin & (1 << (current_floor - 1)) || cabin_btns_bin & (1 << (current_floor - 1)))
            {
                next_floor = current_floor;
            }
            else
            {
                next_floor = 0;
            }
        }
    }

    return next_floor;
}

void Controller::update_current_target()
{
    auto prev_state = m_state;
    update(Updating);

    Cabin::State cabin_state = m_cabin->get_state();

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

    int next_target = calculate_next_target_floor(m_cur_floor, m_floor_btns, m_cabin_btns, is_moving, up);

    if (m_cur_target != next_target)
    {
        std::string s = "Текущая цель обновлена: " + std::to_string(m_cur_target) + " -> " + std::to_string(next_target);
        m_cur_target = next_target;
        write(s);
    }

    update(prev_state);

    if (prev_state == Idling)
    {
        go_for_the_next_target();
    }
}

bool Controller::is_target_floor_reached()
{
    return m_cur_floor == m_cur_target;
}

bool Controller::target_exist()
{
    return m_cur_target != 0;
}

void Controller::update(State state)
{
    m_state = state;
}

void Controller::write(std::string message)
{
    message = "Ctrler: " + message;
    m_writer.write(message);
}
