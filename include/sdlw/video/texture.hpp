#pragma once

#include <sdlw/utility.hpp>
#include <sdlw/video/blend_mode.hpp>
#include <sdlw/video/pixels/pixel_format_type.hpp>
#include <sdlw/video/size.hpp>

namespace sdlw::video {

enum class texture_access : int {
    static_ = SDL_TEXTUREACCESS_STATIC,
    streaming = SDL_TEXTUREACCESS_STREAMING,
    target = SDL_TEXTUREACCESS_TARGET
};

class texture {
    using deleter = ::sdlw::detail::make_functor<SDL_DestroyTexture>;

    std::unique_ptr<SDL_Texture, deleter> _texture;

public:
    using blend_mode_type = blend_mode;
    using size_type = size;

    texture() noexcept = default;

    texture(SDL_Texture* pointer) noexcept
        : _texture(pointer)
    {
    }

    texture(
        const renderer& rend,
        pixels::pixel_format_type format,
        texture_access access,
        const size& sz);

    texture(const renderer& rend, const surface& surf);

    SDL_Texture*
    get_pointer() const noexcept
    {
        return _texture.get();
    }

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

    blend_mode_type
    blend_mode() const
    {
        auto mode = SDL_BlendMode();
        if (SDL_GetTextureBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<blend_mode_type>(mode);
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

    size_type
    size() const
    {
        const auto ptexture = get_pointer();
        auto width = int();
        auto height = int();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, null, &width, &height) == 0) {
            return size_type{width, height};
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
    set_blend_mode(blend_mode_type mode)
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

} // namespace sdlw::video
