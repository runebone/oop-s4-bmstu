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

    void open_doors();
    void close_doors();
    void move_up();
    void move_down();
    void stop();

    State get_state() { return m_state; }
    void print_state();
    Doors::State get_doors_state() { return m_doors.get_state(); }

private:
    void set_on_cabin_moved_up_callback(Callback callback);
    void set_on_cabin_moved_down_callback(Callback callback);
    void set_on_cabin_stopped_callback(Callback callback);

    void set_idling() { update(Idling); }

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
