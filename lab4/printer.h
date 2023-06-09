#pragma once

#include "doors.h"
#include "cabin.h"
#include "controller.h"

#include "writer.h"

class Printer
{
public:
    virtual void print() = 0;
};

class DoorsPrinter : public Printer
{
public:
    DoorsPrinter(Writer &writer) : m_writer(writer) {}

    void set_info(Doors::Info info) { m_info = info; }

    void print()
    {
        std::string s;

        switch (m_info.state)
        {
            case Doors::Closed:
                s = "Закрыты.";
                break;
            case Doors::Closing:
                s = "Закрываются.";
                break;
            case Doors::Opened:
                s = "Открыты.";
                break;
            case Doors::Opening:
                s = "Открываются.";
                break;
            case Doors::Waiting:
                s = "Ожидают.";
                break;
        }

        write("Состояние: " + s);

        s = std::to_string(m_info.open_time);
        write("Таймер открытия: " + s);

        s = std::to_string(m_info.close_time);
        write("Таймер закрытия: " + s);

        s = std::to_string(m_info.wait_time);
        write("Таймер ожидания: " + s);
    }

    void write(std::string message) { m_writer.write(message); }

private:
    Writer &m_writer;
    Doors::Info m_info;
};

class CabinPrinter : public Printer
{
public:
    CabinPrinter(Writer &writer) : m_writer(writer) {}

    void set_info(Cabin::Info info) { m_info = info; }

    void print()
    {
        std::string s;

        switch (m_info.state)
        {
            case Cabin::Idling:
                s = "Простаивает.";
                break;
            case Cabin::Waiting:
                s = "Ожидает.";
                break;
            case Cabin::MovingUp:
                s = "Едет вверх.";
                break;
            case Cabin::MovingDown:
                s = "Едет вниз.";
                break;
        }

        write("Состояние: " + s);

        s = std::to_string(m_info.move_time);
        write("Таймер движения: " + s);
    }

    void write(std::string message) { m_writer.write(message); }

private:
    Writer &m_writer;
    Cabin::Info m_info;
};

class ControllerPrinter : public Printer
{
public:
    ControllerPrinter(Writer &writer) : m_writer(writer) {}

    void set_info(Controller::Info info) { m_info = info; }

    void print()
    {
        std::string s;

        switch (m_info.state)
        {
            case Controller::Idling:
                s = "Простаивает.";
                break;
            case Controller::Updating:
                s = "Обновляется.";
                break;
            case Controller::Serving:
                s = "Обслуживает.";
                break;
            case Controller::TargetFloorReached:
                s = "Достигнут целевой этаж.";
                break;
            case Controller::HandlesCabinMoved:
                s = "Обрабатывает перемещение кабины.";
                break;
            case Controller::HandlesButtonClicked:
                s = "Обрабатывает нажатие кнопки.";
                break;
        }

        write("Состояние: " + s);

        s = "Кнопки на этажах: " + m_info.floor_buttons.to_string();
        write(s);

        s = "Кнопки в кабине : " + m_info.cabin_buttons.to_string();
        write(s);

        s = "Текущий этаж: " + std::to_string(m_info.current_floor);
        write(s);

        std::string target;

        if (m_info.target_floor != 0)
            target = std::to_string(m_info.target_floor);
        else
            target = "Отсутствует.";

        s = "Целевой этаж: " + target;
        write(s);

        s = "Предыдущее направление движения: ";

        if (m_info.last_direction == 0)
            s += "Неподвижна.";
        else if (m_info.last_direction == 1)
            s += "Вверх.";
        else if (m_info.last_direction == -1)
            s += "Вниз.";

        write(s);
    }

    void write(std::string message) { m_writer.write(message); }

private:
    Writer &m_writer;
    Controller::Info m_info;
};
