#pragma once

#include <SDL2/SDL_ttf.h>

#include <sdlw/ttf/font.hpp>

namespace sdlw::ttf {

inline
font
open_font(const char* filename, int ptsize)
{
    if (const auto pfont = TTF_OpenFont(filename, ptsize)) {
        return font(pfont);
    } else {
        throw error();
    }
}

inline
font
open_font(const char* filename, int ptsize, long index)
{
    if (const auto pfont = TTF_OpenFontIndex(filename, ptsize, index)) {
        return font(pfont);
    } else {
        throw error();
    }
}

} // namespace sdlw::ttf
