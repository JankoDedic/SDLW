#pragma once

#include <SDL2/SDL.h>

namespace sdlw::video {

struct subsystem {
    subsystem(const char* driver_name)
    {
        if (SDL_VideoInit(driver_name) < 0) {
            throw error();
        }
    }

    ~subsystem() noexcept
    {
        SDL_VideoQuit();
    }
};

} // namespace sdlw::video
