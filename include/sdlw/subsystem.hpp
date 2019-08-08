#pragma once

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>
#include <sdlw/utility.hpp>

namespace sdlw {

// clang-format off

enum class subsystem_flags : u32 {
    timer           = SDL_INIT_TIMER,
    audio           = SDL_INIT_AUDIO,
    video           = SDL_INIT_VIDEO,
    joystick        = SDL_INIT_JOYSTICK,
    haptic          = SDL_INIT_HAPTIC,
    game_controller = SDL_INIT_GAMECONTROLLER,
    events          = SDL_INIT_EVENTS,
    no_parachute    = SDL_INIT_NOPARACHUTE,
    everything      = SDL_INIT_EVERYTHING
};

// clang-format on

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(subsystem_flags);

class subsystem {
    u32 _flags;

public:
    subsystem(subsystem_flags flags)
        : _flags(static_cast<u32>(flags))
    {
        if (SDL_InitSubSystem(_flags) < 0) {
            throw error{};
        }
    }

    ~subsystem() noexcept
    {
        SDL_QuitSubSystem(_flags);
    }
};

} // namespace sdlw
