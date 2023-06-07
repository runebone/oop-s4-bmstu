#pragma once

#define CABIN_MOVE_ONE_FLOOR 3000

#include <string>
#include <boost/asio.hpp>

#include "doors.h"
#include "writer.h"
#include "timer.h"

class Cabin
{
public:
    enum State { Idling, Waiting, MovingUp, MovingDown };

    Cabin(boost::asio::io_context &ioContext) : Cabin(ioContext, Writer()) {}
    Cabin(boost::asio::io_context &ioContext, Writer &&writer) : Cabin(ioContext, writer) {}
    Cabin(boost::asio::io_context &ioContext, Writer &writer);

    PUBLIC_SLOT void open_doors();
    PUBLIC_SLOT void close_doors();

    PUBLIC_SLOT void move_up();
    PUBLIC_SLOT void move_down();
    PUBLIC_SLOT void stop(); // ~= make_idling();

    State get_state() { return m_state; }
    void print_state();
    Doors::State get_doors_state() { return m_doors.get_state(); }

private:
    PRIVATE_SLOT void make_idling() { update(Idling); }
    PRIVATE_SLOT void make_waiting() { update(Waiting); }
    PRIVATE_SLOT void make_moving_up();
    PRIVATE_SLOT void make_moving_down();

    void set_on_cabin_moved_up_callback(Callback callback);
    void set_on_cabin_moved_down_callback(Callback callback);
    void set_on_cabin_stopped_callback(Callback callback);

    void update(State state);
    void write(std::string message);

    State m_state = Idling;
    Doors m_doors;
    Writer &m_writer;
    boost::asio::io_context &m_context;
    boost::asio::steady_timer m_moveTimer;

    Callback cabin_moved_up_signal = [](){};
    Callback cabin_moved_down_signal = [](){};
    Callback cabin_stopped_signal = [](){};

    friend class Controller;
};
