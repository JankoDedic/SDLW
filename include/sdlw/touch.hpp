#pragma once

#include <optional>

#include <SDL2/SDL_touch.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>

namespace sdl {

using finger_id = i64;

class finger {
    SDL_Finger _finger;

public:
    explicit operator SDL_Finger() const noexcept
    {
        return _finger;
    }

    explicit finger(const SDL_Finger& finger) noexcept
        : _finger(finger)
    {}

    finger_id id() const noexcept
    {
        return static_cast<finger_id>(_finger.id);
    }

    float x() const noexcept
    {
        return _finger.x;
    }

    float y() const noexcept
    {
        return _finger.y;
    }
};

using touch_id = i64;

constexpr auto mouse_id = touch_id{SDL_TOUCH_MOUSEID};

inline int num_touch_devices() noexcept
{
    return SDL_GetNumTouchDevices();
}

inline touch_id get_touch_device(int device_index)
{
    if (const auto result = SDL_GetTouchDevice(device_index); result == 0) {
        throw error{};
    } else {
        return touch_id(result);
    }
}

inline int num_touch_fingers(touch_id id)
{
    const auto result = SDL_GetNumTouchFingers(static_cast<SDL_TouchID>(id));
    if (result == 0) {
        throw error{};
    } else {
        return result;
    }
}

inline finger get_touch_finger(touch_id id, int finger_index)
{
    if (const auto fing = SDL_GetTouchFinger(SDL_TouchID(id), finger_index)) {
        return finger(*fing);
    } else {
        throw error{};
    }
}

} // namespace sdl
