#pragma once

#include <SDL2/SDL.h>

#include <sdlw/rectangle.hpp>

namespace sdlw {

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

} // namespace sdlw