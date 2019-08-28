#pragma once

#include <memory>

#include <SDL2/SDL_filesystem.h>

#include <sdlw/error.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

inline auto base_path() -> std::unique_ptr<const char[], detail::sdl_string_deleter>
{
    if (const auto path = SDL_GetBasePath()) {
        return std::unique_ptr<const char[], detail::sdl_string_deleter>(path);
    } else {
        throw error{};
    }
}

inline auto get_pref_path(const char* org, const char* app) -> std::unique_ptr<const char[], detail::sdl_string_deleter>
{
    if (const auto path = SDL_GetPrefPath(org, app)) {
        return std::unique_ptr<const char[], detail::sdl_string_deleter>(path);
    } else {
        throw error{};
    }
}

} // namespace sdl
