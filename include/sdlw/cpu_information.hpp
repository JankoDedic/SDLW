#pragma once

#include <SDL2/SDL.h>

namespace sdlw {

struct cpu_information {
    static
    int
    cache_line_size() noexcept
    {
        return SDL_GetCPUCacheLineSize();
    }

    static
    int
    cpu_count() noexcept
    {
        return SDL_GetCPUCount();
    }

    static
    int
    system_ram() noexcept
    {
        return SDL_GetSystemRAM();
    }

    static
    bool
    has_3dnow() noexcept
    {
        return SDL_Has3DNow();
    }

    static
    bool
    has_avx() noexcept
    {
        return SDL_HasAVX();
    }

    static
    bool
    has_avx2() noexcept
    {
        return SDL_HasAVX2();
    }

    static
    bool
    has_altivec() noexcept
    {
        return SDL_HasAltiVec();
    }

    static
    bool
    has_mmx() noexcept
    {
        return SDL_HasMMX();
    }

    static
    bool
    has_rdtsc() noexcept
    {
        return SDL_HasRDTSC();
    }

    static
    bool
    has_sse() noexcept
    {
        return SDL_HasSSE();
    }

    static
    bool
    has_sse2() noexcept
    {
        return SDL_HasSSE2();
    }

    static
    bool
    has_sse3() noexcept
    {
        return SDL_HasSSE3();
    }

    static
    bool
    has_sse41() noexcept
    {
        return SDL_HasSSE41();
    }

    static
    bool
    has_sse42() noexcept
    {
        return SDL_HasSSE42();
    }
};

} // namespace sdlw
