#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

using touch_id = i64;

constexpr auto mouse_id = touch_id{SDL_TOUCH_MOUSEID};

} // namespace sdlw
