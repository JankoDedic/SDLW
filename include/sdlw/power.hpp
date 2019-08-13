#pragma once

#include <SDL2/SDL_power.h>

namespace sdl {

// clang-format off

enum class battery_state {
    unknown    = SDL_POWERSTATE_UNKNOWN,
    on_battery = SDL_POWERSTATE_ON_BATTERY,
    no_battery = SDL_POWERSTATE_NO_BATTERY,
    charging   = SDL_POWERSTATE_CHARGING,
    charged    = SDL_POWERSTATE_CHARGED
};

// clang-format on

class battery_info {
    battery_state _battery_state;
    int _seconds_left;
    int _percentage_left;

public:
    battery_info(battery_state battery_state_, int seconds_left, int percentage_left) noexcept
        : _battery_state{battery_state_}
        , _seconds_left{seconds_left}
        , _percentage_left{percentage_left}
    {}

    auto state() const noexcept -> battery_state
    {
        return _battery_state;
    }

    auto seconds_left() const noexcept -> int
    {
        return _seconds_left;
    }

    auto percentage_left() const noexcept -> int
    {
        return _percentage_left;
    }
};

inline auto get_battery_info() noexcept -> battery_info
{
    auto seconds_left = int{};
    auto percentage_left = int{};
    const auto power_state = SDL_GetPowerInfo(&seconds_left, &percentage_left);
    const auto battery_state_ = static_cast<battery_state>(power_state);
    return {battery_state_, seconds_left, percentage_left};
}

} // namespace sdl
