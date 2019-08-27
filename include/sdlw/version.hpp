#pragma once

#include <SDL2/SDL_version.h>

namespace sdl {

using version = SDL_version;

inline auto get_revision() noexcept -> const char*
{
    return SDL_GetRevision();
}

inline auto get_revision_number() noexcept -> int
{
    return SDL_GetRevisionNumber();
}

inline auto get_version() noexcept -> version
{
    auto v = version{};
    SDL_GetVersion(&v);
    return v;
}

} // namespace sdl
