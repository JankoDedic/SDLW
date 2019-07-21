#pragma once

#include <optional>

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/events/touch/touch_device_id.hpp>
#include <sdlw/events/touch/touch_finger.hpp>

namespace sdlw {

inline
int
num_touch_devices() noexcept
{
    return SDL_GetNumTouchDevices();
}

inline
device_id
get_touch_device(int device_index)
{
    if (const auto result = SDL_GetTouchDevice(device_index); result == 0) {
        throw error();
    } else {
        return device_id(result);
    }
}

inline
int
num_touch_fingers(device_id id)
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
get_touch_finger(device_id id, int finger_index)
{
    if (const auto fing = SDL_GetTouchFinger(SDL_TouchID(id), finger_index)) {
        return finger(*fing);
    } else {
        throw error();
    }
}

} // namespace sdlw
