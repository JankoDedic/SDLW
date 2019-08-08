#pragma once

#include <stdexcept>

#include <SDL2/SDL_error.h>

namespace sdlw {

struct error : std::runtime_error {
    error() noexcept
        : std::runtime_error{SDL_GetError()}
    {}
};

} // namespace sdlw
