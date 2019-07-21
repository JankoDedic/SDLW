#pragma once

#include <optional>
#include <tuple>

#include <sdlw/video/point.hpp>
#include <sdlw/video/window.hpp>
#include <sdlw/events/mouse/mouse_button_state.hpp>

namespace sdlw {

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

} // namespace sdlw
