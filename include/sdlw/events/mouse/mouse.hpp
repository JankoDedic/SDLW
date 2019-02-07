#pragma once

#include <optional>
#include <tuple>

#include <sdlw/video/point.hpp>
#include <sdlw/video/window.hpp>
#include <sdlw/events/mouse/button_state.hpp>

namespace sdlw::events::mouse {

inline
std::pair<button_state, video::point>
state() noexcept
{
    auto pos = video::point();
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<button_state>(sdl_button_state);
    return std::make_pair(bstate, pos);
}

inline
std::pair<button_state, video::point>
global_state() noexcept
{
    auto pos = video::point();
    const auto sdl_button_state = SDL_GetMouseState(&pos.x, &pos.y);
    const auto bstate = static_cast<button_state>(sdl_button_state);
    return std::make_pair(bstate, pos);
}

inline
/* video::window* */
std::optional<video::window_ref>
focus_owner() noexcept
{
    /* static auto s = detail::storage<video::window>(); */
    /* if (const auto pwindow = SDL_GetMouseFocus()) { */
    /*     return new (&s) video::window(pwindow); */
    /* } else { */
    /*     return nullptr; */
    /* } */

    if (const auto ptr = SDL_GetMouseFocus()) {
        return video::window_ref(ptr);
    } else {
        return std::nullopt;
    }
}

inline
bool
capture(bool enabled) noexcept
{
    return SDL_CaptureMouse(enabled ? SDL_TRUE : SDL_FALSE) == 0;
}

inline
void
warp_in_window(const video::window& win, const video::point& p) noexcept
{
    SDL_WarpMouseInWindow(win.get_pointer(), p.x, p.y);
}

inline
bool
warp_global(const video::point& coordinate) noexcept
{
    return SDL_WarpMouseGlobal(coordinate.x, coordinate.y) == 0;
}

} // namespace sdlw::events::mouse
