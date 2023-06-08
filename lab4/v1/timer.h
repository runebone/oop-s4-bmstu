#pragma once

#include <boost/asio.hpp>

// TODO: concepts
template <typename Timer, typename Callback>
void schedule_timer(Timer &timer, int duration, Callback callback)
{
    timer.expires_after(std::chrono::milliseconds(duration));
    timer.async_wait([callback](const boost::system::error_code &error) {
            if (!error) callback();
            });
}
