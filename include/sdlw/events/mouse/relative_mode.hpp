#pragma once

#include <utility>

#include <sdlw/error.hpp>
#include <sdlw/video/point.hpp>
#include <sdlw/events/mouse/button_state.hpp>

namespace sdlw {

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
