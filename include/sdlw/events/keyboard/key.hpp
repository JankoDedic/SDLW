#pragma once

#include <SDL2/SDL.h>

#include <sdlw/events/keyboard/keycode.hpp>
#include <sdlw/events/keyboard/keymod.hpp>
#include <sdlw/events/keyboard/scancode.hpp>

namespace sdlw::events::keyboard {

class key {
    SDL_Keysym _keysym;

public:
    using keycode_type = keycode;
    using keymod_type = keymod;
    using scancode_type = scancode;

    constexpr explicit operator SDL_Keysym() const noexcept {
        return _keysym;
    }

    key() noexcept = default;

    constexpr explicit key(SDL_Keysym keysym) noexcept : _keysym(keysym) { }

    constexpr auto scancode() const noexcept -> scancode_type {
        return static_cast<scancode_type>(_keysym.scancode);
    }

    constexpr auto keycode() const noexcept -> keycode_type {
        return static_cast<keycode_type>(_keysym.sym);
    }

    constexpr auto keymod() const noexcept -> keymod_type {
        return static_cast<keymod_type>(_keysym.mod);
    }
};

} // namespace sdlw::events::keyboard
