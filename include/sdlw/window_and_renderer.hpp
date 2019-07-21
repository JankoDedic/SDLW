#pragma once

#include <sdlw/window.hpp>
#include <sdlw/renderer.hpp>

namespace sdlw {

inline
std::pair<window, renderer>
make_window_and_renderer(const size& window_size, window_flags flags)
{
    /* const auto [w, h] = window_size; */
    const auto w = window_size.w;
    const auto h = window_size.h;
    auto pwindow = static_cast<SDL_Window*>(nullptr);
    auto prenderer = static_cast<SDL_Renderer*>(nullptr);
    const auto flags_ = static_cast<u32>(flags);
    if (SDL_CreateWindowAndRenderer(w, h, flags_, &pwindow, &prenderer) == 0) {
        return std::make_pair(window(pwindow), renderer(prenderer));
    } else {
        throw error();
    }
}

} // namespace sdlw
