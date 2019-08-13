#pragma once

#include <SDL2/SDL_platform.h>

namespace sdl {

struct platform {
    static auto name() noexcept -> const char*
    {
        return SDL_GetPlatform();
    }
};

} // namespace sdl
