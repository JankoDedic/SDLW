#pragma once

#include <SDL2/SDL_platform.h>

namespace sdl {

inline auto platform() noexcept -> const char*
{
    return SDL_GetPlatform();
}

} // namespace sdl
