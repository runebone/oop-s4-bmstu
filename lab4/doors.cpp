#include "doors.h"

Doors::Doors(boost::asio::io_context &ioContext, Writer &writer)
    : m_writer(writer),
    m_context(ioContext),
    m_openTimer(ioContext),
    m_closeTimer(ioContext),
    m_waitTimer(ioContext)
{
}

void Doors::print_state()
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

#if 0
void Doors::open()
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
#endif

#if 0
void Doors::close()
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
#endif

void Doors::make_opening()
{
    switch (m_state)
    {
        case Opening:
            write("Двери и так уже открываются.");
            break;
        case Opened:
            write("Двери уже открыты.");
            break;
        case Closing:
            write("Таймеры сброшены.");
            cancel_timers();
            // no break
        case Closed:
            write("Двери открываются...");
            update(Opening);
            schedule_timer(m_openTimer, DOORS_OPEN, [this]() {
                make_opened();
            });
            break;
    }
}

void Doors::make_opened()
{
    switch (m_state)
    {
        case Opening:
            write("Двери открылись.");
            update(Opened);
            wait();
            break;
        case Opened:
        case Closing:
        case Closed:
            break;
    }
}

void Doors::make_closing()
{
    switch (m_state)
    {
        case Opening:
            write("Таймеры сброшены.");
            // no break
            break;
        case Opened:
            cancel_timers();
            write("Двери закрываются...");
            update(Closing);
            schedule_timer(m_closeTimer, DOORS_CLOSE, [this]() {
                make_closed();
            });
            break;
        case Closing:
            write("Двери и так уже закрываются.");
            break;
        case Closed:
            write("Двери уже закрыты.");
            break;
    }
}

void Doors::make_closed()
{
    switch (m_state)
    {
        case Opening:
        case Opened:
            break;
        case Closing:
            write("Двери закрылись.");
            update(Closed);
            break;
        case Closed:
            break;
    }
}

void Doors::set_on_closed_callback(Callback callback)
{
    doors_closed_signal = callback;
}

void Doors::set_on_opened_callback(Callback callback)
{
    doors_opened_signal = callback;
}

void Doors::wait()
{
    switch (m_state)
    {
        case Opened:
            write("Двери ожидают...");
            schedule_timer(m_waitTimer, DOORS_WAIT, [this]() {
                    write("Время ожидания вышло.");
                    make_closing();
                    /* close(); */
                    });
            break;

        default:
            write("Двери не могут ожидать в текущем состоянии.");
            break;
    }
}


void Doors::reset_timer(TimerType timer, Callback callback)
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

void Doors::cancel_timers()
{
    m_openTimer.cancel();
    m_closeTimer.cancel();
    m_waitTimer.cancel();
}

void Doors::update(State state)
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

void Doors::write(std::string message)
{
    message = "Двери : " + message;
    m_writer.write(message);
}
