#pragma once

#define DOORS_OPEN  2000
#define DOORS_CLOSE 3000
#define DOORS_WAIT  5000

#include <string>
#include <boost/asio.hpp>
#include <functional>

#include "writer.h"
#include "timer.h"

#define emit
using Callback = std::function<void()>;

class Doors
{
    enum TimerType
    {
        OpenTimer,
        CloseTimer,
        WaitTimer
    };
public:
    enum State
    {
        Closed,
        Closing,
        Opened,
        Opening
    };

    Doors(boost::asio::io_context &ioContext) : Doors(ioContext, Writer()) {}
    Doors(boost::asio::io_context &ioContext, Writer &&writer) : Doors(ioContext, writer) {}
    Doors(boost::asio::io_context &ioContext, Writer &writer)
        : m_writer(writer),
            m_context(ioContext),
            m_openTimer(ioContext),
            m_closeTimer(ioContext),
            m_waitTimer(ioContext)
    {
    }

    State get_state() { return m_state; }

    void print_state()
    {
        switch (m_state)
        {
            case Closing:
                write("Состояние: Закрываются.");
                break;
            case Closed:
                write("Состояние: Закрыты.");
                break;
            case Opening:
                write("Состояние: Открываются.");
                break;
            case Opened:
                write("Состояние: Открыты.");
                break;
        }
    }

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
                schedule_timer(m_openTimer, DOORS_OPEN, [this]() {
                    write("Двери открылись.");
                    update(Opened);
                    wait();
                });
                break;
            case Opened:
                write("Удержание.");
                reset_timer(WaitTimer, [this]() {
                    // FIXME: DRY
                    write("Время ожидания вышло.");
                    close();
                });
                break;
            default:
                write("Двери и так уже открываются.");
                break;
        }
    }

    void close()
    {
        switch (m_state)
        {
            case Opening:
                write("Таймеры сброшены.");
                // no break
            case Opened:
                cancel_timers();
                write("Двери закрываются...");
                update(Closing);
                schedule_timer(m_closeTimer, DOORS_CLOSE, [this]() {
                    write("Двери закрылись.");
                    update(Closed);
                });
                break;
            case Closing:
                write("Двери и так уже закрываются.");
                break;
            default:
                write("Двери уже закрыты.");
                break;
        }
    }

    void set_on_closed_callback(Callback callback)
    {
        doors_closed_signal = callback;
    }

    void set_on_opened_callback(Callback callback)
    {
        doors_opened_signal = callback;
    }

    void execute_on_closed_callback()
    {
        emit doors_closed_signal();
    }

    void execute_on_opened_callback()
    {
        emit doors_opened_signal();
    }

private:
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

    void reset_timer(TimerType timer, Callback callback)
    {
        switch (timer)
        {
            case OpenTimer:
                m_openTimer.cancel();
                schedule_timer(m_openTimer, DOORS_OPEN, [callback]() {
                    callback();
                });
                break;
            case CloseTimer:
                m_closeTimer.cancel();
                schedule_timer(m_closeTimer, DOORS_CLOSE, [callback]() {
                    callback();
                });
                break;
            case WaitTimer:
                m_waitTimer.cancel();
                schedule_timer(m_waitTimer, DOORS_WAIT, [callback]() {
                    callback();
                });
                break;
        }
    }

    void cancel_timers()
    {
        m_openTimer.cancel();
        m_closeTimer.cancel();
        m_waitTimer.cancel();
    }

    void update(State state)
    {
        m_state = state;

        switch (state)
        {
            case Closed:
                emit doors_closed_signal();
                break;
            case Opened:
                emit doors_opened_signal();
                break;
            default:
                break;
        }
    }

    void write(std::string message)
    {
        message = "Двери : " + message;
        m_writer.write(message);
    }

    State m_state = Closed;
    Writer &m_writer;
    boost::asio::io_context &m_context;
    boost::asio::steady_timer m_openTimer;
    boost::asio::steady_timer m_closeTimer;
    boost::asio::steady_timer m_waitTimer;

    Callback doors_closed_signal = [](){};
    Callback doors_opened_signal = [](){};
};
