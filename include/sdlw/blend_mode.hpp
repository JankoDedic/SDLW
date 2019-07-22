#pragma once

#include <SDL2/SDL_blendmode.h>

namespace sdlw {

enum class blend_mode {
    none  = SDL_BLENDMODE_NONE,
    blend = SDL_BLENDMODE_BLEND,
    add   = SDL_BLENDMODE_ADD,
    mod   = SDL_BLENDMODE_MOD
};

} // namespace sdlw
