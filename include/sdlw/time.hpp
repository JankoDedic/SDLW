#pragma once

#include <chrono>

#include <SDL2/SDL_timer.h>

#include <sdlw/types.hpp>

namespace sdlw {

struct clock {
    using duration   = std::chrono::duration<u32, std::milli>;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<clock>;
    static constexpr auto is_steady = true;

    static auto now() noexcept -> time_point {
        return time_point(duration(SDL_GetTicks()));
    }
};

inline void delay(clock::duration d) noexcept {
    SDL_Delay(d.count());
}

} // namespace sdlw
