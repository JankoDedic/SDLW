#pragma once

#include <SDL2/SDL.h>

#include <sdlw/events/keyboard/keycode.hpp>
#include <sdlw/events/keyboard/keymod.hpp>
#include <sdlw/events/keyboard/scancode.hpp>

namespace sdlw::events::keyboard {

class key {
    SDL_Keysym _keysym;

public:
    constexpr explicit operator SDL_Keysym() const noexcept {
        return _keysym;
    }

    key() noexcept = default;

    constexpr explicit key(SDL_Keysym keysym) noexcept : _keysym(keysym) { }

    constexpr auto scancode() const noexcept -> sdlw::events::keyboard::scancode {
        return static_cast<sdlw::events::keyboard::scancode>(_keysym.scancode);
    }

    constexpr auto keycode() const noexcept -> sdlw::events::keyboard::keycode {
        return static_cast<sdlw::events::keyboard::keycode>(_keysym.sym);
    }

    constexpr auto keymod() const noexcept -> sdlw::events::keyboard::keymod {
        return static_cast<sdlw::events::keyboard::keymod>(_keysym.mod);
    }
};

} // namespace sdlw::events::keyboard
