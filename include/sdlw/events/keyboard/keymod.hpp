#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>
#include <sdlw/utility.hpp>

namespace sdlw::events::keyboard {

enum class keymod : u16 {
    none     = KMOD_NONE,
    lshift   = KMOD_LSHIFT,
    rshift   = KMOD_RSHIFT,
    lctrl    = KMOD_LCTRL,
    rctrl    = KMOD_RCTRL,
    lalt     = KMOD_LALT,
    ralt     = KMOD_RALT,
    lgui     = KMOD_LGUI,
    rgui     = KMOD_RGUI,
    num      = KMOD_NUM,
    caps     = KMOD_CAPS,
    mode     = KMOD_MODE,
    reserved = KMOD_RESERVED
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(keymod);

} // namespace sdlw::events::keyboard
