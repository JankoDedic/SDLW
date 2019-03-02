#pragma once

#include <sdlw/video/window.hpp>
#include <sdlw/video/renderer.hpp>

namespace sdlw::video {

inline
const renderer&
window_ref::renderer() const
{
    static auto s = ::sdlw::detail::storage<sdlw::video::renderer>();
    if (const auto ptr = SDL_GetRenderer(get_pointer())) {
        return *new (&s) sdlw::video::renderer(ptr);
    } else {
        throw error();
    }
}

inline
renderer&
window_ref::renderer()
{
    static auto s = ::sdlw::detail::storage<sdlw::video::renderer>();
    if (const auto ptr = SDL_GetRenderer(get_pointer())) {
        return *new (&s) sdlw::video::renderer(ptr);
    } else {
        throw error();
    }
}

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

} // namespace sdlw::video
