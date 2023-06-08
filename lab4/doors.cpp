#include "doors.h"

#define DOORS_OPEN  2000
#define DOORS_CLOSE 3000
#define DOORS_WAIT  5000

Doors::Doors(boost::asio::io_context &ioContext, Writer &writer)
    : m_writer(writer),
    m_context(ioContext),
    m_openTimer(ioContext),
    m_closeTimer(ioContext),
    m_waitTimer(ioContext)
{
    s_open_timer_expired.connect([this](){ make_opened(); });
    s_close_timer_expired.connect([this](){ make_closed(); });
    s_wait_timer_expired.connect([this](){ make_closing(); });

    m_openTimer.set_duration(DOORS_OPEN);
    m_openTimer.set_callback([this](){
            s_open_timer_expired.emit();
            });

    m_closeTimer.set_duration(DOORS_CLOSE);
    m_closeTimer.set_callback([this](){
            s_close_timer_expired.emit();
            });

    m_waitTimer.set_duration(DOORS_WAIT);
    m_waitTimer.set_callback([this](){
            s_wait_timer_expired.emit();
            });

    s_opened.connect([this](){
            m_waitTimer.schedule();
            write("Ожидают...");
            });
}

void Doors::make_opening()
{
    if (m_state == Opening)
    {
        write("Уже открываются.");
        return;
    }

    if (m_state == Opened)
    {
        write("Уже открыты.");
        return;
    }

    if (m_state == Closed)
    {
        m_openTimer.schedule();
    }
    else if (m_state == Closing)
    {
        int time_closing = DOORS_CLOSE - m_closeTimer.remaining_time();

        m_closeTimer.cancel();
        write("Все события отвязаны от таймера закрытия.");
        m_openTimer.schedule(time_closing);
    }

    update_state(Opening);
    write("Открываются...");
}

void Doors::make_opened()
{
    if (m_state != Opening)
    {
        write("Ошибка: невозможен переход в состояние <Opened> из этого состояния.");
        return;
    }

    update_state(Opened);
    write("Открыты.");

    s_opened.emit();
}

void Doors::make_closing()
{
    if (m_state == Closing)
    {
        write("Уже закрываются.");
        return;
    }

    if (m_state == Closed)
    {
        write("Уже закрыты.");
        return;
    }

    if (m_state == Opened)
    {
        if (!m_waitTimer.expired())
        {
            m_waitTimer.cancel();
            write("Все события отвязаны от таймера ожидания.");
        }

        m_closeTimer.schedule();
    }
    else if (m_state == Opening)
    {
        int time_opening = DOORS_OPEN - m_openTimer.remaining_time();

        m_openTimer.cancel();
        write("Все события отвязаны от таймера открытия.");
        m_closeTimer.schedule(time_opening);
    }

    update_state(Closing);
    write("Закрываются...");
}

void Doors::make_closed()
{
    if (m_state != Closing)
    {
        write("Ошибка: невозможен переход в состояние <Closed> из этого состояния.");
        return;
    }

    update_state(Closed);
    write("Закрыты.");

    s_closed.emit();
}

void Doors::update_state(State new_state)
{
    m_state = new_state;
}

void Doors::write(std::string message)
{
    message = "Двери : " + message;
    m_writer.write(message);
}

void Doors::print_state()
{
    std::string s;

    switch (m_state)
    {
        case Closed:
            s = "Закрыты.";
            break;
        case Closing:
            s = "Закрываются.";
            break;
        case Opened:
            s = "Открыты.";
            break;
        case Opening:
            s = "Открываются.";
            break;
    }

    write("Состояние: " + s);

    s = std::to_string(m_openTimer.remaining_time());
    write("Таймер открытия: " + s);

    s = std::to_string(m_closeTimer.remaining_time());
    write("Таймер закрытия: " + s);

    s = std::to_string(m_waitTimer.remaining_time());
    write("Таймер ожидания: " + s);
}
