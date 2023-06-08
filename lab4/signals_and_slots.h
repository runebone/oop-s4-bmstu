#pragma once

#include <iostream>
#include <functional>

#define SIGNALS
#define SLOTS

template<typename... Args>
class Signal {
public:
    using SlotType = std::function<void(Args...)>;

    void connect(SlotType slot) {
        slots.push_back(slot);
    }

    void emit(Args... args) {
        for (const auto& callback : slots) {
            callback(args...);
        }
    }

private:
    std::vector<SlotType> slots;
};
