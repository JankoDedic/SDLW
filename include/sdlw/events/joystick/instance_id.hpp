#pragma once

#include <SDL2/SDL.h>

namespace sdlw::events::joystick {

class instance_id {
    SDL_JoystickID _id;

public:
    constexpr explicit
    operator SDL_JoystickID() const noexcept
    {
        return _id;
    }

    instance_id() noexcept = default;

    constexpr explicit
    instance_id(SDL_JoystickID id) noexcept
        : _id(id)
    {
    }
};

constexpr
bool
operator==(instance_id lhs, instance_id rhs) noexcept
{
    auto l = static_cast<SDL_JoystickID>(lhs);
    auto r = static_cast<SDL_JoystickID>(rhs);
    return l == r;
}

constexpr
bool
operator!=(instance_id lhs, instance_id rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace sdlw::events::joystick
