#pragma once

#define DOORS_OPEN  2000
#define DOORS_CLOSE 3000
#define DOORS_WAIT  5000

#include <string>
#include <boost/asio.hpp>

#include "writer.h"
#include "timer.h"

class Doors
{
    enum State
    {
        Closed,
        Closing,
        Opened,
        Opening
    };

public:
    Doors(boost::asio::io_context &ioContext, Writer &&writer)
        : m_writer(writer),
            m_context(ioContext),
            m_openTimer(ioContext),
            m_closeTimer(ioContext),
            m_waitTimer(ioContext)
    {
    }

    State get_state() { return m_state; }

    void open()
    {
        switch (m_state)
        {
            case Closing:
                write("Таймеры сброшены.");
                cancel_timers();
                // no break
            case Closed:
                write("Двери открываются...");
                update(Opening);
                schedule_timer(m_closeTimer, DOORS_OPEN, [this]() {
                    write("Двери открылись.");
                    update(Opened);
                    wait();
                });
                break;
            default:
                write("Неверное действие: Двери нельзя открыть в текущем состоянии.");
                break;
        }
    }

    void close()
    {
        switch (m_state)
        {
            case Opening:
                write("Таймеры сброшены.");
                cancel_timers();
                // no break
            case Opened:
                write("Двери закрываются...");
                update(Closing);
                schedule_timer(m_closeTimer, DOORS_CLOSE, [this]() {
                    write("Двери закрылись.");
                    update(Closed);
                });
                break;
            default:
                write("Неверное действие: Двери нельзя закрыть в текущем состоянии.");
                break;
        }
    }

    void wait()
    {
        switch (m_state)
        {
            case Opened:
                write("Двери ожидают...");
                schedule_timer(m_waitTimer, DOORS_WAIT, [this]() {
                    write("Время ожидания вышло.");
                    close();
                });
                break;

            default:
                write("Неверное действие: Двери не могут ожидать в текущем состоянии.");
                break;
        }
    }

private:
    void cancel_timers()
    {
        m_openTimer.cancel();
        m_closeTimer.cancel();
        m_waitTimer.cancel();
    }

    void update(State state)
    {
        m_state = state;
    }

    void write(std::string message)
    {
        m_writer.write(message);
    }

    State m_state = Closed;

    Writer &m_writer;
    boost::asio::io_context &m_context;

    boost::asio::steady_timer m_openTimer;
    boost::asio::steady_timer m_closeTimer;
    boost::asio::steady_timer m_waitTimer;
};
