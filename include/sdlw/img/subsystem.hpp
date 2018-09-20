#pragma once

#include <SDL2/SDL_image.h>

#include <sdlw/utility.hpp>

namespace sdlw::img {

enum class subsystem_flags : int {
    JPG = IMG_INIT_JPG,
    PNG = IMG_INIT_PNG,
    TIF = IMG_INIT_TIF,
    WEBP = IMG_INIT_WEBP
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(subsystem_flags);

struct subsystem {
    subsystem(subsystem_flags flags)
    {
        if (IMG_Init(static_cast<int>(flags)) < 0) {
            throw error();
        }
    }

    ~subsystem() noexcept
    {
        IMG_Quit();
    }
};

} // namespace sdlw::img
