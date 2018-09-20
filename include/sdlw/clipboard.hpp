#pragma once

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>

namespace sdlw {

struct clipboard {
    static
    bool
    has_text() noexcept
    {
        return SDL_HasClipboardText();
    }

    static
    const char*
    text() noexcept
    {
        return SDL_GetClipboardText();
    }

    static
    void
    set_text(const char* text)
    {
        if (SDL_SetClipboardText(text) < 0) {
            throw error();
        }
    }
};

} // namespace sdlw
