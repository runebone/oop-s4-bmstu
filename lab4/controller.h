#pragma once

#include <string>
#include <bitset>
#include <boost/asio.hpp>

#include "cabin.h"

#include "writer.h"
#include "timer.h"

#include "signals_and_slots.h"

#define NUMBER_OF_FLOORS 16

#define IN_CABIN true
#define ON_FLOOR false

class Controller
{
    enum PrevMoveDirection { Down = -1, None = 0, Up = 1 };
public:
    enum State { Idling, Updating, Serving, HandlesCabinMoved, TargetFloorReached, HandlesButtonClicked };

/* private: */
    struct Info
    {
        State state;

        std::bitset<NUMBER_OF_FLOORS> floor_buttons;
        std::bitset<NUMBER_OF_FLOORS> cabin_buttons;
        int current_floor;
        int target_floor;
        int last_direction;
    };

/* public: */
    Controller(boost::asio::io_context &ioContext) : Controller(ioContext, Writer()) {}
    Controller(boost::asio::io_context &ioContext, Writer &&writer) : Controller(ioContext, writer) {}
    Controller(boost::asio::io_context &ioContext, Writer &writer);

    /* void print_state(); */

    Info get_info() const
    {
        Info info;

        info.state = m_state;
        info.floor_buttons = m_floor_btns;
        info.cabin_buttons = m_cabin_btns;
        info.current_floor = m_cur_floor;
        info.target_floor = m_cur_target;
        info.last_direction = m_last_direction;

        return info;
    }

public SLOTS:
    void handle_button_clicked(int button, bool in_cabin);

private SLOTS:
    void make_idling();
    void make_updating();
    void make_serving();
    void handle_cabin_moved();
    void handle_target_floor_reached();

private SIGNALS:
    Signal<> s_should_update;
    Signal<> s_target_updated;
    Signal<> s_target_reached;
    Signal<> s_no_target;
    Signal<> s_cabin_should_move_up;
    Signal<> s_cabin_should_move_down;
    Signal<> s_cabin_should_be_waiting;
    Signal<> s_cabin_should_be_idling;
    Signal<> s_cabin_moved_handled;

private:
    bool activate_floor_button(int button, bool in_cabin);
    bool activate_cabin_cancel_button();
    void deactivate_floor_button(int button, bool in_cabin);

    void increment_current_floor() { ++m_cur_floor; }
    void decrement_current_floor() { --m_cur_floor; }

    bool target_exist() { return m_cur_target != 0; }
    void update_target();

    void update_state(State new_state);
    void write(std::string message);

    State m_state = Idling;

    Writer &m_writer;
    boost::asio::io_context &m_context;

    int m_floor_btns = 0b0000'0000'0000'0000;
    int m_cabin_btns = 0b0000'0000'0000'0000;
    int m_cur_floor  = 1;
    int m_cur_target = 0;

    PrevMoveDirection m_last_direction = None;

public:
    std::shared_ptr<Cabin> m_cabin;
};
