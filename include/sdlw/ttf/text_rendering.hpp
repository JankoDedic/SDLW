#pragma once

#include <SDL2/SDL_ttf.h>

#include <sdlw/video/color.hpp>
#include <sdlw/video/size.hpp>
#include <sdlw/video/surface.hpp>
#include <sdlw/ttf/font.hpp>

namespace sdlw::ttf {

inline
video::size
text_render_size(const font& f, const char* text)
{
    auto size = video::size();
    if (TTF_SizeText(f.get_pointer(), text, &size.w, &size.h) == 0) {
        return size;
    } else {
        throw error();
    }
}

inline
video::surface
solid_text_render(const font& f, const char* text, video::color fg_color)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Solid(pfont, text, fg_color)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
solid_utf8_render(const font& f, const char* text, video::color fg_color)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Solid(pfont, text, fg_color)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
shaded_text_render(
    const font& f,
    const char* text,
    video::color fg,
    video::color bg)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Shaded(pfont, text, fg, bg)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
shaded_utf8_render(
    const font& f,
    const char* text,
    video::color fg,
    video::color bg)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Shaded(pfont, text, fg, bg)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
blended_text_render(const font& f, const char* text, video::color fg_color)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Blended(pfont, text, fg_color)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
blended_utf8_render(const font& f, const char* text, video::color fg_color)
{
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Blended(pfont, text, fg_color)) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
blended_wrapped_text_render(
    const font& f,
    const char* txt,
    video::color fg,
    u32 wrap_length)
{
    const auto pfont = f.get_pointer();
    const auto wrap = wrap_length;
    const auto psurface = TTF_RenderText_Blended_Wrapped(pfont, txt, fg, wrap);
    if (psurface) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

inline
video::surface
blended_wrapped_utf8_render(
    const font& f,
    const char* txt,
    video::color fg,
    u32 wrap_length)
{
    const auto pfont = f.get_pointer();
    const auto wrap = wrap_length;
    const auto psurface = TTF_RenderUTF8_Blended_Wrapped(pfont, txt, fg, wrap);
    if (psurface) {
        return video::surface(psurface);
    } else {
        throw error();
    }
}

} // namespace sdlw::ttf
