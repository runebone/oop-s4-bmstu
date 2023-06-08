#include "cabin.h"

#define CABIN_MOVE_ONE_FLOOR 3000

Cabin::Cabin(boost::asio::io_context &ioContext, Writer &writer)
    : m_context(ioContext),
    m_doors(ioContext, writer),
    m_writer(writer),
    m_moveTimer(ioContext)
{
    m_moveTimer.set_duration(CABIN_MOVE_ONE_FLOOR);
    m_moveTimer.set_callback([this](){
            if (m_state == MovingUp)
                s_moved_up.emit();
            else if (m_state == MovingDown)
                s_moved_down.emit();
            });

    m_doors.s_closed.connect([this](){ make_idling(); });
    s_waiting.connect([this](){ m_doors.make_opening(); });
}

void Cabin::make_moving_up()
{
    if (m_state == Waiting)
    {
        /* write("Нельзя начать движение с открытыми дверями."); */
        return;
    }

    if (m_state == MovingDown)
    {
        /* write("Нельзя поменять направление движения во время движения (v)."); */
        return;
    }

    if (m_state == Idling)
    {
        write("Начала движение вверх.");
    }

    update_state(MovingUp);
    m_moveTimer.schedule();
}

void Cabin::make_moving_down()
{
    if (m_state == Waiting)
    {
        /* write("Нельзя начать движение с открытыми дверями."); */
        return;
    }
    
    if (m_state == MovingUp)
    {
        /* write("Нельзя поменять направление движения во время движения (^)."); */
        return;
    }

    if (m_state == Idling)
    {
        write("Начала движение вниз.");
    }

    update_state(MovingDown);
    m_moveTimer.schedule();
}

void Cabin::make_idling()
{
    if (!m_moveTimer.expired())
    {
        /* write("Нельзя остановиться между этажами."); */
        return;
    }
    
    if (m_state == Idling)
    {
        /* write("Уже остановлена."); */
        return;
    }

    if (m_state == MovingUp || m_state == MovingDown)
    {
        write("Остановилась.");
    }

    update_state(Idling);
    s_idling.emit();
}

void Cabin::make_waiting()
{
    /* if (m_state == MovingUp || m_state == MovingDown) */
    if (!m_moveTimer.expired())
    {
        /* write("Нельзя открыть двери в движении."); */
        return;
    }

    if (m_state == Waiting)
    {
        /* write("Уже ожидает."); */
        return;
    }

    update_state(Waiting);
    write("Ожидает...");

    s_waiting.emit();
}

void Cabin::update_state(State new_state)
{
    m_state = new_state;
}

void Cabin::write(std::string message)
{
    message = "  Кабина  : " + message;
    m_writer.write(message);
}

void Cabin::print_state()
{
    std::string s;

    switch (m_state)
    {
        case Idling:
            s = "Простаивает.";
            break;
        case Waiting:
            s = "Ожидает.";
            break;
        case MovingUp:
            s = "Едет вверх.";
            break;
        case MovingDown:
            s = "Едет вниз.";
            break;
    }

    write("Состояние: " + s);

    s = std::to_string(m_moveTimer.remaining_time());
    write("Таймер движения: " + s);

    m_doors.print_state();
}
