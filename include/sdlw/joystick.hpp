#pragma once

#include <SDL2/SDL_joystick.h>

#include <sdlw/types.hpp>

namespace sdlw {

using joystick_id = SDL_JoystickID;

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

} // namespace sdlw
