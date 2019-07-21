#pragma once

#include <SDL2/SDL.h>

#include <sdlw/video.hpp>

namespace sdlw {

struct screen_keyboard {
    static auto is_supported() noexcept -> bool {
        return SDL_HasScreenKeyboardSupport();
    }

    static auto is_shown(const window &win) noexcept -> bool {
        return SDL_IsScreenKeyboardShown(win.get_pointer());
    }
};

} // namespace sdlw
