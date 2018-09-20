#pragma once

#include <sdlw/video/window.hpp>
#include <sdlw/events/keyboard/keycode.hpp>
#include <sdlw/events/keyboard/keymod.hpp>
#include <sdlw/events/keyboard/scancode.hpp>

namespace sdlw::events::keyboard {

inline
keycode
get_keycode(const char* name)
{
    if (const auto result = SDL_GetKeyFromName(name); result == SDLK_UNKNOWN) {
        throw error();
    } else {
        return static_cast<keycode>(result);
    }
}

inline
keycode
get_keycode(scancode sc) noexcept
{
    const auto sdl_scancode = static_cast<SDL_Scancode>(sc);
    const auto sdl_keycode = SDL_GetKeyFromScancode(sdl_scancode);
    return static_cast<keycode>(sdl_keycode);
}

inline
const char*
name_of(keycode kc) noexcept
{
    return SDL_GetKeyName(static_cast<SDL_Keycode>(kc));
}

inline
scancode
get_scancode(const char* name)
{
    const auto result = SDL_GetScancodeFromName(name);
    if (result == SDL_SCANCODE_UNKNOWN) {
        throw error();
    } else {
        return static_cast<scancode>(result);
    }
}

inline
scancode
get_scancode(keycode kc) noexcept
{
    const auto sdl_keycode = static_cast<SDL_Keycode>(kc);
    const auto sdl_scancode = SDL_GetScancodeFromKey(sdl_keycode);
    return static_cast<scancode>(sdl_scancode);
}

inline
const char*
name_of(scancode sc) noexcept
{
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(sc));
}

inline
video::window*
focus_owner() noexcept
{
    static auto s = detail::storage<video::window>();
    if (const auto pwindow = SDL_GetKeyboardFocus()) {
        return new (&s) video::window(pwindow);
    } else {
        return nullptr;
    }
}

inline
bool
is_pressed(scancode sc) noexcept
{
    return SDL_GetKeyboardState(nullptr)[static_cast<SDL_Scancode>(sc)];
}

inline
keymod
mod_state() noexcept
{
    return static_cast<keymod>(SDL_GetModState());
}

inline
void
set_mod_state(keymod mod) noexcept
{
    SDL_SetModState(static_cast<SDL_Keymod>(mod));
}

} // namespace sdlw::events::keyboard
