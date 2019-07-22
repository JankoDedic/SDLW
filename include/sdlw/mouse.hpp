#pragma once

#include <SDL2/SDL_mouse.h>

#include <memory>
#include <optional>

#include <sdlw/error.hpp>
#include <sdlw/rectangle.hpp>
#include <sdlw/video.hpp>
#include "sdlw/utility.hpp"

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

enum class mouse_button : u8 {
    left   = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right  = SDL_BUTTON_RIGHT,
    x1     = SDL_BUTTON_X1,
    x2     = SDL_BUTTON_X2
};

enum class mouse_button_state : u32 {
    left   = SDL_BUTTON_LMASK,
    middle = SDL_BUTTON_MMASK,
    right  = SDL_BUTTON_RMASK,
    x1     = SDL_BUTTON_X1MASK,
    x2     = SDL_BUTTON_X2MASK
};

enum class mouse_wheel_direction : u32 {
    normal  = SDL_MOUSEWHEEL_NORMAL,
    flipped = SDL_MOUSEWHEEL_FLIPPED
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(mouse_button_state);

constexpr
mouse_button_state
button_mask(mouse_button b) noexcept
{
    return static_cast<mouse_button_state>(SDL_BUTTON(static_cast<u8>(b)));
}


inline
std::pair<mouse_button_state, point>
state() noexcept
{
    auto pos = point();
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
    return std::make_pair(bstate, pos);
}

inline
std::pair<mouse_button_state, point>
global_state() noexcept
{
    auto pos = point();
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
    return std::make_pair(bstate, pos);
}

inline
std::optional<window_ref>
mouse_focus() noexcept
{
    if (const auto ptr = SDL_GetMouseFocus()) {
        return window_ref(ptr);
    } else {
        return std::nullopt;
    }
}

inline
bool
capture_mouse(bool enabled) noexcept
{
    return SDL_CaptureMouse(enabled ? SDL_TRUE : SDL_FALSE) == 0;
}

inline
void
warp_mouse_in_window(const window& win, const point& p) noexcept
{
    SDL_WarpMouseInWindow(win.get_pointer(), p.x, p.y);
}

inline
bool
warp_mouse_global(const point& coordinate) noexcept
{
    return SDL_WarpMouseGlobal(coordinate.x, coordinate.y) == 0;
}

struct relative_mode {
    static
    bool
    is_enabled() noexcept
    {
        return SDL_GetRelativeMouseMode();
    }

    static
    void
    enable()
    {
        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
            throw error();
        }
    }

    static
    void
    disable()
    {
        if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) {
            throw error();
        }
    }

    static
    std::pair<mouse_button_state, point>
    get_state() noexcept
    {
        auto p = point();
        const auto sdl_button_state = SDL_GetRelativeMouseState(&p.x, &p.y);
        const auto bstate = static_cast<mouse_button_state>(sdl_button_state);
        return std::make_pair(bstate, p);
    }
};

} // namespace sdlw
