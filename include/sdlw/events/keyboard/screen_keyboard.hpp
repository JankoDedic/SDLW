#pragma once

#include <SDL2/SDL.h>

#include <sdlw/video/window.hpp>

namespace sdlw::events::keyboard {

struct screen_keyboard {
    static
    bool
    is_supported() noexcept
    {
        return SDL_HasScreenKeyboardSupport();
    }

    static
    bool
    is_shown(const video::window& win) noexcept
    {
        return SDL_IsScreenKeyboardShown(win.get_pointer());
    }
};

} // namespace sdlw::events::keyboard
