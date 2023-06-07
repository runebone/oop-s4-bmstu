#include "cabin.h"

Cabin::Cabin(boost::asio::io_context &ioContext, Writer &writer)
    : m_context(ioContext),
    m_doors(ioContext, writer),
    m_writer(writer),
    m_moveTimer(ioContext)
{
    m_doors.set_on_opened_callback([this](){
            make_waiting();
            });

    m_doors.set_on_closed_callback([this](){
            make_idling();
            });
}

void Cabin::open_doors()
{
    if (m_state == Idling || m_state == Waiting)
    {
        m_doors.open();
    }
    else
    {
        write("Кабина не может открыть двери во время движения.");
    }
}

void Cabin::close_doors()
{
    if (m_state == Idling || m_state == Waiting)
    {
        m_doors.close();
    }
    else
    {
        write("Кабина движется, двери уже закрыты.");
    }
}

#if 0
void Cabin::move_up()
{
    bool doors_are_closed = m_doors.get_state() == Doors::Closed;

    if (!doors_are_closed)
    {
        write("Кабина не может начать движение вверх с открытыми дверями.");
    }
    else
    {
        switch (m_state)
        {
            case Idling:
                write("Кабина начала движение вверх.");
                update(MovingUp);
                schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
                        write("Кабина поднялась на этаж выше.");
                        emit cabin_moved_up_signal();
                        /* update(Idling); // XXX */
                        });
                break;
            case Waiting:
                write("Кабина ожидает. Пока нельзя начать движение вверх.");
                break;
            case MovingUp:
                schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
                        // FIXME: DRY
                        write("Кабина поднялась на этаж выше.");
                        emit cabin_moved_up_signal();
                        });
                break;
            case MovingDown:
                write("Кабина не может изменить направление своего движения во время движения.");
                break;
        }
    }
}
#endif

#if 0
void Cabin::move_down()
{
    bool doors_are_closed = m_doors.get_state() == Doors::Closed;

    if (!doors_are_closed)
    {
        write("Кабина не может начать движение вниз с открытыми дверями.");
    }
    else
    {
        switch (m_state)
        {
            case Idling:
                write("Кабина начала движение вниз.");
                update(MovingDown);
                schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
                        write("Кабина опустилась на этаж ниже.");
                        emit cabin_moved_down_signal();
                        /* update(Idling); // XXX */
                        });
                break;
            case Waiting:
                write("Кабина ожидает. Пока нельзя начать движение вниз.");
                break;
            case MovingUp:
                write("Кабина не может изменить направление своего движения во время движения.");
                break;
            case MovingDown:
                schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
                        // FIXME: DRY
                        write("Кабина опустилась на этаж ниже.");
                        emit cabin_moved_down_signal();
                        });
                break;
        }
    }
}
#endif

void Cabin::move_up()
{
    bool doors_are_closed = m_doors.get_state() == Doors::Closed;

    if (!doors_are_closed)
    {
        write("Кабина не может начать движение вверх с открытыми дверями.");
    }
    else if (m_state == Idling || m_state == MovingUp)
    {
        if (m_state == Idling)
            write("Кабина начала движение вверх.");

        make_moving_up();
    }
    else if (m_state == Waiting)
    {
        write("Кабина ожидает. Пока нельзя начать движение вверх.");
    }
    else if (m_state == MovingDown)
    {
        write("Кабина не может изменить направление своего движения во время движения.");
    }
}

void Cabin::move_down()
{
    bool doors_are_closed = m_doors.get_state() == Doors::Closed;

    if (!doors_are_closed)
    {
        write("Кабина не может начать движение вниз с открытыми дверями.");
    }
    else if (m_state == Idling || m_state == MovingDown)
    {
        if (m_state == Idling)
            write("Кабина начала движение вниз.");

        make_moving_down();
    }
    else if (m_state == Waiting)
    {
        write("Кабина ожидает. Пока нельзя начать движение вниз.");
    }
    else if (m_state == MovingUp)
    {
        write("Кабина не может изменить направление своего движения во время движения.");
    }
}

void Cabin::stop()
{
    bool stopped = m_moveTimer.expiry() <= boost::asio::steady_timer::clock_type::now();

    if (m_state == Idling)
    {
        write("Кабина уже неподвижна.");
    }
    else if (m_state == Waiting)
    {
        write("Кабина уже неподвижна. Ожидает.");
    }
    else if (m_state == MovingUp || m_state == MovingDown)
    {
        if (stopped)
        {
            write("Кабина остановилась.");
            make_idling();

            emit cabin_stopped_signal();
        }
        else
        {
            write("Кабина в движении, нельзя выполнить остановку.");
        }
    }
}

void Cabin::make_moving_up()
{
    update(MovingUp);
    schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
            write("Кабина поднялась на этаж выше.");
            emit cabin_moved_up_signal();
            });
}

void Cabin::make_moving_down()
{
    update(MovingDown);
    schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() {
            write("Кабина опустилась на этаж ниже.");
            emit cabin_moved_down_signal();
            });
}

void Cabin::print_state()
{
    switch (m_state)
    {
        case Idling:
            write("Состояние: Простаивает.");
            break;
        case Waiting:
            write("Состояние: Ожидает.");
            break;
        case MovingUp:
            write("Состояние: Движется вверх.");
            break;
        case MovingDown:
            write("Состояние: Движется вниз.");
            break;
    }

    m_doors.print_state();
}

void Cabin::set_on_cabin_moved_up_callback(Callback callback)
{
    cabin_moved_up_signal = callback;
}

void Cabin::set_on_cabin_moved_down_callback(Callback callback)
{
    cabin_moved_down_signal = callback;
}

void Cabin::set_on_cabin_stopped_callback(Callback callback)
{
    cabin_stopped_signal = callback;
}

void Cabin::update(State state)
{
    m_state = state;
}

void Cabin::write(std::string message)
{
    message = "Кабина: " + message;
    m_writer.write(message);
}
