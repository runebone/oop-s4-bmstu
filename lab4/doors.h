#pragma once

#include <string>
#include <boost/asio.hpp>

#include "writer.h"
#include "timer.h"

#include "signals_and_slots.h"

class Doors
{
public:
    enum State { Opened, Opening, Waiting, Closed, Closing };

/* private: */
    struct Info
    {
        State state;
        int open_time;
        int close_time;
        int wait_time;
    };

/* public: */
    Doors(boost::asio::io_context &ioContext) : Doors(ioContext, Writer()) {}
    Doors(boost::asio::io_context &ioContext, Writer &&writer) : Doors(ioContext, writer) {}
    Doors(boost::asio::io_context &ioContext, Writer &writer);

    /* void print_state(); */

    Info get_info() const
    {
        Info info;

        info.state = m_state;
        info.open_time = m_openTimer.remaining_time();
        info.close_time = m_closeTimer.remaining_time();
        info.wait_time = m_waitTimer.remaining_time();

        return info;
    }

public SLOTS:
    void make_opening();
    void make_closing();

private SLOTS:
    void make_opened();
    void make_closed();
    void make_waiting();

public SIGNALS:
    Signal<> s_closed;
    Signal<> s_opened;

private SIGNALS:
    Signal<> s_open_timer_expired;
    Signal<> s_close_timer_expired;
    Signal<> s_wait_timer_expired;

private:
    void update_state(State new_state);
    void write(std::string message);

    State m_state = Closed;
    Writer &m_writer;

    boost::asio::io_context &m_context;

    AsioTimer m_openTimer;
    AsioTimer m_closeTimer;
    AsioTimer m_waitTimer;

    friend class Cabin;
};
