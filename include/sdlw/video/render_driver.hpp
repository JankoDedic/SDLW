#pragma once

#include <SDL2/SDL.h>

#include <sdlw/video/renderer.hpp>

namespace sdlw::video {

struct render_driver {
    static
    int
    count()
    {
        if (const auto number = SDL_GetNumRenderDrivers(); number >= 0) {
            return number;
        } else {
            throw error();
        }
    }

    static
    renderer_info
    info(int driver_index)
    {
        auto info = SDL_RendererInfo();
        if (SDL_GetRenderDriverInfo(driver_index, &info) == 0) {
            return renderer_info(info);
        } else {
            throw error();
        }
    }
};

} // namespace sdlw::video
