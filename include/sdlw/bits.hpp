#pragma once

#include <SDL2/SDL_bits.h>

#include <sdlw/types.hpp>

namespace sdl {

inline auto most_significant_bit_index(u32 x) -> int
{
    return SDL_MostSignificantBitIndex32(x);
}

} // namespace sdl
