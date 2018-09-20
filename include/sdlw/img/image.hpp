#pragma once

#include <SDL2/SDL_image.h>

#include <sdlw/video/renderer.hpp>
#include <sdlw/video/surface.hpp>
#include <sdlw/video/texture.hpp>

namespace sdlw::img {

inline
video::surface
load_as_surface(const char* filename)
{
    if (const auto psurface = IMG_Load(filename)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::texture
load_as_texture(const video::renderer& rend, const char* filename)
{
    const auto ptexture = IMG_LoadTexture(rend.get_pointer(), filename);
    if (ptexture) {
        return video::texture(ptexture);
    } else {
        throw error();
    }
}

inline
void
save_as_png(const video::surface& surf, const char* filename)
{
    if (IMG_SavePNG(surf.get_pointer(), filename) < 0) {
        throw error();
    }
}

inline
void
save_as_jpg(const video::surface& surf, const char* filename, int quality)
{
    if (IMG_SaveJPG(surf.get_pointer(), filename, quality) < 0) {
        throw error();
    }
}

} // namespace sdlw::img
