#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

struct video_subsystem {
    video_subsystem(const char* driver_name)
    {
        if (SDL_VideoInit(driver_name) < 0) {
            throw error();
        }
    }

    ~video_subsystem() noexcept
    {
        SDL_VideoQuit();
    }
};

} // namespace sdlw
