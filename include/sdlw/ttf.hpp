#pragma once

#include <memory>

#include <SDL2/SDL_ttf.h>

#include <sdlw/error.hpp>
#include <sdlw/types.hpp>
#include <sdlw/utility.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rectangle.hpp>
#include <sdlw/surface.hpp>

namespace sdlw::ttf {

struct subsystem {
    subsystem() {
        if (TTF_Init() < 0) {
            throw error{};
        }
    }

    ~subsystem() noexcept {
        TTF_Quit();
    }
};

enum class style : int {
    normal        = TTF_STYLE_NORMAL,
    bold          = TTF_STYLE_BOLD,
    italic        = TTF_STYLE_ITALIC,
    underline     = TTF_STYLE_UNDERLINE,
    strikethrough = TTF_STYLE_STRIKETHROUGH
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(style);

enum class hinting : int {
    normal = TTF_HINTING_NORMAL,
    light  = TTF_HINTING_LIGHT,
    mono   = TTF_HINTING_MONO,
    none   = TTF_HINTING_NONE
};

struct glyph_metrics {
    int minx;
    int maxx;
    int miny;
    int maxy;
    int advance;
};

class font {
    std::unique_ptr<TTF_Font, detail::make_functor<TTF_CloseFont>> _font;

public:
    font(TTF_Font* pointer) noexcept
        : _font{pointer}
    {
    }

    auto get_pointer() const noexcept -> TTF_Font* {
        return _font.get();
    }

// ----------------------------------------------------------------------------
// Style

    // Style

    auto style() const noexcept -> sdlw::ttf::style {
        return static_cast<sdlw::ttf::style>(TTF_GetFontStyle(get_pointer()));
    }

    void set_style(sdlw::ttf::style style) noexcept {
        TTF_SetFontStyle(get_pointer(), static_cast<int>(style));
    }

    // Outline

    auto outline() const noexcept -> int {
        return TTF_GetFontOutline(get_pointer());
    }

    void set_outline(int outline) noexcept {
        TTF_SetFontOutline(get_pointer(), outline);
    }

// ----------------------------------------------------------------------------
// Settings

    // Hinting

    auto hinting() const noexcept -> sdlw::ttf::hinting {
        return static_cast<sdlw::ttf::hinting>(TTF_GetFontHinting(get_pointer()));
    }

    void set_hinting(sdlw::ttf::hinting hinting) noexcept {
        TTF_SetFontHinting(get_pointer(), static_cast<int>(hinting));
    }

    // Kerning

    auto is_kerning_allowed() const noexcept -> bool {
        return static_cast<bool>(TTF_GetFontKerning(get_pointer()));
    }

    void set_kerning(bool allowed) noexcept {
        TTF_SetFontKerning(get_pointer(), static_cast<int>(allowed));
    }

// ----------------------------------------------------------------------------
// Metrics

    auto height()    const noexcept -> int { return TTF_FontHeight(get_pointer());   }
    auto ascent()    const noexcept -> int { return TTF_FontAscent(get_pointer());   }
    auto descent()   const noexcept -> int { return TTF_FontDescent(get_pointer());  }
    auto line_skip() const noexcept -> int { return TTF_FontLineSkip(get_pointer()); }

// ----------------------------------------------------------------------------
// Face attributes

    auto number_of_faces() const noexcept -> int {
        return TTF_FontFaces(get_pointer());
    }

    auto is_face_fixed_width() const noexcept -> bool {
        return static_cast<bool>(TTF_FontFaceIsFixedWidth(get_pointer()));
    }

    auto face_family_name() const noexcept -> const char* {
        return TTF_FontFaceFamilyName(get_pointer());
    }

    auto face_style_name() const noexcept -> const char* {
        return TTF_FontFaceStyleName(get_pointer());
    }

// ----------------------------------------------------------------------------
// Glyphs

    auto is_glyph_provided(u16 ch) const noexcept -> bool {
        return static_cast<bool>(TTF_GlyphIsProvided(get_pointer(), ch));
    }

    auto glyph_metrics(u16 ch) const -> sdlw::ttf::glyph_metrics {
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

inline auto open_font(const char* filename, int ptsize) -> font {
    if (const auto pfont = TTF_OpenFont(filename, ptsize)) {
        return font(pfont);
    } else {
        throw error();
    }
}

inline auto open_font(const char* filename, int ptsize, long index) -> font {
    if (const auto pfont = TTF_OpenFontIndex(filename, ptsize, index)) {
        return font(pfont);
    } else {
        throw error();
    }
}

inline auto text_render_size(const font& f, const char* text) -> size {
    auto size = sdlw::size();
    if (TTF_SizeText(f.get_pointer(), text, &size.w, &size.h) == 0) {
        return size;
    } else {
        throw error();
    }
}

inline auto solid_text_render(const font& f, const char* text, color fg_color) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Solid(pfont, text, fg_color)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto solid_utf8_render(const font& f, const char* text, color fg_color) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Solid(pfont, text, fg_color)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto shaded_text_render(const font& f, const char* text, color fg, color bg) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Shaded(pfont, text, fg, bg)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto shaded_utf8_render(const font& f, const char* text, color fg, color bg) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Shaded(pfont, text, fg, bg)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto blended_text_render(const font& f, const char* text, color fg_color) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderText_Blended(pfont, text, fg_color)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto blended_utf8_render(const font& f, const char* text, color fg_color) -> surface {
    const auto pfont = f.get_pointer();
    if (const auto psurface = TTF_RenderUTF8_Blended(pfont, text, fg_color)) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto blended_wrapped_text_render(const font& f, const char* txt, color fg, u32 wrap_length) -> surface {
    const auto pfont = f.get_pointer();
    const auto wrap = wrap_length;
    const auto psurface = TTF_RenderText_Blended_Wrapped(pfont, txt, fg, wrap);
    if (psurface) {
        return surface(psurface);
    } else {
        throw error();
    }
}

inline auto blended_wrapped_utf8_render(const font& f, const char* txt, color fg, u32 wrap_length) -> surface {
    const auto pfont = f.get_pointer();
    const auto wrap = wrap_length;
    const auto psurface = TTF_RenderUTF8_Blended_Wrapped(pfont, txt, fg, wrap);
    if (psurface) {
        return surface(psurface);
    } else {
        throw error();
    }
}

} // namespace sdlw::ttf
