#pragma once

#include <SDL2/SDL.h>

namespace sdlw::events::game_controller {

enum class button {
    invalid = SDL_CONTROLLER_BUTTON_INVALID,
    a = SDL_CONTROLLER_BUTTON_A,
    b = SDL_CONTROLLER_BUTTON_B,
    x = SDL_CONTROLLER_BUTTON_X,
    y = SDL_CONTROLLER_BUTTON_Y,
    back = SDL_CONTROLLER_BUTTON_BACK,
    guide = SDL_CONTROLLER_BUTTON_GUIDE,
    start = SDL_CONTROLLER_BUTTON_START,
    left_stick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
    right_stick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    left_shoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    dpad_up = SDL_CONTROLLER_BUTTON_DPAD_UP,
    dpad_down = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    dpad_left = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    dpad_right = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    max = SDL_CONTROLLER_BUTTON_MAX
};

} // namespace sdlw::events::game_controller
