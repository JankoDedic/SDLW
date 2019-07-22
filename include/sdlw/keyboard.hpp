#pragma once

#include <optional>

#include <SDL2/SDL_keyboard.h>

#include <sdlw/video.hpp>
#include <sdlw/keycode.hpp>
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

struct text_input {
    static auto is_active() noexcept -> bool {
        return SDL_IsTextInputActive();
    }

    static void start() noexcept {
        SDL_StartTextInput();
    }

    static void stop() noexcept {
        SDL_StopTextInput();
    }

    static void set_candidate_list_bounds(const rectangle &bounds) noexcept {
        SDL_SetTextInputRect(const_cast<SDL_Rect *>(&bounds));
    }
};

struct screen_keyboard {
    static auto is_supported() noexcept -> bool {
        return SDL_HasScreenKeyboardSupport();
    }

    static auto is_shown(const window &win) noexcept -> bool {
        return SDL_IsScreenKeyboardShown(win.get_pointer());
    }
};

} // namespace sdlw
