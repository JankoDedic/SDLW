#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

static
int
num_video_drivers() noexcept
{
    return SDL_GetNumVideoDrivers();
}

static
const char*
video_driver_name(int driver_index) noexcept
{
    return SDL_GetVideoDriver(driver_index);
}

static
const char*
current_video_driver() noexcept
{
    return SDL_GetCurrentVideoDriver();
}

} // namespace sdlw
