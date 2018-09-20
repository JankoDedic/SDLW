#pragma once

#include <SDL2/SDL_ttf.h>

namespace sdlw::ttf {

struct subsystem {
    subsystem()
    {
        if (TTF_Init() < 0) {
            throw error();
        }
    }

    ~subsystem() noexcept
    {
        TTF_Quit();
    }
};

} // namespace sdlw::ttf
