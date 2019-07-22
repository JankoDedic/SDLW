#pragma once

#include <SDL2/SDL_render.h>

#include <sdlw/utility.hpp>
#include <sdlw/blend_mode.hpp>
#include <sdlw/surface.hpp>
#include <sdlw/video.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/size.hpp>

namespace sdlw {

class texture;
class texture_ref;

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

    pixel_format_type
    texture_format(int index) const noexcept
    {
        const auto tex_format = _renderer_info.texture_formats[index];
        return static_cast<pixel_format_type>(tex_format);
    }

    size
    max_texture_size() const noexcept
    {
        const auto width = _renderer_info.max_texture_width;
        const auto height = _renderer_info.max_texture_height;
        return size{width, height};
    }
};

class renderer_ref {
protected:
    SDL_Renderer* _prenderer;

public:
    renderer_ref() noexcept : _prenderer{nullptr} {}
    renderer_ref(SDL_Renderer* pointer) noexcept : _prenderer{pointer} {}
    auto get_pointer() const noexcept -> SDL_Renderer* { return _prenderer; }

    auto draw_blend_mode() const -> blend_mode {
        auto mode = SDL_BlendMode{};
        if (SDL_GetRenderDrawBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<blend_mode>(mode);
        } else {
            throw error{};
        }
    }

    void set_draw_blend_mode(blend_mode mode) {
        const auto sdl_blend_mode = static_cast<SDL_BlendMode>(mode);
        if (SDL_SetRenderDrawBlendMode(get_pointer(), sdl_blend_mode) < 0) {
            throw error{};
        }
    }

    color draw_color() const {
        auto c = color{};
        if (SDL_GetRenderDrawColor(get_pointer(), &c.r, &c.g, &c.b, &c.a) == 0) {
            return c;
        } else {
            throw error{};
        }
    }

    void set_draw_color(color c) {
        if (SDL_SetRenderDrawColor(get_pointer(), c.r, c.g, c.b, c.a) < 0) {
            throw error{};
        }
    }

    auto are_targets_supported() const noexcept -> bool {
        return SDL_RenderTargetSupported(get_pointer());
    }

    auto target() -> texture_ref;

    void set_target(texture*);

    auto output_size() const -> size {
        auto sz = size{};
        if (SDL_GetRendererOutputSize(get_pointer(), &sz.w, &sz.h) == 0) {
            return sz;
        } else {
            throw error{};
        }
    }

    auto is_clip_enabled() const noexcept -> bool {
        return SDL_RenderIsClipEnabled(get_pointer());
    }

    auto clip() const noexcept -> rectangle {
        auto rect = rectangle{};
        SDL_RenderGetClipRect(get_pointer(), &rect);
        return rect;
    }

    void set_clip(const rectangle& rect) {
        if (SDL_RenderSetClipRect(get_pointer(), &rect) < 0) {
            throw error{};
        }
    }

    auto viewport() const noexcept -> rectangle {
        auto rect = rectangle{};
        SDL_RenderGetViewport(get_pointer(), &rect);
        return rect;
    }

    void set_viewport(const rectangle& viewport) {
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

    void set_scale(float xscale, float yscale) {
        if (SDL_RenderSetScale(get_pointer(), xscale, yscale) < 0) {
            throw error{};
        }
    }

    auto integer_scale() const noexcept -> bool {
        return SDL_RenderGetIntegerScale(get_pointer());
    }

    void set_integer_scale(bool should_enable) {
        const auto b = static_cast<SDL_bool>(should_enable);
        if (SDL_RenderSetIntegerScale(get_pointer(), b) < 0) {
            throw error{};
        }
    }

    auto logical_size() const noexcept -> size {
        auto sz = size{};
        SDL_RenderGetLogicalSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_logical_size(const size& sz) {
        if (SDL_RenderSetLogicalSize(get_pointer(), sz.w, sz.h) < 0) {
            throw error{};
        }
    }

    auto info() const -> renderer_info {
        auto info = SDL_RendererInfo{};
        if (SDL_GetRendererInfo(get_pointer(), &info) < 0) {
            return renderer_info{info};
        } else {
            throw error{};
        }
    }

    void read_pixels(void* pixels, const rectangle& rect, pixel_format_type format, int pitch) const {
        const auto prend = get_pointer();
        const auto fmt = static_cast<u32>(format);
        if (SDL_RenderReadPixels(prend, &rect, fmt, pixels, pitch) < 0) {
            throw error{};
        }
    }

    void
    clear()
    {
        if (SDL_RenderClear(get_pointer()) < 0) {
            throw error{};
        }
    }

    void
    draw_line(const point& p1, const point& p2)
    {
        if (SDL_RenderDrawLine(get_pointer(), p1.x, p1.y, p2.x, p2.y) < 0) {
            throw error{};
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
            throw error{};
        }
    }

    void
    draw_point(const point& p)
    {
        if (SDL_RenderDrawPoint(get_pointer(), p.x, p.y) < 0) {
            throw error{};
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
            throw error{};
        }
    }

    void
    draw_rectangle(const rectangle& rect)
    {
        if (SDL_RenderDrawRect(get_pointer(), &rect) < 0) {
            throw error{};
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
            throw error{};
        }
    }

    void
    fill_rectangle(const rectangle& rect)
    {
        if (SDL_RenderFillRect(get_pointer(), &rect) < 0) {
            throw error{};
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
            throw error{};
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

class renderer : public renderer_ref {
public:
    using renderer_ref::renderer_ref;

    renderer(const renderer&) = delete;
    auto operator=(const renderer&) = delete;

    renderer(renderer&& other) noexcept
        : renderer_ref{std::exchange(other._prenderer, nullptr)}
    {
    }

    auto operator=(renderer&& other) noexcept -> renderer& {
        SDL_DestroyRenderer(_prenderer);
        _prenderer = std::exchange(other._prenderer, nullptr);
        return *this;
    }

    ~renderer() noexcept {
        SDL_DestroyRenderer(_prenderer);
        _prenderer = nullptr;
    }

    renderer(const window& win, renderer_flags flags, int rendering_driver_index = -1) {
        const auto pwin = win.get_pointer();
        const auto index = rendering_driver_index;
        const auto flags_ = static_cast<u32>(flags);
        if (const auto ptr = SDL_CreateRenderer(pwin, index, flags_)) {
            _prenderer = ptr;
        } else {
            throw error{};
        }
    }

    renderer(const surface& surf) {
        if (const auto ptr = SDL_CreateSoftwareRenderer(surf.get_pointer())) {
            _prenderer = ptr;
        } else {
            throw error{};
        }
    }
};

inline auto operator==(const renderer& lhs, const renderer& rhs) noexcept -> bool {
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const renderer& lhs, const renderer& rhs) noexcept -> bool {
    return !(lhs == rhs);
}

inline auto window_ref::renderer() -> renderer_ref {
    if (const auto ptr = SDL_GetRenderer(_pwindow)) {
        return {ptr};
    } else {
        throw error{};
    }
}

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

    sdlw::blend_mode
    blend_mode() const
    {
        auto mode = SDL_BlendMode();
        if (SDL_GetTextureBlendMode(get_pointer(), &mode) == 0) {
            return static_cast<sdlw::blend_mode>(mode);
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

    pixel_format_type
    format() const
    {
        const auto ptexture = get_pointer();
        auto format = u32();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, &format, null, null, null) == 0) {
            return static_cast<pixel_format_type>(format);
        } else {
            throw error();
        }
    }

    sdlw::size
    size() const
    {
        const auto ptexture = get_pointer();
        auto width = int();
        auto height = int();
        constexpr auto null = nullptr;
        if (SDL_QueryTexture(ptexture, null, null, &width, &height) == 0) {
            return sdlw::size{width, height};
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
    set_blend_mode(sdlw::blend_mode mode)
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

    texture(const renderer& rend, pixel_format_type format, texture_access access, const sdlw::size& sz)
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

inline auto renderer_ref::target() -> texture_ref {
    if (const auto ptr = SDL_GetRenderTarget(_prenderer)) {
        return {ptr};
    } else {
        throw error{};
    }
}

inline void renderer_ref::set_target(texture* t) {
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

struct render_driver {
    static
    int
    count()
    {
        if (const auto number = SDL_GetNumRenderDrivers(); number >= 0) {
            return number;
        } else {
            throw error();
        }
    }

    static
    renderer_info
    info(int driver_index)
    {
        auto info = SDL_RendererInfo();
        if (SDL_GetRenderDriverInfo(driver_index, &info) == 0) {
            return renderer_info(info);
        } else {
            throw error();
        }
    }
};

inline
std::pair<window, renderer>
make_window_and_renderer(const size& window_size, window_flags flags)
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
        throw error();
    }
}

} // namespace sdlw
