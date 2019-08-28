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

using timer_id = SDL_TimerID;
using timer_callback = clock::duration(clock::duration);

inline auto add_timer(clock::duration interval, timer_callback* callback) -> timer_id
{
    constexpr auto sdl_timer_callback = [](u32 interval, void* param) -> u32 {
        const auto func = reinterpret_cast<timer_callback*>(param);
        return func(clock::duration{interval}).count();
    };
    const auto result = SDL_AddTimer(interval.count(), sdl_timer_callback, reinterpret_cast<void*>(callback));
    if (result == 0) throw error{};
    return result;
}

template<typename Callable>
auto add_timer(clock::duration inverval, Callable& cb) -> timer_id {
    constexpr auto sdl_timer_callback = [](u32 interval, void* param) -> u32 {
        auto& func = *reinterpret_cast<Callable*>(param);
        return func(clock::duration{interval}).count();
    };
    const auto result = SDL_AddTimer(interval.count(), sdl_timer_callback, &cb);
    if (result == 0) throw error{};
    return result;
}

inline auto remove_timer(timer_id id) noexcept -> bool
{
    return static_cast<bool>(SDL_RemoveTimer(id));
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
