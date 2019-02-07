#pragma once

#include <SDL2/SDL.h>

namespace sdlw::events::game_controller {

enum class axis {
    invalid       = SDL_CONTROLLER_AXIS_INVALID,
    left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    right_y       = SDL_CONTROLLER_AXIS_RIGHTY,
    trigger_left  = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    max           = SDL_CONTROLLER_AXIS_MAX
};

} // namespace sdlw::events::game_controller
