#pragma once

#include <SDL2/SDL_clipboard.h>

#include <sdlw/error.hpp>

namespace sdl {

struct clipboard {
    static auto has_text() noexcept -> bool
    {
        return SDL_HasClipboardText();
    }

    static auto text() noexcept -> const char*
    {
        return SDL_GetClipboardText();
    }

    static void set_text(const char* text)
    {
        if (SDL_SetClipboardText(text) < 0) {
            throw error{};
        }
    }
};

} // namespace sdl
