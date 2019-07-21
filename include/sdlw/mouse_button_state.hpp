#pragma once

#include <sdlw/mouse_button.hpp>
#include <sdlw/types.hpp>
#include <sdlw/utility.hpp>

namespace sdlw {

enum class mouse_button_state : u32 {
    left   = SDL_BUTTON_LMASK,
    middle = SDL_BUTTON_MMASK,
    right  = SDL_BUTTON_RMASK,
    x1     = SDL_BUTTON_X1MASK,
    x2     = SDL_BUTTON_X2MASK
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(mouse_button_state);

constexpr
mouse_button_state
button_mask(mouse_button b) noexcept
{
    return static_cast<mouse_button_state>(SDL_BUTTON(static_cast<u8>(b)));
}

} // namespace sdlw
