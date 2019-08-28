#pragma once

#include <chrono>

#include <SDL2/SDL_timer.h>

#include <sdlw/types.hpp>

namespace sdl {

struct clock {
    using duration = std::chrono::duration<u32, std::milli>;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<clock>;
    static constexpr auto is_steady = true;

    static auto now() noexcept -> time_point
    {
        return time_point{duration{SDL_GetTicks()}};
    }
};

inline void delay(clock::duration d) noexcept
{
    SDL_Delay(d.count());
}

struct high_resolution_clock {
    using duration = std::chrono::duration<u64, std::nano>;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<high_resolution_clock>;
    static constexpr auto is_steady = true;

    static auto now() noexcept -> time_point
    {
        static const auto frequency = SDL_GetPerformanceFrequency();
        const auto counter = SDL_GetPerformanceCounter();
        return time_point{duration{counter * std::nano::den / frequency}};
    }
};

} // namespace sdl
