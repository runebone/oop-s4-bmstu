#include "elevator.h"

Elevator::Elevator(boost::asio::io_context &ioContext, Writer &writer)
    : m_context(ioContext), m_writer(writer),
    m_doors_printer(writer),
    m_cabin_printer(writer),
    m_controller_printer(writer)
{
    m_controller = std::make_shared<Controller>(ioContext, writer);

    s_button_clicked.connect([this](int button, bool in_cabin){
            m_controller->handle_button_clicked(button, in_cabin);
            });
}

void Elevator::activate_floor_button(int button, bool in_cabin)
{
    s_button_clicked.emit(button, in_cabin);
}

void Elevator::activate_cabin_cancel_button()
{
    s_button_clicked.emit(0, IN_CABIN);
}
