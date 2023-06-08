#pragma once

#include <string>
#include <boost/asio.hpp>

#include "doors.h"

#include "writer.h"
#include "timer.h"

#include "signals_and_slots.h"

class Cabin
{
public:
    enum State { Idling, Waiting, MovingUp, MovingDown };

    Cabin(boost::asio::io_context &ioContext) : Cabin(ioContext, Writer()) {}
    Cabin(boost::asio::io_context &ioContext, Writer &&writer) : Cabin(ioContext, writer) {}
    Cabin(boost::asio::io_context &ioContext, Writer &writer);

    void print_state();

public SLOTS:
    void make_moving_up();
    void make_moving_down();
    void make_idling();
    void make_waiting();

public SIGNALS:
    Signal<> s_moved_up;
    Signal<> s_moved_down;
    Signal<> s_stopped;
    Signal<> s_waiting;

private:
    void update_state(State new_state);
    void write(std::string message);

    State m_state = Idling;
    Writer &m_writer;

    boost::asio::io_context &m_context;

    AsioTimer m_moveTimer;

    Doors m_doors;
};
