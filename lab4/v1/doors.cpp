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

void Doors::open()
{
    if (m_state == Opening)
    {
        write("Двери и так уже открываются.");
    }
    else if (m_state == Opened)
    {
        write("Двери уже открыты (удержание).");
        hold();
    }
    else if (m_state == Closing || m_state == Closed)
    {
        if (m_state == Closing)
            write("Таймеры сброшены.");

        cancel_timers();
        write("Двери открываются...");
        update(Opening);

        schedule_timer(m_openTimer, DOORS_OPEN, [this]() {
                write("Двери открылись.");
                make_opened();
                });
    }
}

void Doors::hold()
{
    reset_timer(WaitTimer, [this]() {
            write("Время ожидания вышло.");
            close();
            });
}

void Doors::close()
{
    if (m_state == Opening || m_state == Opened)
    {
        if (m_state == Opening)
            write("Таймеры сброшены.");

        cancel_timers();
        write("Двери закрываются...");
        update(Closing);

        schedule_timer(m_closeTimer, DOORS_CLOSE, [this]() {
                write("Двери закрылись.");
                make_closed();
                });
    }
    else if (m_state == Closing)
    {
        write("Двери и так уже закрываются.");
    }
    else if (m_state == Closed)
    {
        write("Двери уже закрыты.");
    }
}

void Doors::make_opened()
{
    update(Opened);
    wait();
}

void Doors::make_closed()
{
    update(Closed);
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
    if (m_state == Opened)
    {
        write("Двери ожидают...");
        schedule_timer(m_waitTimer, DOORS_WAIT, [this]() {
                write("Время ожидания вышло.");
                close();
                });
    }
}

void Doors::reset_timer(TimerType timer, Callback callback)
{
    if (timer == OpenTimer)
    {
        m_openTimer.cancel();
        schedule_timer(m_openTimer, DOORS_OPEN, [callback]() {
                callback();
                });
    }
    else if (timer == CloseTimer)
    {
        m_closeTimer.cancel();
        schedule_timer(m_closeTimer, DOORS_CLOSE, [callback]() {
                callback();
                });
    }
    else if (timer == WaitTimer)
    {
        m_waitTimer.cancel();
        schedule_timer(m_waitTimer, DOORS_WAIT, [callback]() {
                callback();
                });
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

    if (state == Closed)
    {
        emit doors_closed_signal();
    }
    else if (state == Opened)
    {
        emit doors_opened_signal();
    }
}

void Doors::write(std::string message)
{
    message = "Двери : " + message;
    m_writer.write(message);
}
