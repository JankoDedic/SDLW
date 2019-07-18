#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>

namespace sdlw {

enum class mouse_wheel_direction : u32 {
    normal  = SDL_MOUSEWHEEL_NORMAL,
    flipped = SDL_MOUSEWHEEL_FLIPPED
};

} // namespace sdlw
