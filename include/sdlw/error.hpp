#pragma once

#include <stdexcept>

#include <SDL2/SDL_error.h>

namespace sdl {

struct error : std::runtime_error {
    error() noexcept
        : std::runtime_error{SDL_GetError()}
    {}
};

inline void clear_error() noexcept
{
    SDL_ClearError();
}

inline auto get_error() noexcept -> const char*
{
    return SDL_GetError();
}

template<typename... Args>
void set_error(const char* fmt, Args... args) noexcept
{
    SDL_SetError(fmt, args...);
}

} // namespace sdl
