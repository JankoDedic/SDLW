#pragma once

#include <chrono>

#include <SDL2/SDL.h>

namespace sdlw::time {

struct clock {
    using duration = std::chrono::duration<Uint32, std::milli>;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<clock>;
    static constexpr auto is_steady = true;

    static
    time_point
    now() noexcept
    {
        return time_point(duration(SDL_GetTicks()));
    }
};

template<typename Rep, typename Period>
inline
void
delay(const std::chrono::duration<Rep, Period>& duration_) noexcept
{
    using namespace std::chrono;
    SDL_Delay(duration_cast<clock::duration>(duration_).count());
}

} // namespace sdlw::time
