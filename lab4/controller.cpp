#include "controller.h"

#include "algorithm.h"

Controller::Controller(boost::asio::io_context &ioContext, Writer &writer)
    : m_context(ioContext),
    m_writer(writer)
{
    m_cabin = std::make_shared<Cabin>(ioContext, writer);

    s_should_update.connect([this](){ make_updating(); });
    s_target_updated.connect([this](){ make_serving(); });
    s_target_reached.connect([this](){ handle_target_floor_reached(); });
    s_no_target.connect([this](){ make_idling(); });

    s_cabin_should_move_up.connect([this](){ m_cabin->make_moving_up(); });
    s_cabin_should_move_down.connect([this](){ m_cabin->make_moving_down(); });
    s_cabin_should_be_waiting.connect([this](){ m_cabin->make_waiting(); });
    s_cabin_should_be_idling.connect([this](){ m_cabin->make_idling(); });

    m_cabin->s_moved_up.connect([this](){ handle_cabin_moved(); });
    m_cabin->s_moved_down.connect([this](){ handle_cabin_moved(); });
    s_cabin_moved_handled.connect([this](){ make_serving(); });

    m_cabin->s_idling.connect([this](){ make_updating(); });
}

void Controller::handle_button_clicked(int button, bool in_cabin)
{
  if (m_state == Updating || m_state == HandlesCabinMoved) return;

  update_state(HandlesButtonClicked);

  bool updated = false;

  if (in_cabin && button == 0)
  {
      updated = activate_cabin_cancel_button();
  }
  else
  {
      updated = activate_floor_button(button, in_cabin);
  }

  if (updated)
  {
      s_should_update.emit();
  }
}

bool Controller::activate_floor_button(int button, bool in_cabin)
{
    bool updated = false;

    std::string s;

    if (button < 1 || button > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1 - " + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return updated;
    }

    if (in_cabin)
        m_cabin_btns |= (int)(std::pow(2, button - 1));
    else
        m_floor_btns |= (int)(std::pow(2, button - 1));

    s = "Кнопка " + std::to_string(button) + " этажа активирована.";

    updated = true;

    if (in_cabin)
        s += " (в кабине)";
    else
        s += " (на этаже)";

    write(s);

    return updated;
}

bool Controller::activate_cabin_cancel_button()
{
    bool updated = false;

    if (!m_cabin_btns)
    {
        write("Никакие кнопки не нажаты.");
    }
    else
    {
        m_cabin_btns = 0;
        write("Кнопки кабины сброшены.");
        updated = true;
    }

    return updated;
}

void Controller::deactivate_floor_button(int button, bool in_cabin)
{
    std::string s;

    if (button < 1 || button > NUMBER_OF_FLOORS)
    {
        s = "Неверный этаж. (Доступно 1 - " + std::to_string(NUMBER_OF_FLOORS) + ")";
        write(s);
        return;
    }

    int bin_button = (int)(std::pow(2, button - 1));

    // If button is active, deactivate it
    if (in_cabin)
    {
        if (m_cabin_btns & bin_button)
        {
            m_cabin_btns ^= bin_button;
            s = "Кнопка " + std::to_string(button) + " этажа деактивирована (в кабине).";
            write(s);
        }
    }
    else
    {
        if (m_floor_btns & bin_button)
        {
            m_floor_btns ^= bin_button;
            s = "Кнопка " + std::to_string(button) + " этажа деактивирована (на этаже).";
            write(s);
        }
    }
}

void Controller::update_target()
{
    bool is_moving = false;
    bool up = false;

    if (m_last_direction != None)
    {
        is_moving = true;

        if (m_last_direction == Up)
        {
            up = true;
        }
        else
        {
            up = false;
        }
    }

    int next_target = calculate_next_target_floor(m_cur_floor, m_floor_btns, m_cabin_btns, is_moving, up);

    if (m_cur_target != next_target)
    {
        std::string s = "Текущая цель обновлена: " + std::to_string(m_cur_target) + " -> " + std::to_string(next_target);
        m_cur_target = next_target;
        write(s);
    }
}

void Controller::make_idling()
{
    if (m_cabin->is_moving()) return;

    update_state(Idling);

    m_last_direction = None;
}

void Controller::make_updating()
{
    update_state(Updating);

    update_target();

    if (target_exist())
        s_target_updated.emit();
    else
        s_no_target.emit();
}

void Controller::make_serving()
{
    update_state(Serving);

    if (target_exist())
    {
        if (m_cur_target > m_cur_floor)
        {
            m_last_direction = Up;
            s_cabin_should_move_up.emit();
        }
        else if (m_cur_target < m_cur_floor)
        {
            m_last_direction = Down;
            s_cabin_should_move_down.emit();
        }
        else
        {
            s_target_reached.emit();
        }
    }
    else
    {
        s_cabin_should_be_idling.emit();
    }
}

void Controller::handle_cabin_moved()
{
    if (m_last_direction == None) return;

    update_state(HandlesCabinMoved);

    if (m_last_direction == Up)
    {
        increment_current_floor();
        write("Кабина поднялась на этаж " + std::to_string(m_cur_floor) + ".");
    }
    else if (m_last_direction == Down)
    {
        decrement_current_floor();
        write("Кабина опустилась на этаж " + std::to_string(m_cur_floor) + ".");
    }

    s_cabin_moved_handled.emit();
}

void Controller::handle_target_floor_reached()
{
    update_state(TargetFloorReached);

    deactivate_floor_button(m_cur_floor, ON_FLOOR);
    deactivate_floor_button(m_cur_floor, IN_CABIN);

    update_target();

    s_cabin_should_be_waiting.emit();
}

void Controller::update_state(State new_state)
{
    m_state = new_state;
}

void Controller::write(std::string message)
{
    message = "Контроллер: " + message;
    m_writer.write(message);
}

#if 0
void Controller::print_state()
{
    std::string s;

    switch (m_state)
    {
        case Idling:
            s = "Простаивает.";
            break;
        case Updating:
            s = "Обновляется.";
            break;
        case Serving:
            s = "Обслуживает.";
            break;
        case TargetFloorReached:
            s = "Достигнут целевой этаж.";
            break;
        case HandlesCabinMoved:
            s = "Обрабатывает перемещение кабины.";
            break;
        case HandlesButtonClicked:
            s = "Обрабатывает нажатие кнопки.";
            break;
    }

    write("Состояние: " + s);

    std::bitset<NUMBER_OF_FLOORS> floor_btns(m_floor_btns);
    std::bitset<NUMBER_OF_FLOORS> cabin_btns(m_cabin_btns);

    s = "Кнопки на этажах: " + floor_btns.to_string();
    write(s);

    s = "Кнопки в кабине : " + cabin_btns.to_string();
    write(s);

    s = "Текущий этаж: " + std::to_string(m_cur_floor);
    write(s);

    std::string target;

    if (target_exist())
        target = std::to_string(m_cur_target);
    else
        target = "Отсутствует.";

    s = "Целевой этаж: " + target;
    write(s);

    s = "Предыдущее направление движения: ";

    if (m_last_direction == None)
        s += "Неподвижна.";
    else if (m_last_direction == Up)
        s += "Вверх.";
    else if (m_last_direction == Down)
        s += "Вниз.";

    write(s);

    m_cabin->print_state();
}
#endif
