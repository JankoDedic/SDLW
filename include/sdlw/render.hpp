#pragma once

#include <SDL2/SDL_render.h>

#include <sdlw/blend_mode.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/surface.hpp>
#include <sdlw/video.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class texture;
class texture_ref;

// clang-format off

enum class renderer_flags {
    software       = SDL_RENDERER_SOFTWARE,
    accelerated    = SDL_RENDERER_ACCELERATED,
    present_vsync  = SDL_RENDERER_PRESENTVSYNC,
    target_texture = SDL_RENDERER_TARGETTEXTURE
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(renderer_flags);

enum class renderer_flip : u32 {
    none       = SDL_FLIP_NONE,
    horizontal = SDL_FLIP_HORIZONTAL,
    vertical   = SDL_FLIP_VERTICAL
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(renderer_flip);

// clang-format on

class renderer_info {
    SDL_RendererInfo _renderer_info;

public:
    explicit operator SDL_RendererInfo() const noexcept
    {
        return _renderer_info;
    }

    renderer_info() noexcept = default;

    explicit renderer_info(const SDL_RendererInfo& info) noexcept
        : _renderer_info{info}
    {}

    auto name() const noexcept -> const char*
    {
        return _renderer_info.name;
    }

    auto flags() const noexcept -> renderer_flags
    {
        return static_cast<renderer_flags>(_renderer_info.flags);
    }

    auto num_texture_formats() const noexcept -> int
    {
        return _renderer_info.num_texture_formats;
    }

    auto texture_format(int index) const noexcept -> pixel_format_type
    {
        const auto tex_format = _renderer_info.texture_formats[index];
        return static_cast<pixel_format_type>(tex_format);
    }

    auto max_texture_size() const noexcept -> size
    {
        const auto width = _renderer_info.max_texture_width;
        const auto height = _renderer_info.max_texture_height;
        return {width, height};
    }
};

class renderer_ref {
protected:
    SDL_Renderer* _prenderer = nullptr;

public:
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_prenderer);
    }

    renderer_ref() noexcept
        : _prenderer{nullptr}
    {}

    explicit renderer_ref(SDL_Renderer* pointer) noexcept
        : _prenderer{pointer}
    {}

    auto get_pointer() const noexcept -> SDL_Renderer*
    {
        return _prenderer;
    }

    auto draw_blend_mode() const -> blend_mode
    {
        auto mode = SDL_BlendMode{};
        if (SDL_GetRenderDrawBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<blend_mode>(mode);
        } else {
            throw error{};
        }
    }

    void set_draw_blend_mode(blend_mode mode)
    {
        const auto sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetRenderDrawBlendMode(get_pointer(), sdl_blend_mode) < 0) {
            throw error{};
        }
    }

    auto draw_color() const -> color
    {
        auto c = color{};
        if (SDL_GetRenderDrawColor(get_pointer(), &c.r, &c.g, &c.b, &c.a) == 0) {
            return c;
        } else {
            throw error{};
        }
    }

    void set_draw_color(color c)
    {
        if (SDL_SetRenderDrawColor(get_pointer(), c.r, c.g, c.b, c.a) < 0) {
            throw error{};
        }
    }

    auto are_targets_supported() const noexcept -> bool
    {
        return SDL_RenderTargetSupported(get_pointer());
    }

    auto target() -> texture_ref;

    void set_target(texture*);

    auto output_size() const -> size
    {
        auto sz = size{};
        if (SDL_GetRendererOutputSize(get_pointer(), &sz.w, &sz.h) == 0) {
            return sz;
        } else {
            throw error{};
        }
    }

    auto is_clip_enabled() const noexcept -> bool
    {
        return SDL_RenderIsClipEnabled(get_pointer());
    }

    auto clip() const noexcept -> rect
    {
        auto r = rect{};
        SDL_RenderGetClipRect(get_pointer(), &r);
        return r;
    }

    void set_clip(const rect& rect)
    {
        if (SDL_RenderSetClipRect(get_pointer(), &rect) < 0) {
            throw error{};
        }
    }

    auto viewport() const noexcept -> rect
    {
        auto r = rect{};
        SDL_RenderGetViewport(get_pointer(), &r);
        return r;
    }

    void set_viewport(const rect& viewport)
    {
        if (SDL_RenderSetViewport(get_pointer(), &viewport) < 0) {
            throw error{};
        }
    }

    auto scale() const noexcept -> std::pair<float, float>
    {
        auto xscale = float{};
        auto yscale = float{};
        SDL_RenderGetScale(get_pointer(), &xscale, &yscale);
        return std::make_pair(xscale, xscale);
    }

    void set_scale(float xscale, float yscale)
    {
        if (SDL_RenderSetScale(get_pointer(), xscale, yscale) < 0) {
            throw error{};
        }
    }

    auto integer_scale() const noexcept -> bool
    {
        return SDL_RenderGetIntegerScale(get_pointer());
    }

    void set_integer_scale(bool should_enable)
    {
        const auto b = static_cast<SDL_bool>(should_enable);
        if (SDL_RenderSetIntegerScale(get_pointer(), b) < 0) {
            throw error{};
        }
    }

    auto logical_size() const noexcept -> size
    {
        auto sz = size{};
        SDL_RenderGetLogicalSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_logical_size(const size& sz)
    {
        if (SDL_RenderSetLogicalSize(get_pointer(), sz.w, sz.h) < 0) {
            throw error{};
        }
    }

    auto info() const -> renderer_info
    {
        auto info = SDL_RendererInfo{};
        if (SDL_GetRendererInfo(get_pointer(), &info) < 0) {
            return renderer_info{info};
        } else {
            throw error{};
        }
    }

    void read_pixels(void* pixels, const rect& rect, pixel_format_type format, int pitch) const
    {
        const auto prend = get_pointer();
        const auto fmt = static_cast<u32>(format);
        if (SDL_RenderReadPixels(prend, &rect, fmt, pixels, pitch) < 0) {
            throw error{};
        }
    }

    void clear()
    {
        if (SDL_RenderClear(get_pointer()) < 0) {
            throw error{};
        }
    }

    void draw_line(const point& p1, const point& p2)
    {
        if (SDL_RenderDrawLine(get_pointer(), p1.x, p1.y, p2.x, p2.y) < 0) {
            throw error{};
        }
    }

    void draw_line_strip(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        const auto sz = static_cast<int>(points.size());
        if (SDL_RenderDrawLines(get_pointer(), points.data(), sz) < 0) {
            throw error{};
        }
    }

    void draw_point(const point& p)
    {
        if (SDL_RenderDrawPoint(get_pointer(), p.x, p.y) < 0) {
            throw error{};
        }
    }

    void draw_points(span<const point> points)
    {
        if (!points.data()) {
            return;
        }
        const auto sz = static_cast<int>(points.size());
        if (SDL_RenderDrawPoints(get_pointer(), points.data(), sz) < 0) {
            throw error{};
        }
    }

    void draw_rectangle(const rect& rect)
    {
        if (SDL_RenderDrawRect(get_pointer(), &rect) < 0) {
            throw error{};
        }
    }

    void draw_rectangles(span<const rect> rectangles)
    {
        if (!rectangles.data()) {
            return;
        }
        const auto sz = static_cast<int>(rectangles.size());
        if (SDL_RenderDrawRects(get_pointer(), rectangles.data(), sz) == 0) {
            throw error{};
        }
    }

    void fill_rectangle(const rect& rect)
    {
        if (SDL_RenderFillRect(get_pointer(), &rect) < 0) {
            throw error{};
        }
    }

    void fill_rectangles(span<const rect> rectangles)
    {
        if (!rectangles.data()) return;
        const auto sz = static_cast<int>(rectangles.size());
        if (SDL_RenderFillRects(get_pointer(), rectangles.data(), sz) < 0) {
            throw error{};
        }
    }

    void copy(const texture&, const rect* src, const rect* dst);

    void copy(const texture&, const rect* src, const rect* dst, double angle, const point* center, renderer_flip);

    void present() noexcept
    {
        SDL_RenderPresent(get_pointer());
    }
};

class renderer : public renderer_ref {
public:
    using renderer_ref::renderer_ref;

    renderer(const renderer&) = delete;

    auto operator=(const renderer&) = delete;

    renderer(renderer&& other) noexcept
        : renderer_ref{std::exchange(other._prenderer, nullptr)}
    {}

    auto operator=(renderer&& other) noexcept -> renderer&
    {
        SDL_DestroyRenderer(_prenderer);
        _prenderer = std::exchange(other._prenderer, nullptr);
        return *this;
    }

    ~renderer() noexcept
    {
        SDL_DestroyRenderer(_prenderer);
        _prenderer = nullptr;
    }

    renderer(const window& win, renderer_flags flags, int rendering_driver_index = -1)
    {
        const auto pwin = win.get_pointer();
        const auto index = rendering_driver_index;
        const auto flags_ = static_cast<u32>(flags);
        if (const auto ptr = SDL_CreateRenderer(pwin, index, flags_)) {
            _prenderer = ptr;
        } else {
            throw error{};
        }
    }

    renderer(const surface& surf)
    {
        if (const auto ptr = SDL_CreateSoftwareRenderer(surf.get_pointer())) {
            _prenderer = ptr;
        } else {
            throw error{};
        }
    }
};

inline auto operator==(const renderer& lhs, const renderer& rhs) noexcept -> bool
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const renderer& lhs, const renderer& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

inline auto window_ref::renderer() -> renderer_ref
{
    if (const auto ptr = SDL_GetRenderer(_pwindow)) {
        return renderer_ref{ptr};
    } else {
        throw error{};
    }
}

// clang-format off

enum class texture_access : int {
    static_   = SDL_TEXTUREACCESS_STATIC,
    streaming = SDL_TEXTUREACCESS_STREAMING,
    target    = SDL_TEXTUREACCESS_TARGET
};

// clang-format on

class texture_ref {
protected:
    SDL_Texture* _ptexture = nullptr;

public:
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_ptexture);
    }

    texture_ref() noexcept
        : _ptexture{nullptr}
    {}

    explicit texture_ref(SDL_Texture* pointer) noexcept
        : _ptexture{pointer}
    {}

    auto get_pointer() const noexcept -> SDL_Texture*
    {
        return _ptexture;
    }

    auto access() const -> texture_access
    {
        const auto ptexture = get_pointer();
        auto access = int{};
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, &access, null, null) == 0) {
            return static_cast<texture_access>(access);
        } else {
            throw error{};
        }
    }

    auto alpha_mod() const -> u8
    {
        auto alpha = u8{};
        if (SDL_GetTextureAlphaMod(get_pointer(), &alpha) == 0) {
            return alpha;
        } else {
            throw error{};
        }
    }

    auto blend_mode() const -> sdl::blend_mode
    {
        auto mode = SDL_BlendMode{};
        if (SDL_GetTextureBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<sdl::blend_mode>(mode);
        } else {
            throw error{};
        }
    }

    auto color_mod() const -> std::array<u8, 3>
    {
        auto r = u8{};
        auto g = u8{};
        auto b = u8{};
        if (SDL_GetTextureColorMod(get_pointer(), &r, &g, &b) == 0) {
            return std::array{r, g, b};
        } else {
            throw error{};
        }
    }

    auto format() const -> pixel_format_type
    {
        const auto ptexture = get_pointer();
        auto format = u32{};
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, &format, null, null, null) == 0) {
            return static_cast<pixel_format_type>(format);
        } else {
            throw error{};
        }
    }

    auto size() const -> sdl::size
    {
        const auto ptexture = get_pointer();
        auto width = int();
        auto height = int();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, null, &width, &height) == 0) {
            return sdl::size{width, height};
        } else {
            throw error{};
        }
    }

    void set_alpha_mod(u8 alpha)
    {
        if (SDL_SetTextureAlphaMod(get_pointer(), alpha) < 0) {
            throw error{};
        }
    }

    void set_blend_mode(sdl::blend_mode mode)
    {
        const auto sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetTextureBlendMode(get_pointer(), sdl_blend_mode) < 0) {
            throw error{};
        }
    }

    void set_color_mod(u8 red, u8 green, u8 blue)
    {
        if (SDL_SetTextureColorMod(get_pointer(), red, green, blue) < 0) {
            throw error{};
        }
    }

    auto lock(const rect& area) -> std::tuple<void*, int>
    {
        auto pixels = static_cast<void*>(nullptr);
        auto pitch = int{};
        if (SDL_LockTexture(get_pointer(), &area, &pixels, &pitch) == 0) {
            return std::make_tuple(pixels, pitch);
        } else {
            throw error{};
        }
    }

    void unlock() noexcept
    {
        SDL_UnlockTexture(get_pointer());
    }

    void update(const rect& rect, const void* pixels, int pitch)
    {
        if (SDL_UpdateTexture(get_pointer(), &rect, pixels, pitch) < 0) {
            throw error();
        }
    }

    void update_yuv(const rect& rect, const u8* yplane, int ypitch, const u8* uplane, int upitch, const u8* vplane, int vpitch)
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
    {}

    auto operator=(texture&& other) noexcept -> texture&
    {
        SDL_DestroyTexture(_ptexture);
        _ptexture = std::exchange(other._ptexture, nullptr);
        return *this;
    }

    ~texture() noexcept
    {
        SDL_DestroyTexture(_ptexture);
        _ptexture = nullptr;
    }

    texture(const renderer& r, pixel_format_type format, texture_access access, const sdl::size& sz)
        : texture_ref{SDL_CreateTexture(r.get_pointer(), static_cast<u32>(format), static_cast<int>(access), sz.w, sz.h)}
    {
        if (!_ptexture) throw error{};
    }

    texture(const renderer& rend, const surface& surf)
        : texture_ref{SDL_CreateTextureFromSurface(rend.get_pointer(), surf.get_pointer())}
    {
        if (!_ptexture) throw error{};
    }
};

inline auto operator==(const texture& lhs, const texture& rhs) noexcept -> bool
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const texture& lhs, const texture& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

inline auto renderer_ref::target() -> texture_ref
{
    if (const auto ptr = SDL_GetRenderTarget(_prenderer)) {
        return texture_ref{ptr};
    } else {
        throw error{};
    }
}

inline void renderer_ref::set_target(texture* t)
{
    if (SDL_SetRenderTarget(_prenderer, t ? t->get_pointer() : nullptr) < 0) {
        throw error{};
    }
}

inline void renderer_ref::copy(const texture& tex, const rect* source, const rect* destination)
{
    const auto ptexture = tex.get_pointer();
    if (SDL_RenderCopy(get_pointer(), ptexture, source, destination) < 0) {
        throw error{};
    }
}

inline void renderer_ref::copy(const texture& t, const rect* src, const rect* dst, double angle, const point* center, renderer_flip f)
{
    if (SDL_RenderCopyEx(get_pointer(), t.get_pointer(), src, dst, angle, center, static_cast<SDL_RendererFlip>(f)) < 0) {
        throw error{};
    }
}

struct render_driver {
    static auto count() -> int
    {
        if (const auto number = SDL_GetNumRenderDrivers(); number >= 0) {
            return number;
        } else {
            throw error{};
        }
    }

    static auto info(int driver_index) -> renderer_info
    {
        auto info = SDL_RendererInfo{};
        if (SDL_GetRenderDriverInfo(driver_index, &info) == 0) {
            return renderer_info{info};
        } else {
            throw error{};
        }
    }
};

inline auto make_window_and_renderer(const size& window_size, window_flags flags) -> std::pair<window, renderer>
{
    /* const auto [w, h] = window_size; */
    const auto w = window_size.w;
    const auto h = window_size.h;
    auto pwindow = static_cast<SDL_Window*>(nullptr);
    auto prenderer = static_cast<SDL_Renderer*>(nullptr);
    const auto flags_ = static_cast<u32>(flags);
    if (SDL_CreateWindowAndRenderer(w, h, flags_, &pwindow, &prenderer) == 0) {
        return std::make_pair(window(pwindow), renderer(prenderer));
    } else {
        throw error{};
    }
}

} // namespace sdl
