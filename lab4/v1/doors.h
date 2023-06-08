#pragma once

#define DOORS_OPEN  2000
#define DOORS_CLOSE 3000
#define DOORS_WAIT  5000

#include <string>
#include <boost/asio.hpp>
#include <functional>

#include "writer.h"
#include "timer.h"

// XXX
#define emit
#define PUBLIC_SLOT
#define PRIVATE_SLOT
using Callback = std::function<void()>;

class Doors
{
private:
    enum TimerType { OpenTimer, CloseTimer, WaitTimer };

public:
    enum State { Opened, Opening, Closed, Closing };

    Doors(boost::asio::io_context &ioContext) : Doors(ioContext, Writer()) {}
    Doors(boost::asio::io_context &ioContext, Writer &&writer) : Doors(ioContext, writer) {}
    Doors(boost::asio::io_context &ioContext, Writer &writer);

    PUBLIC_SLOT void open();
    PUBLIC_SLOT void close();

    State get_state() { return m_state; }
    void print_state();

private:
    PRIVATE_SLOT void make_opened();
    PRIVATE_SLOT void make_closed();

    void set_on_closed_callback(Callback callback);
    void set_on_opened_callback(Callback callback);

    void wait();
    void hold();
    void reset_timer(TimerType timer, Callback callback);
    void cancel_timers();
    void update(State state);
    void write(std::string message);

    State m_state = Closed;
    Writer &m_writer;
    boost::asio::io_context &m_context;
    boost::asio::steady_timer m_openTimer;
    boost::asio::steady_timer m_closeTimer;
    boost::asio::steady_timer m_waitTimer;

    Callback doors_closed_signal = [](){};
    Callback doors_opened_signal = [](){};

    friend class Cabin;
    friend class Controller; // XXX
};
