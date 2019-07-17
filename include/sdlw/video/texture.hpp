#pragma once

#include <sdlw/utility.hpp>
#include <sdlw/blend_mode.hpp>
#include <sdlw/video/pixels/pixel_format_type.hpp>
#include <sdlw/video/size.hpp>
#include <sdlw/video/renderer.hpp>

namespace sdlw::video {

enum class texture_access : int {
    static_   = SDL_TEXTUREACCESS_STATIC,
    streaming = SDL_TEXTUREACCESS_STREAMING,
    target    = SDL_TEXTUREACCESS_TARGET
};

class texture_ref {
protected:
    SDL_Texture* _ptexture = nullptr;

public:
    texture_ref() noexcept : _ptexture{nullptr} {}
    texture_ref(SDL_Texture* pointer) noexcept : _ptexture{pointer} {}
    auto get_pointer() const noexcept -> SDL_Texture* { return _ptexture; }

    texture_access
    access() const
    {
        const auto ptexture = get_pointer();
        auto access = int();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, &access, null, null) == 0) {
            return static_cast<texture_access>(access);
        } else {
            throw error();
        }
    }

    u8
    alpha_mod() const
    {
        auto alpha = u8();
        if (SDL_GetTextureAlphaMod(get_pointer(), &alpha) == 0) {
            return alpha;
        } else {
            throw error();
        }
    }

    sdlw::video::blend_mode
    blend_mode() const
    {
        auto mode = SDL_BlendMode();
        if (SDL_GetTextureBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<sdlw::video::blend_mode>(mode);
        } else {
            throw error();
        }
    }

    std::array<u8, 3>
    color_mod() const
    {
        auto r = u8();
        auto g = u8();
        auto b = u8();
        if (SDL_GetTextureColorMod(get_pointer(), &r, &g, &b) == 0) {
            return std::array{r, g, b};
        } else {
            throw error();
        }
    }

    pixels::pixel_format_type
    format() const
    {
        const auto ptexture = get_pointer();
        auto format = u32();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, &format, null, null, null) == 0) {
            return static_cast<pixels::pixel_format_type>(format);
        } else {
            throw error();
        }
    }

    sdlw::video::size
    size() const
    {
        const auto ptexture = get_pointer();
        auto width = int();
        auto height = int();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, null, &width, &height) == 0) {
            return sdlw::video::size{width, height};
        } else {
            throw error();
        }
    }

    void
    set_alpha_mod(u8 alpha)
    {
        if (SDL_SetTextureAlphaMod(get_pointer(), alpha) < 0) {
            throw error();
        }
    }

    void
    set_blend_mode(sdlw::video::blend_mode mode)
    {
        const auto sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetTextureBlendMode(get_pointer(), sdl_blend_mode) < 0) {
            throw error();
        }
    }

    void
    set_color_mod(u8 red, u8 green, u8 blue)
    {
        if (SDL_SetTextureColorMod(get_pointer(), red, green, blue) < 0) {
            throw error();
        }
    }

    std::tuple<void*, int>
    lock(const rectangle& area)
    {
        auto pixels = static_cast<void*>(nullptr);
        auto pitch = int();
        if (SDL_LockTexture(get_pointer(), &area, &pixels, &pitch) == 0) {
            return std::make_tuple(pixels, pitch);
        } else {
            throw error();
        }
    }

    void
    unlock() noexcept
    {
        SDL_UnlockTexture(get_pointer());
    }

    void
    update(const rectangle& rect, const void* pixels, int pitch)
    {
        if (SDL_UpdateTexture(get_pointer(), &rect, pixels, pitch) < 0) {
            throw error();
        }
    }

    void
    update_yuv(
        const rectangle& rect,
        const u8* yplane, int ypitch,
        const u8* uplane, int upitch,
        const u8* vplane, int vpitch)
    {
        const auto ptex = get_pointer();
        const auto y = yplane;
        const auto u = uplane;
        const auto v = vplane;
        constexpr auto update = SDL_UpdateYUVTexture;
        if (update(ptex, &rect, y, ypitch, u, upitch, v, vpitch) < 0) {
            throw error();
        }
    }
};

class texture : public texture_ref {
public:
    using texture_ref::texture_ref;

    texture(const texture&) = delete;
    auto operator=(const texture&) -> texture& = delete;

    texture(texture&& other) noexcept
        : texture_ref{std::exchange(other._ptexture, nullptr)}
    {
    }

    auto operator=(texture&& other) noexcept -> texture& {
        SDL_DestroyTexture(_ptexture);
        _ptexture = std::exchange(other._ptexture, nullptr);
        return *this;
    }

    ~texture() noexcept {
        SDL_DestroyTexture(_ptexture);
        _ptexture = nullptr;
    }

    texture(const renderer& rend, pixels::pixel_format_type format, texture_access access, const sdlw::video::size& sz)
    {
        const auto prend = rend.get_pointer();
        const auto format_ = static_cast<u32>(format);
        const auto access_ = static_cast<int>(access);
        const auto ptr = SDL_CreateTexture(prend, format_, access_, sz.w, sz.h);
        if (ptr) {
            _ptexture = ptr;
        } else {
            throw error();
        }
    }

    texture(const renderer& rend, const surface& surf)
    {
        const auto prend = rend.get_pointer();
        const auto psurf = surf.get_pointer();
        const auto ptr = SDL_CreateTextureFromSurface(prend, psurf);
        if (ptr) {
            _ptexture = ptr;
        } else {
            throw error();
        }
    }
};

inline
bool
operator==(const texture& lhs, const texture& rhs) noexcept
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline
bool
operator!=(const texture& lhs, const texture& rhs) noexcept
{
    return !(lhs == rhs);
}

auto renderer_ref::target() -> texture_ref {
    if (const auto ptr = SDL_GetRenderTarget(_prenderer)) {
        return {ptr};
    } else {
        throw error{};
    }
}

void renderer_ref::set_target(texture* t) {
    if (SDL_SetRenderTarget(_prenderer, t ? t->get_pointer() : nullptr) < 0) {
        throw error{};
    }
}

inline
void
renderer_ref::copy(
    const texture& tex,
    const rectangle* source,
    const rectangle* destination)
{
    const auto ptexture = tex.get_pointer();
    if (SDL_RenderCopy(get_pointer(), ptexture, source, destination) < 0) {
        throw error();
    }
}

inline
void
renderer_ref::copy(
    const texture& tex,
    const rectangle* source,
    const rectangle* destination,
    double angle,
    const point* center,
    renderer_flip flip)
{
    const auto ptr = get_pointer();
    const auto ptex = tex.get_pointer();
    const auto src = source;
    const auto dst = destination;
    const auto flip_ = static_cast<SDL_RendererFlip>(flip);
    if (SDL_RenderCopyEx(ptr, ptex, src, dst, angle, center, flip_) < 0) {
        throw error();
    }
}

} // namespace sdlw::video
