#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>

namespace sdlw::events::mouse {

enum class button : u8 {
    left   = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right  = SDL_BUTTON_RIGHT,
    x1     = SDL_BUTTON_X1,
    x2     = SDL_BUTTON_X2
};

} // namespace sdlw::events::mouse
