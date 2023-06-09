#pragma once

#include <string>
#include <boost/asio.hpp>

#include "controller.h"

#include "writer.h"

#include "signals_and_slots.h"

#include "printer.h"

class Elevator
{
public:
    Elevator(boost::asio::io_context &ioContext) : Elevator(ioContext, Writer()) {}
    Elevator(boost::asio::io_context &ioContext, Writer &&writer) : Elevator(ioContext, writer) {}
    Elevator(boost::asio::io_context &ioContext, Writer &writer);

    /* void print_state() { m_controller->print_state(); } */

    void print()
    {
        m_doors_printer.set_info(m_controller->m_cabin->get_doors_info());
        m_cabin_printer.set_info(m_controller->m_cabin->get_info());
        m_controller_printer.set_info(m_controller->get_info());

        m_controller_printer.print();
        m_cabin_printer.print();
        m_doors_printer.print();
    }

    void activate_floor_button(int button, bool in_cabin);
    void activate_cabin_cancel_button();

private SIGNALS:
    Signal<int, bool> s_button_clicked;

private:
    void deactivate_floor_button(int button, bool in_cabin);

    Writer &m_writer;
    boost::asio::io_context &m_context;

    std::shared_ptr<Controller> m_controller;

    // XXX
    DoorsPrinter m_doors_printer;
    CabinPrinter m_cabin_printer;
    ControllerPrinter m_controller_printer;
};
