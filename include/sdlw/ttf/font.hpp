#pragma once

#include <memory>

#include <SDL2/SDL_ttf.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>
#include <sdlw/utility.hpp>

namespace sdlw::ttf {

enum class style : int {
    normal = TTF_STYLE_NORMAL,
    bold = TTF_STYLE_BOLD,
    italic = TTF_STYLE_ITALIC,
    underline = TTF_STYLE_UNDERLINE,
    strikethrough = TTF_STYLE_STRIKETHROUGH
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(style);

enum class hinting : int {
    normal = TTF_HINTING_NORMAL,
    light = TTF_HINTING_LIGHT,
    mono = TTF_HINTING_MONO,
    none = TTF_HINTING_NONE
};

struct glyph_metrics {
    int minx;
    int maxx;
    int miny;
    int maxy;
    int advance;
};

class font {
    using deleter = detail::make_functor<TTF_CloseFont>;

    std::unique_ptr<TTF_Font, deleter> _font;

public:
    font(TTF_Font* pointer) noexcept
        : _font(pointer)
    {
    }

    TTF_Font*
    get_pointer() const noexcept
    {
        return _font.get();
    }

// ----------------------------------------------------------------------------
// Style

    // Style

    sdlw::ttf::style
    style() const noexcept
    {
        return static_cast<sdlw::ttf::style>(TTF_GetFontStyle(get_pointer()));
    }

    void
    set_style(sdlw::ttf::style style) noexcept
    {
        TTF_SetFontStyle(get_pointer(), static_cast<int>(style));
    }

    // Outline

    int
    outline() const noexcept
    {
        return TTF_GetFontOutline(get_pointer());
    }

    void
    set_outline(int outline) noexcept
    {
        TTF_SetFontOutline(get_pointer(), outline);
    }

// ----------------------------------------------------------------------------
// Settings

    // Hinting

    sdlw::ttf::hinting
    hinting() const noexcept
    {
        return static_cast<sdlw::ttf::hinting>(TTF_GetFontHinting(get_pointer()));
    }

    void
    set_hinting(sdlw::ttf::hinting hinting) noexcept
    {
        TTF_SetFontHinting(get_pointer(), static_cast<int>(hinting));
    }

    // Kerning

    bool
    is_kerning_allowed() const noexcept
    {
        return static_cast<bool>(TTF_GetFontKerning(get_pointer()));
    }

    void
    set_kerning(bool allowed) noexcept
    {
        TTF_SetFontKerning(get_pointer(), static_cast<int>(allowed));
    }

// ----------------------------------------------------------------------------
// Metrics

    int
    height() const noexcept
    {
        return TTF_FontHeight(get_pointer());
    }

    int
    ascent() const noexcept
    {
        return TTF_FontAscent(get_pointer());
    }

    int
    descent() const noexcept
    {
        return TTF_FontDescent(get_pointer());
    }

    int
    line_skip() const noexcept
    {
        return TTF_FontLineSkip(get_pointer());
    }

// ----------------------------------------------------------------------------
// Face attributes

    int
    number_of_faces() const noexcept
    {
        return TTF_FontFaces(get_pointer());
    }

    bool
    is_face_fixed_width() const noexcept
    {
        return static_cast<bool>(TTF_FontFaceIsFixedWidth(get_pointer()));
    }

    const char*
    face_family_name() const noexcept
    {
        return TTF_FontFaceFamilyName(get_pointer());
    }

    const char*
    face_style_name() const noexcept
    {
        return TTF_FontFaceStyleName(get_pointer());
    }

// ----------------------------------------------------------------------------
// Glyphs

    bool
    is_glyph_provided(u16 ch) const noexcept
    {
        return static_cast<bool>(TTF_GlyphIsProvided(get_pointer(), ch));
    }

    sdlw::ttf::glyph_metrics
    glyph_metrics(u16 ch) const
    {
        const auto pfont = get_pointer();
        auto metrics = sdlw::ttf::glyph_metrics();
        const auto minx = &metrics.minx;
        const auto maxx = &metrics.maxx;
        const auto miny = &metrics.miny;
        const auto maxy = &metrics.maxy;
        const auto adv = &metrics.advance;
        if (TTF_GlyphMetrics(pfont, ch, minx, maxx, miny, maxy, adv) == 0) {
            return metrics;
        } else {
            throw error();
        }
    }
};

} // namespace sdlw::ttf
