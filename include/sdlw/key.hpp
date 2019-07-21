#pragma once

#include <SDL2/SDL.h>

#include <sdlw/keycode.hpp>
#include <sdlw/keymod.hpp>
#include <sdlw/scancode.hpp>

namespace sdlw {

class key {
    SDL_Keysym _keysym;

public:
    constexpr explicit operator SDL_Keysym() const noexcept {
        return _keysym;
    }

    key() noexcept = default;

    constexpr explicit key(SDL_Keysym keysym) noexcept : _keysym(keysym) { }

    constexpr auto scancode() const noexcept -> sdlw::scancode {
        return static_cast<sdlw::scancode>(_keysym.scancode);
    }

    constexpr auto keycode() const noexcept -> sdlw::keycode {
        return static_cast<sdlw::keycode>(_keysym.sym);
    }

    constexpr auto keymod() const noexcept -> sdlw::keymod {
        return static_cast<sdlw::keymod>(_keysym.mod);
    }
};

} // namespace sdlw
