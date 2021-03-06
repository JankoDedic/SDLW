#pragma once

#include <memory>

#include <SDL2/SDL_surface.h>

#include <sdlw/blend_mode.hpp>
#include <sdlw/rwops.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class surface {
public:
    surface(class surface_ref) = delete;
    explicit surface(SDL_Surface* s) noexcept : _surface{s} {}
    auto get_pointer() const noexcept -> SDL_Surface* { return _surface.get(); }

    surface(int width, int height, int depth, u32 rmask, u32 gmask, u32 bmask, u32 amask)
        : surface{SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask)}
    {
        if (!_surface) throw error{};
    }

    surface(void* pixels, int width, int height, int depth, int pitch, u32 rmask, u32 gmask, u32 bmask, u32 amask)
        : surface{SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rmask, gmask, bmask, amask)}
    {
        if (!_surface) throw error{};
    }

    surface(int width, int height, int depth, pixel_format_type format)
        : surface{SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, static_cast<u32>(format))}
    {
        if (!_surface) throw error{};
    }

    surface(void* pixels, int width, int height, int depth, int pitch, pixel_format_type format)
        : surface{SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, depth, pitch, static_cast<u32>(format))}
    {
        if (!_surface) throw error{};
    }

    auto format() const noexcept -> pixel_format_ref
    {
        return pixel_format_ref{_surface->format};
    }

    auto size() const noexcept -> sdl::size
    {
        return {_surface->w, _surface->h};
    }

    auto pitch() const noexcept -> int
    {
        return _surface->pitch;
    }

    void fill(const rect& r, u32 color)
    {
        if (SDL_FillRect(_surface.get(), &r, color) < 0) {
            throw error{};
        }
    }

    void fill(span<const rect> rects, u32 color)
    {
        if (SDL_FillRects(_surface.get(), rects.data(), static_cast<int>(rects.size()), color) < 0) {
            throw error{};
        }
    }

    auto convert(pixel_format_ref fmt) const -> surface;

    auto clip() const noexcept -> rect
    {
        auto r = rect{};
        SDL_GetClipRect(_surface.get(), &r);
        return r;
    }

    auto color_key() const -> u32
    {
        auto key = u32{};
        if (SDL_GetColorKey(_surface.get(), &key) < 0) {
            throw error{};
        } else {
            return key;
        }
    }

    auto alpha_mod() const -> u8
    {
        auto alpha = u8{};
        if (SDL_GetSurfaceAlphaMod(_surface.get(), &alpha) < 0) {
            throw error{};
        } else {
            return alpha;
        }
    }

    auto blend_mode() const -> sdl::blend_mode
    {
        auto bm = SDL_BlendMode{};
        if (SDL_GetSurfaceBlendMode(_surface.get(), &bm) < 0) {
            throw error{};
        } else {
            return static_cast<sdl::blend_mode>(bm);
        }
    }

    auto color_mod() const -> std::array<u8, 3>
    {
        auto r = u8{};
        auto g = u8{};
        auto b = u8{};
        if (SDL_GetSurfaceColorMod(_surface.get(), &r, &g, &b) < 0) {
            throw error{};
        } else {
            return {r, g, b};
        }
    }

    void lock()
    {
        if (SDL_LockSurface(_surface.get()) < 0) {
            throw error{};
        }
    }

    auto must_lock() const noexcept -> bool
    {
        return static_cast<bool>(SDL_MUSTLOCK(_surface.get()));
    }

    auto set_clip(const rect& r) -> bool
    {
        return static_cast<bool>(SDL_SetClipRect(_surface.get(), &r));
    }

    void set_color_key(bool flag, u32 key)
    {
        if (SDL_SetColorKey(_surface.get(), static_cast<int>(flag), key) < 0) {
            throw error{};
        }
    }

    void set_alpha_mod(u8 alpha)
    {
        if (SDL_SetSurfaceAlphaMod(_surface.get(), alpha) < 0) {
            throw error{};
        }
    }

    void set_blend_mode(sdl::blend_mode bm)
    {
        if (SDL_SetSurfaceBlendMode(_surface.get(), static_cast<SDL_BlendMode>(bm)) < 0) {
            throw error{};
        }
    }

    void set_color_mod(u8 r, u8 g, u8 b)
    {
        if (SDL_SetSurfaceColorMod(_surface.get(), r, g, b) < 0) {
            throw error{};
        }
    }

    void set_palette(palette_ref p)
    {
        if (SDL_SetSurfacePalette(_surface.get(), p.get_pointer()) < 0) {
            throw error{};
        }
    }

    void set_rle(bool flag)
    {
        if (SDL_SetSurfaceRLE(_surface.get(), static_cast<int>(flag)) < 0) {
            throw error{};
        }
    }

    void unlock() noexcept
    {
        SDL_UnlockSurface(_surface.get());
    }

protected:
    std::unique_ptr<SDL_Surface, detail::make_functor<SDL_FreeSurface>> _surface;
};

inline auto surface::convert(pixel_format_ref fmt) const -> surface
{
    if (const auto s = SDL_ConvertSurface(_surface.get(), fmt.get_pointer(), 0)) {
        return surface{s};
    } else {
        throw error{};
    }
}

inline auto operator==(const surface& lhs, const surface& rhs) noexcept -> bool
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const surface& lhs, const surface& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

class surface_ref : public surface {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_surface.get()); }
    explicit surface_ref(SDL_Surface* s) noexcept : surface{s} {}
    ~surface_ref() { _surface.release(); }
};

inline void blit_scaled(const surface& src, surface& dst)
{
    if (SDL_BlitScaled(src.get_pointer(), nullptr, dst.get_pointer(), nullptr) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, const rect& srcrect, surface& dst)
{
    if (SDL_BlitScaled(src.get_pointer(), &srcrect, dst.get_pointer(), nullptr) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, surface& dst, rect& dstrect)
{
    if (SDL_BlitScaled(src.get_pointer(), nullptr, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline void blit_scaled(const surface& src, const rect& srcrect, surface& dst, rect& dstrect)
{
    if (SDL_BlitScaled(src.get_pointer(), &srcrect, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline void blit(const surface& src, surface& dst, rect& dstrect)
{
    if (SDL_BlitSurface(src.get_pointer(), nullptr, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline void blit(const surface& src, const rect& srcrect, surface& dst, rect& dstrect)
{
    if (SDL_BlitSurface(src.get_pointer(), &srcrect, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline void convert_pixels(
    int width,
    int height,
    pixel_format_type src_format,
    const void* src,
    int src_pitch,
    pixel_format_type dst_format,
    void* dst,
    int dst_pitch)
{
    const auto srcfmt = static_cast<u32>(src_format);
    const auto dstfmt = static_cast<u32>(dst_format);
    if (SDL_ConvertPixels(width, height, srcfmt, src, src_pitch, dstfmt, dst, dst_pitch) < 0) {
        throw error{};
    }
}

inline auto load_bmp(const char* file) -> surface
{
    if (const auto ptr = SDL_LoadBMP(file)) {
        return surface{ptr};
    } else {
        throw error{};
    }
}

inline auto load_bmp(stream& s) -> surface
{
    if (const auto ptr = SDL_LoadBMP_RW(s.get_pointer(), 0)) {
        return surface{ptr};
    } else {
        throw error{};
    }
}

inline auto lower_blit(const surface& src, surface& dst, rect& dstrect)
{
    if (SDL_LowerBlit(src.get_pointer(), nullptr, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline auto lower_blit(const surface& src, rect& srcrect, surface& dst, rect& dstrect)
{
    if (SDL_LowerBlit(src.get_pointer(), &srcrect, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline auto lower_blit_scaled(const surface& src, rect& srcrect, surface& dst, rect& dstrect)
{
    if (SDL_LowerBlitScaled(src.get_pointer(), &srcrect, dst.get_pointer(), &dstrect) < 0) {
        throw error{};
    }
}

inline void save_bmp(const surface& s, const char* file)
{
    if (SDL_SaveBMP(s.get_pointer(), file) < 0) {
        throw error{};
    }
}

inline void save_bmp(const surface& surf, stream& s)
{
    if (SDL_SaveBMP_RW(surf.get_pointer(), s.get_pointer(), 0) < 0) {
        throw error{};
    }
}

} // namespace sdl
