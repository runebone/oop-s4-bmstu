#pragma once

#define CABIN_MOVE_ONE_FLOOR 5000

#include <string>
#include <boost/asio.hpp>

#include "doors.h"
#include "writer.h"
#include "timer.h"

class Cabin
{
public:
    enum State
    {
        Idling,
        Waiting,
        MovingUp,
        MovingDown
    };

    Cabin(boost::asio::io_context &ioContext) : Cabin(ioContext, Writer()) {}
    Cabin(boost::asio::io_context &ioContext, Writer &&writer) : Cabin(ioContext, writer) {}
    Cabin(boost::asio::io_context &ioContext, Writer &writer)
        : m_context(ioContext),
            m_doors(ioContext, writer),
            m_writer(writer),
            m_moveTimer(ioContext)
    {
        m_doors.set_on_opened_callback([this](){
                update(Waiting);
            });

        m_doors.set_on_closed_callback([this](){
                update(Idling);
            });
    }

    void open_doors()
    {
        switch (m_state)
        {
            case Idling:
            case Waiting:
                m_doors.open();
                break;
            case MovingUp:
            case MovingDown:
                write("Кабина не может открыть двери во время движения. Игнорируем запрос.");
                break;
        }
    }

    void close_doors()
    {
        switch (m_state)
        {
            case Idling:
            case Waiting:
                m_doors.close();
                break;
            case MovingUp:
            case MovingDown:
                write("Кабина движется, двери уже закрыты. Игнорируем запрос.");
                break;
        }
    }

    void move_up()
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
                            update(Idling); // XXX
                            });
                    break;
                case Waiting:
                    write("Кабина ожидает. Пока нельзя начать движение вверх.");
                    break;
                case MovingUp:
                    /* schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() { */
                    /*     // FIXME: DRY */
                    /*     write("Кабина поднялась на этаж выше."); */
                    /*     emit cabin_moved_up_signal(); */
                    /* }); */
                    break;
                case MovingDown:
                    write("Кабина не может изменить направление своего движения во время движения.");
                    break;
            }
        }
    }

    void move_down()
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
                            update(Idling); // XXX
                            });
                    break;
                case Waiting:
                    write("Кабина ожидает. Пока нельзя начать движение вниз.");
                    break;
                case MovingUp:
                    write("Кабина не может изменить направление своего движения во время движения.");
                    break;
                case MovingDown:
                    /* schedule_timer(m_moveTimer, CABIN_MOVE_ONE_FLOOR, [this]() { */
                    /*         // FIXME: DRY */
                    /*         write("Кабина опустилась на этаж ниже."); */
                    /*         emit cabin_moved_down_signal(); */
                    /*         }); */
                    break;
            }
        }
    }

    void stop()
    {
        bool stopped = m_moveTimer.expiry() <= boost::asio::steady_timer::clock_type::now();

        switch (m_state)
        {
            case Idling:
                write("Кабина уже неподвижна.");
                break;
            case Waiting:
                write("Кабина уже неподвижна. Ожидает.");
                break;
            case MovingUp:
            case MovingDown:
                if (stopped)
                {
                    write("Кабина остановилась.");
                    update(Idling);
                    emit cabin_stopped_signal();
                }
                else
                {
                    write("Кабина в движении, нельзя выполнить остановку.");
                }
                break;
        }
    }

    State get_state() { return m_state; }

    void print_state()
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

    Doors::State get_doors_state() { return m_doors.get_state(); }

    void set_on_cabin_moved_up_callback(Callback callback)
    {
        cabin_moved_up_signal = callback;
    }

    void set_on_cabin_moved_down_callback(Callback callback)
    {
        cabin_moved_down_signal = callback;
    }

    void set_on_cabin_stopped_callback(Callback callback)
    {

        cabin_stopped_signal = callback;
    }

private:
    void update(State state)
    {
        m_state = state;
    }

    void write(std::string message)
    {
        message = "Кабина: " + message;
        m_writer.write(message);
    }

    State m_state = Idling;
public:
    Doors m_doors;
private:
    Writer &m_writer;
    boost::asio::io_context &m_context;
    boost::asio::steady_timer m_moveTimer;

    Callback cabin_moved_up_signal = [](){};
    Callback cabin_moved_down_signal = [](){};
    Callback cabin_stopped_signal = [](){};
};
