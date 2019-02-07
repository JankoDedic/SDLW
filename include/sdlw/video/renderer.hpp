#pragma once

#include <SDL2/SDL.h>

#include <sdlw/utility.hpp>
#include <sdlw/video/color.hpp>
#include <sdlw/video/blend_mode.hpp>
#include <sdlw/video/surface.hpp>

namespace sdlw::video {

struct window;   // for renderer::renderer
class texture;  // for renderer::copy

enum class renderer_flags {
    software = SDL_RENDERER_SOFTWARE,
    accelerated = SDL_RENDERER_ACCELERATED,
    present_vsync = SDL_RENDERER_PRESENTVSYNC,
    target_texture = SDL_RENDERER_TARGETTEXTURE
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(renderer_flags);

enum class renderer_flip : u32 {
    none = SDL_FLIP_NONE,
    horizontal = SDL_FLIP_HORIZONTAL,
    vertical = SDL_FLIP_VERTICAL
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(renderer_flip);

class renderer_info {
    SDL_RendererInfo _renderer_info;

public:
    explicit
    operator SDL_RendererInfo() const noexcept
    {
        return _renderer_info;
    }

    renderer_info() noexcept = default;

    explicit
    renderer_info(const SDL_RendererInfo& info) noexcept
        : _renderer_info(info)
    {
    }

    const char*
    name() const noexcept
    {
        return _renderer_info.name;
    }

    renderer_flags
    flags() const noexcept
    {
        return static_cast<renderer_flags>(_renderer_info.flags);
    }

    int
    num_texture_formats() const noexcept
    {
        return _renderer_info.num_texture_formats;
    }

    pixels::pixel_format_type
    texture_format(int index) const noexcept
    {
        const auto tex_format = _renderer_info.texture_formats[index];
        return static_cast<pixels::pixel_format_type>(tex_format);
    }

    size
    max_texture_size() const noexcept
    {
        const auto width = _renderer_info.max_texture_width;
        const auto height = _renderer_info.max_texture_height;
        return size{width, height};
    }
};

class renderer {
    using deleter = ::sdlw::detail::make_functor<SDL_DestroyRenderer>;

    std::unique_ptr<SDL_Renderer, deleter> _renderer;

public:
    renderer() noexcept = default;

    renderer(SDL_Renderer* pointer) noexcept
        : _renderer(pointer)
    {
    }

    renderer(
        const window& win,
        renderer_flags flags,
        int rendering_driver_index = -1)
    {
        const auto pwin = win.get_pointer();
        const auto index = rendering_driver_index;
        const auto flags_ = static_cast<u32>(flags);
        if (const auto ptr = SDL_CreateRenderer(pwin, index, flags_)) {
            *this = renderer(ptr);
        } else {
            throw error();
        }
    }

    renderer(const surface& surf)
    {
        if (const auto ptr = SDL_CreateSoftwareRenderer(surf.get_pointer())) {
            *this = renderer(ptr);
        } else {
            throw error();
        }
    }

    SDL_Renderer*
    get_pointer() const noexcept
    {
        return _renderer.get();
    }

    blend_mode
    draw_blend_mode() const
    {
        auto mode = SDL_BlendMode();
        if (SDL_GetRenderDrawBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<blend_mode>(mode);
        } else {
            throw error();
        }
    }

    void
    set_draw_blend_mode(blend_mode mode)
    {
        const auto sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetRenderDrawBlendMode(get_pointer(), sdl_blend_mode) < 0) {
            throw error();
        }
    }

    color
    draw_color() const
    {
        auto r = u8();
        auto g = u8();
        auto b = u8();
        auto a = u8();
        if (SDL_GetRenderDrawColor(get_pointer(), &r, &g, &b, &a) == 0) {
            return color{r, g, b, a};
        } else {
            throw error();
        }
    }

    void
    set_draw_color(color c)
    {
        if (SDL_SetRenderDrawColor(get_pointer(), c.r, c.g, c.b, c.a) < 0) {
            throw error();
        }
    }

    bool
    are_targets_supported() const noexcept
    {
        return SDL_RenderTargetSupported(get_pointer());
    }

    const texture&
    target() const;

    texture&
    target();

    void
    set_target(texture*);

    size
    output_size() const
    {
        auto sz = size();
        if (SDL_GetRendererOutputSize(get_pointer(), &sz.w, &sz.h) == 0) {
            return sz;
        } else {
            throw error();
        }
    }

    bool
    is_clip_enabled() const noexcept
    {
        return SDL_RenderIsClipEnabled(get_pointer());
    }

    rectangle
    clip() const noexcept
    {
        auto rect = rectangle();
        SDL_RenderGetClipRect(get_pointer(), &rect);
        return rect;
    }

    void
    set_clip(const rectangle& rect)
    {
        if (SDL_RenderSetClipRect(get_pointer(), &rect) < 0) {
            throw error();
        }
    }

    rectangle
    viewport() const noexcept
    {
        auto rect = rectangle();
        SDL_RenderGetViewport(get_pointer(), &rect);
        return rect;
    }

    void
    set_viewport(const rectangle& viewport)
    {
        if (SDL_RenderSetViewport(get_pointer(), &viewport) < 0) {
            throw error();
        }
    }

    std::pair<float, float>
    scale() const noexcept
    {
        auto xscale = float();
        auto yscale = float();
        SDL_RenderGetScale(get_pointer(), &xscale, &yscale);
        return std::make_pair(xscale, xscale);
    }

    void
    set_scale(float xscale, float yscale)
    {
        if (SDL_RenderSetScale(get_pointer(), xscale, yscale) < 0) {
            throw error();
        }
    }

    bool
    integer_scale() const noexcept
    {
        return SDL_RenderGetIntegerScale(get_pointer());
    }

    void
    set_integer_scale(bool should_enable)
    {
        const auto b = static_cast<SDL_bool>(should_enable);
        if (SDL_RenderSetIntegerScale(get_pointer(), b) < 0) {
            throw error();
        }
    }

    size
    logical_size() const noexcept
    {
        auto sz = size();
        SDL_RenderGetLogicalSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void
    set_logical_size(const size& sz)
    {
        if (SDL_RenderSetLogicalSize(get_pointer(), sz.w, sz.h) < 0) {
            throw error();
        }
    }

    renderer_info
    info() const
    {
        auto info = SDL_RendererInfo();
        if (SDL_GetRendererInfo(get_pointer(), &info) < 0) {
            return renderer_info(info);
        } else {
            throw error();
        }
    }

    void
    read_pixels(
        void* pixels,
        const rectangle& rect,
        pixels::pixel_format_type format,
        int pitch) const
    {
        const auto prend = get_pointer();
        const auto fmt = static_cast<u32>(format);
        if (SDL_RenderReadPixels(prend, &rect, fmt, pixels, pitch) < 0) {
            throw error();
        }
    }

    void
    clear()
    {
        if (SDL_RenderClear(get_pointer()) < 0) {
            throw error();
        }
    }

    void
    draw_line(const point& p1, const point& p2)
    {
        if (SDL_RenderDrawLine(get_pointer(), p1.x, p1.y, p2.x, p2.y) < 0) {
            throw error();
        }
    }

    void
    draw_line_strip(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        const auto sz = static_cast<int>(points.size());
        if (SDL_RenderDrawLines(get_pointer(), points.data(), sz) < 0) {
            throw error();
        }
    }

    void
    draw_point(const point& p)
    {
        if (SDL_RenderDrawPoint(get_pointer(), p.x, p.y) < 0) {
            throw error();
        }
    }

    void
    draw_points(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        const auto sz = static_cast<int>(points.size());
        if (SDL_RenderDrawPoints(get_pointer(), points.data(), sz) < 0) {
            throw error();
        }
    }

    void
    draw_rectangle(const rectangle& rect)
    {
        if (SDL_RenderDrawRect(get_pointer(), &rect) < 0) {
            throw error();
        }
    }

    void
    draw_rectangles(span<const rectangle> rectangles)
    {
        if (!rectangles.data()) {
            return;
        }
        const auto sz = static_cast<int>(rectangles.size());
        if (SDL_RenderDrawRects(get_pointer(), rectangles.data(), sz) == 0) {
            throw error();
        }
    }

    void
    fill_rectangle(const rectangle& rect)
    {
        if (SDL_RenderFillRect(get_pointer(), &rect) < 0) {
            throw error();
        }
    }

    void
    fill_rectangles(span<const rectangle> rectangles)
    {
        if (!rectangles.data()) {
            return;
        }
        const auto sz = static_cast<int>(rectangles.size());
        if (SDL_RenderFillRects(get_pointer(), rectangles.data(), sz) < 0) {
            throw error();
        }
    }

    void
    copy(
        const texture&,
        const rectangle* source,
        const rectangle* destination);

    void
    copy(
        const texture&,
        const rectangle* source,
        const rectangle* destination,
        double angle,
        const point* center,
        renderer_flip);

    void
    present() noexcept
    {
        SDL_RenderPresent(get_pointer());
    }
};

inline
bool
operator==(const renderer& lhs, const renderer& rhs) noexcept
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline
bool
operator!=(const renderer& lhs, const renderer& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace sdlw::video
