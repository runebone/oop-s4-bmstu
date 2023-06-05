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
    enum State { Idling, Updating, Serving };

    Controller(boost::asio::io_context &ioContext) : Controller(ioContext, Writer()) {}
    Controller(boost::asio::io_context &ioContext, Writer &&writer) : Controller(ioContext, writer) {}
    Controller(boost::asio::io_context &ioContext, Writer &writer);

    void activate_floor_button(int button_number);
    void activate_cabin_button(int button_number);

    void deactivate_floor_button(int button_number);
    void deactivate_cabin_button(int button_number);

    void cancel_cabin_buttons();

    State get_state() { return m_state; }
    void print_state();

private:
    void on_cabin_stopped_callback();

    void inc_current_floor();
    void dec_current_floor();

    void go_for_the_next_target();
    void update_current_target();

    bool is_target_floor_reached();
    bool target_exist();

    void update(State state);
    void write(std::string message);

    State m_state = Idling;

public:
    Cabin m_cabin; // XXX (just to open/close doors directly)

private:
    Writer &m_writer;
    boost::asio::io_context &m_context;

    int m_floor_btns = 0b0000'0000'0000;
    int m_cabin_btns = 0b0000'0000'0000;
    int m_cur_floor  = 1;
    int m_cur_target   = 0;
};
