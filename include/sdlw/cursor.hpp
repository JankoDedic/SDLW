#pragma once

#include <SDL2/SDL.h>

#include <sdlw/utility.hpp>
#include <sdlw/point.hpp>
#include <sdlw/size.hpp>
#include <sdlw/surface.hpp>

namespace sdlw {

enum class system_cursor {
    arrow              = SDL_SYSTEM_CURSOR_ARROW,
    ibeam              = SDL_SYSTEM_CURSOR_IBEAM,
    wait               = SDL_SYSTEM_CURSOR_WAIT,
    crosshair          = SDL_SYSTEM_CURSOR_CROSSHAIR,
    wait_arrow         = SDL_SYSTEM_CURSOR_WAITARROW,
    size_nwse          = SDL_SYSTEM_CURSOR_SIZENWSE,
    size_nesw          = SDL_SYSTEM_CURSOR_SIZENESW,
    size_we            = SDL_SYSTEM_CURSOR_SIZEWE,
    size_ns            = SDL_SYSTEM_CURSOR_SIZENS,
    size_all           = SDL_SYSTEM_CURSOR_SIZEALL,
    no                 = SDL_SYSTEM_CURSOR_NO,
    hand               = SDL_SYSTEM_CURSOR_HAND,
    num_system_cursors = SDL_NUM_SYSTEM_CURSORS
};

class cursor {
    using deleter = detail::make_functor<SDL_FreeCursor>;

    std::unique_ptr<SDL_Cursor, deleter> _cursor;

public:
    cursor() noexcept = default;

    cursor(SDL_Cursor* pointer) noexcept
        : _cursor(pointer)
    {
    }

    cursor(
        const u8* data,
        const u8* mask,
        const size& sz,
        const point& topleft_corner)
    {
        /* const auto &[x, y] = topleft_corner; */
        const auto x = topleft_corner.x;
        const auto y = topleft_corner.y;
        if (const auto ptr = SDL_CreateCursor(data, mask, sz.w, sz.h, x, y)) {
            *this = cursor(ptr);
        } else {
            throw error();
        }
    }

    cursor(const surface& surf, const point& topleft_corner)
    {
        /* const auto [x, y] = topleft_corner; */
        const auto x = topleft_corner.x;
        const auto y = topleft_corner.y;
        if (const auto ptr = SDL_CreateColorCursor(surf.get_pointer(), x, y)) {
            *this = cursor(ptr);
        } else {
            throw error();
        }
    }

    cursor(system_cursor sysc)
    {
        const auto c = static_cast<SDL_SystemCursor>(sysc);
        if (const auto ptr = SDL_CreateSystemCursor(c)) {
            *this = cursor(ptr);
        } else {
            throw error();
        }
    }

    SDL_Cursor*
    get_pointer() const noexcept
    {
        return _cursor.get();
    }
};

} // namespace sdlw
