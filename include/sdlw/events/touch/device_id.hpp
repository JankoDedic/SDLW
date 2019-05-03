#pragma once

#include <SDL2/SDL.h>

namespace sdlw::events::touch {

using device_id = i64;

constexpr auto mouse_id = device_id{SDL_TOUCH_MOUSEID};

} // namespace sdlw::events::touch
