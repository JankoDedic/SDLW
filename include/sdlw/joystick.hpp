#pragma once

#include <memory>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

using joystick_id = SDL_JoystickID;

class joystick_guid {
public:
    explicit operator SDL_JoystickGUID() const noexcept
    {
        return _guid;
    }

    joystick_guid() = default;

    explicit joystick_guid(SDL_JoystickGUID guid) noexcept
        : _guid{guid}
    {}

    explicit joystick_guid(const char* pch_guid) noexcept
        : _guid{SDL_JoystickGetGUIDFromString(pch_guid)}
    {}

    auto data() const noexcept -> span<const u8, 16>
    {
        return _guid.data;
    }

    auto data() noexcept -> span<u8, 16>
    {
        return _guid.data;
    }

    void string(span<char> psz_guid) noexcept
    {
        SDL_JoystickGetGUIDString(_guid, psz_guid.data(), static_cast<int>(psz_guid.size()));
    }

private:
    SDL_JoystickGUID _guid = {};
};

// clang-format off

enum class joystick_hat_position : u8 {
    centered   = SDL_HAT_CENTERED,
    up         = SDL_HAT_UP,
    right      = SDL_HAT_RIGHT,
    down       = SDL_HAT_DOWN,
    left       = SDL_HAT_LEFT,
    right_up   = SDL_HAT_RIGHTUP,
    right_down = SDL_HAT_RIGHTDOWN,
    left_up    = SDL_HAT_LEFTUP,
    left_down  = SDL_HAT_LEFTDOWN
};

enum class joystick_power_level : std::underlying_type_t<SDL_JoystickPowerLevel> {
    unknown = SDL_JOYSTICK_POWER_UNKNOWN,
    empty   = SDL_JOYSTICK_POWER_EMPTY,
    low     = SDL_JOYSTICK_POWER_LOW,
    medium  = SDL_JOYSTICK_POWER_MEDIUM,
    full    = SDL_JOYSTICK_POWER_FULL,
    wired   = SDL_JOYSTICK_POWER_WIRED,
    max     = SDL_JOYSTICK_POWER_MAX
};

class joystick_ref;

class joystick {
public:
    joystick(joystick_ref) = delete;
    explicit joystick(SDL_Joystick* j) noexcept : _joystick{j} {}
    auto get_pointer() const noexcept -> SDL_Joystick* { return _joystick.get(); }

    explicit joystick(int device_index)
        : _joystick{SDL_JoystickOpen(device_index)}
    {
        if (!_joystick) throw error{};
    }

    auto power_level() const -> joystick_power_level
    {
        const auto pl = SDL_JoystickCurrentPowerLevel(_joystick.get());
        if (pl == SDL_JOYSTICK_POWER_UNKNOWN) {
            throw error{};
        } else {
            return static_cast<joystick_power_level>(pl);
        }
    }

    auto attached() const noexcept -> bool
    {
        return static_cast<bool>(SDL_JoystickGetAttached(_joystick.get()));
    }

    auto axis(int axis) const noexcept -> i16
    {
        const auto result = SDL_JoystickGetAxis(_joystick.get(), axis);
        return result;
    }

    auto ball(int ball) const -> std::pair<int, int>
    {
        auto dx = int{};
        auto dy = int{};
        if (SDL_JoystickGetBall(_joystick.get(), ball, &dx, &dy) < 0) {
            throw error{};
        }
        return {dx, dy};
    }

    auto button(int button) const -> bool
    {
        return static_cast<bool>(SDL_JoystickGetButton(_joystick.get(), button));
    }

    auto guid() const noexcept -> joystick_guid
    {
        const auto result = SDL_JoystickGetGUID(_joystick.get());
        return joystick_guid{result};
    }

    auto hat(int hat) const noexcept -> joystick_hat_position
    {
        return static_cast<joystick_hat_position>(SDL_JoystickGetHat(_joystick.get(), hat));
    }

    auto id() const -> joystick_id
    {
        const auto result = SDL_JoystickInstanceID(_joystick.get());
        if (result < 0) throw error{};
        return result;
    }

    auto name() const -> const char*
    {
        const auto result = SDL_JoystickName(_joystick.get());
        if (!result) throw error{};
        return result;
    }

    auto num_axes() const -> int
    {
        const auto result = SDL_JoystickNumAxes(_joystick.get());
        if (result < 0) throw error{};
        return result;
    }

    auto num_balls() const -> int
    {
        const auto result = SDL_JoystickNumBalls(_joystick.get());
        if (result < 0) throw error{};
        return result;
    }

    auto num_buttons() const -> int
    {
        const auto result = SDL_JoystickNumButtons(_joystick.get());
        if (result < 0) throw error{};
        return result;
    }

    auto num_hats() const -> int
    {
        const auto result = SDL_JoystickNumHats(_joystick.get());
        if (result < 0) throw error{};
        return result;
    }

    static auto event_state() -> bool
    {
        const auto result = SDL_JoystickEventState(SDL_QUERY);
        if (result < 0) {
            throw error{};
        } else {
            return result;
        }
    }

    static void set_event_state(bool enabled) noexcept
    {
        SDL_JoystickEventState(enabled ? SDL_ENABLE : SDL_DISABLE);
    }

    static auto get(joystick_id id) noexcept -> joystick_ref;

    static auto get_name(int device_index) -> const char*
    {
        const auto result = SDL_JoystickNameForIndex(device_index);
        if (!result) throw error{};
        return result;
    }

    static void update() noexcept
    {
        SDL_JoystickUpdate();
    }

protected:
    std::unique_ptr<SDL_Joystick, detail::make_functor<SDL_JoystickClose>> _joystick;
};

class joystick_ref : public joystick {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_joystick); }
    explicit joystick_ref(SDL_Joystick* j) noexcept : joystick{j} {}
    ~joystick_ref() { _joystick.release(); }
};

inline auto joystick::get(joystick_id id) noexcept -> joystick_ref
{
    return joystick_ref{SDL_JoystickFromInstanceID(id)};
}

inline auto joystick_device_guid(int device_index) noexcept -> joystick_guid
{
    const auto result = SDL_JoystickGetDeviceGUID(device_index);
    return joystick_guid{result};
}

inline auto num_joysticks() -> int
{
    const auto result = SDL_NumJoysticks();
    if (result < 0) throw error{};
    return result;
}

// clang-format on

} // namespace sdl
