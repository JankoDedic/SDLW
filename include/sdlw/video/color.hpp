#pragma once

#include <SDL2/SDL.h>

namespace sdlw::video {

using color = SDL_Color;

enum alpha : u8 {
    transparent = SDL_ALPHA_TRANSPARENT,
    opaque      = SDL_ALPHA_OPAQUE
};

} // namespace sdlw::video
