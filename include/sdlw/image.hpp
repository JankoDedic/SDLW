#pragma once

#include <SDL2/SDL_image.h>

#include <sdlw/video/renderer.hpp>
#include <sdlw/video/surface.hpp>
#include <sdlw/video/texture.hpp>
#include <sdlw/utility.hpp>

namespace sdlw::img {

enum class subsystem_flags : int {
    jpg  = IMG_INIT_JPG,
    png  = IMG_INIT_PNG,
    tif  = IMG_INIT_TIF,
    webp = IMG_INIT_WEBP
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(subsystem_flags);

struct subsystem {
    subsystem(subsystem_flags flags) {
        if (IMG_Init(static_cast<int>(flags)) < 0) {
            throw error{};
        }
    }

    ~subsystem() noexcept {
        IMG_Quit();
    }
};

inline auto load_as_surface(const char* filename) -> video::surface {
    if (const auto psurface = IMG_Load(filename)) {
        return {psurface};
    } else {
        throw error{};
    }
}

inline auto load_as_texture(const video::renderer& rend, const char* filename) -> video::texture {
    const auto ptexture = IMG_LoadTexture(rend.get_pointer(), filename);
    if (ptexture) {
        return video::texture{ptexture};
    } else {
        throw error{};
    }
}

inline void save_as_png(const video::surface& surf, const char* filename) {
    if (IMG_SavePNG(surf.get_pointer(), filename) < 0) {
        throw error{};
    }
}

inline void save_as_jpg(const video::surface& surf, const char* filename, int quality) {
    if (IMG_SaveJPG(surf.get_pointer(), filename, quality) < 0) {
        throw error{};
    }
}

} // namespace sdlw::img

