#pragma once

#include <optional>

#include <sdlw/window.hpp>
#include <sdlw/keycode.hpp>
#include <sdlw/keymod.hpp>
#include <sdlw/scancode.hpp>

namespace sdlw {

inline auto get_keycode(const char *name) -> keycode {
    if (const auto result = SDL_GetKeyFromName(name); result == SDLK_UNKNOWN) {
        throw error();
    } else {
        return static_cast<keycode>(result);
    }
}

inline auto get_keycode(scancode sc) noexcept -> keycode {
    const auto sdl_scancode = static_cast<SDL_Scancode>(sc);
    const auto sdl_keycode = SDL_GetKeyFromScancode(sdl_scancode);
    return static_cast<keycode>(sdl_keycode);
}

inline auto name_of(keycode kc) noexcept -> const char * {
    return SDL_GetKeyName(static_cast<SDL_Keycode>(kc));
}

inline auto get_scancode(const char *name) -> scancode {
    const auto result = SDL_GetScancodeFromName(name);
    if (result == SDL_SCANCODE_UNKNOWN) {
        throw error();
    } else {
        return static_cast<scancode>(result);
    }
}

inline auto get_scancode(keycode kc) noexcept -> scancode {
    const auto sdl_keycode = static_cast<SDL_Keycode>(kc);
    const auto sdl_scancode = SDL_GetScancodeFromKey(sdl_keycode);
    return static_cast<scancode>(sdl_scancode);
}

inline auto name_of(scancode sc) noexcept -> const char * {
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(sc));
}

inline auto keyboard_focus() noexcept -> std::optional<window_ref> {
    if (const auto ptr = SDL_GetKeyboardFocus()) {
        return window_ref(ptr);
    } else {
        return std::nullopt;
    }
}

inline auto is_pressed(scancode sc) noexcept -> bool {
    return SDL_GetKeyboardState(nullptr)[static_cast<SDL_Scancode>(sc)];
}

inline auto mod_state() noexcept -> keymod {
    return static_cast<keymod>(SDL_GetModState());
}

inline void set_mod_state(keymod mod) noexcept {
    SDL_SetModState(static_cast<SDL_Keymod>(mod));
}

} // namespace sdlw
