#pragma once

#include <SDL2/SDL.h>

namespace sdlw::video {

struct driver {
    static
    int
    count() noexcept
    {
        return SDL_GetNumVideoDrivers();
    }

    static
    const char*
    name(int driver_index) noexcept
    {
        return SDL_GetVideoDriver(driver_index);
    }

    static
    const char*
    current() noexcept
    {
        return SDL_GetCurrentVideoDriver();
    }
};

} // namespace sdlw::video
