#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>

namespace sdlw {

using finger_id = i64;

class finger {
    SDL_Finger _finger;

public:
    explicit
    operator SDL_Finger() const noexcept
    {
        return _finger;
    }

    explicit
    finger(const SDL_Finger& finger) noexcept
        : _finger(finger)
    {
    }

    finger_id
    id() const noexcept
    {
        return static_cast<finger_id>(_finger.id);
    }

    float
    x() const noexcept
    {
        return _finger.x;
    }

    float
    y() const noexcept
    {
        return _finger.y;
    }
};

} // namespace sdlw
