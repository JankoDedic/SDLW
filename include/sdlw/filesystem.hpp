#pragma once

#include <memory>
#include <optional>

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/utility.hpp>

namespace sdlw::filesystem {

inline
std::unique_ptr<const char, detail::sdl_string_deleter>
base_path()
{
    if (const auto path = SDL_GetBasePath()) {
        return std::unique_ptr<const char, detail::sdl_string_deleter>(path);
    } else {
        throw error();
    }
}

inline
std::unique_ptr<const char, detail::sdl_string_deleter>
get_pref_path(const char* org, const char* app)
{
    if (const auto path = SDL_GetPrefPath(org, app)) {
        return std::unique_ptr<const char, detail::sdl_string_deleter>(path);
    } else {
        throw error();
    }
}

} // namespace sdlw::filesystem
