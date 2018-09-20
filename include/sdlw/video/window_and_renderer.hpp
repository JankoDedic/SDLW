#pragma once

#include <sdlw/video/window.hpp>
#include <sdlw/video/renderer.hpp>

namespace sdlw::video {

inline
const renderer&
window::renderer() const
{
    static auto s = ::sdlw::detail::storage<renderer_type>();
    if (const auto ptr = SDL_GetRenderer(get_pointer())) {
        return *new (&s) renderer_type(ptr);
    } else {
        throw error();
    }
}

inline
renderer&
window::renderer()
{
    static auto s = ::sdlw::detail::storage<renderer_type>();
    if (const auto ptr = SDL_GetRenderer(get_pointer())) {
        return *new (&s) renderer_type(ptr);
    } else {
        throw error();
    }
}

inline
std::pair<window, renderer>
make_window_and_renderer(const size& window_size, window_flags flags)
{
    const auto [w, h] = window_size;
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
