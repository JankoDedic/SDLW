#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

struct platform {
    static
    const char*
    name() noexcept
    {
        return SDL_GetPlatform();
    }
};

} // namespace sdlw
