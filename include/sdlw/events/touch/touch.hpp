#pragma once

#include <optional>

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/events/touch/device_id.hpp>
#include <sdlw/events/touch/finger.hpp>

namespace sdlw::events::touch {

inline
int
num_devices() noexcept
{
    return SDL_GetNumTouchDevices();
}

inline
device_id
get_device_id(int device_index)
{
    if (const auto result = SDL_GetTouchDevice(device_index); result == 0) {
        throw error();
    } else {
        return device_id(result);
    }
}

inline
int
num_fingers(device_id id)
{
    const auto result = SDL_GetNumTouchFingers(static_cast<SDL_TouchID>(id));
    if (result == 0) {
        throw error();
    } else {
        return result;
    }
}

inline
finger
get_finger(device_id id, int finger_index)
{
    if (const auto fing = SDL_GetTouchFinger(SDL_TouchID(id), finger_index)) {
        return finger(*fing);
    } else {
        throw error();
    }
}

} // namespace sdlw::events::touch
