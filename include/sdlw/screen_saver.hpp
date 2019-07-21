#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

struct screen_saver {
    static
    bool
    is_enabled() noexcept
    {
        return SDL_IsScreenSaverEnabled();
    }

    static
    void
    enable() noexcept
    {
        SDL_EnableScreenSaver();
    }

    static
    void
    disable() noexcept
    {
        SDL_DisableScreenSaver();
    }
};

} // namespace sdlw
