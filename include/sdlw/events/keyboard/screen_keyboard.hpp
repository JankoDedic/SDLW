#pragma once

#include <SDL2/SDL.h>

#include <sdlw/video/window.hpp>

namespace sdlw::events::keyboard {

struct screen_keyboard {
    static auto is_supported() noexcept -> bool {
        return SDL_HasScreenKeyboardSupport();
    }

    static auto is_shown(const video::window &win) noexcept -> bool {
        return SDL_IsScreenKeyboardShown(win.get_pointer());
    }
};

} // namespace sdlw::events::keyboard
