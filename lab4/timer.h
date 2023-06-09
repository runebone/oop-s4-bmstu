#pragma once

#include <chrono>
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

class AsioTimer
{
    using Callback = std::function<void()>;
public:
    AsioTimer(boost::asio::io_context &ioContext) : m_timer(ioContext) {}

    void schedule()
    {
        schedule(m_duration);
    }

    void schedule(int duration)
    {
        schedule_timer(m_timer, duration, m_callback);
    }

    void cancel()
    {
        m_timer.cancel();
    }

    int remaining_time() const
    {
        boost::asio::steady_timer::duration remainingTime = m_timer.expires_from_now();
        std::chrono::milliseconds remainingTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(remainingTime);
        int milliseconds = static_cast<int>(remainingTimeMs.count());

        return milliseconds > 0 ? milliseconds : 0;
    }

    bool expired() const
    {
        return remaining_time() == 0;
    }

    void set_duration(int duration_ms)
    {
        m_duration = duration_ms;
    }

    void set_callback(Callback callback)
    {
        m_callback = callback;
    }

private:
    boost::asio::steady_timer m_timer;
    int m_duration = 1000;
    Callback m_callback = [](){};
};
