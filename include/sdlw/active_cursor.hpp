#pragma once

#include <optional>

#include <SDL2/SDL.h>

#include <sdlw/cursor.hpp>

namespace sdlw {

struct active_cursor {
    static
    cursor*
    get() noexcept
    {
        static auto s = detail::storage<cursor>();
        if (const auto pcursor = SDL_GetCursor()) {
            return new (&s) cursor(pcursor);
        } else {
            return nullptr;
        }
    }

    static
    void
    set(const cursor& c) noexcept
    {
        SDL_SetCursor(c.get_pointer());
    }

    static
    void
    redraw() noexcept
    {
        SDL_SetCursor(nullptr);
    }

    static
    bool
    is_shown()
    {
        if (const auto result = SDL_ShowCursor(SDL_QUERY); result < 0) {
            throw error();
        } else {
            return static_cast<bool>(result);
        }
    }

    static
    void
    show()
    {
        if (const auto result = SDL_ShowCursor(SDL_ENABLE); result < 0) {
            throw error();
        }
    }

    static
    void
    hide()
    {
        if (const auto result = SDL_ShowCursor(SDL_DISABLE); result < 0) {
            throw error();
        }
    }
};

} // namespace sdlw
