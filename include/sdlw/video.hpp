#pragma once

#include <array>
#include <memory>

#include <SDL2/SDL_video.h>

#include <sdlw/error.hpp>
#include <sdlw/pixels.hpp>
#include <sdlw/rect.hpp>
#include <sdlw/surface.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class display_mode {
    SDL_DisplayMode _display_mode;

public:
    constexpr explicit operator SDL_DisplayMode() const noexcept
    {
        return _display_mode;
    }

    display_mode() = default;

    constexpr explicit display_mode(const SDL_DisplayMode& display_mode) noexcept
        : _display_mode{display_mode}
    {}

    constexpr auto format() const noexcept -> pixel_format_type
    {
        return static_cast<pixel_format_type>(_display_mode.format);
    }

    constexpr void set_format(pixel_format_type format) noexcept
    {
        _display_mode.format = static_cast<u32>(format);
    }

    constexpr auto size() const noexcept -> sdl::size
    {
        return {_display_mode.w, _display_mode.h};
    }

    constexpr void set_size(const sdl::size& size) noexcept
    {
        _display_mode.w = size.w;
        _display_mode.h = size.h;
    }

    constexpr auto refresh_rate() const noexcept -> const int&
    {
        return _display_mode.refresh_rate;
    }

    constexpr auto refresh_rate() noexcept -> int&
    {
        return _display_mode.refresh_rate;
    }

    constexpr auto driver_data() const noexcept -> void* const&
    {
        return _display_mode.driverdata;
    }

    constexpr auto driver_data() noexcept -> void*&
    {
        return _display_mode.driverdata;
    }
};

class renderer_ref; // for window::renderer

// clang-format off

using window_id = u32;

enum class window_fullscreen_mode : u32 {
    normal          = SDL_WINDOW_FULLSCREEN,
    fake_fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
    windowed        = 0
};

enum class hit_test_result {
    normal              = SDL_HITTEST_NORMAL,
    draggable           = SDL_HITTEST_DRAGGABLE,
    resize_top_left     = SDL_HITTEST_RESIZE_TOPLEFT,
    resize_top          = SDL_HITTEST_RESIZE_TOP,
    resize_top_right    = SDL_HITTEST_RESIZE_TOPRIGHT,
    resize_right        = SDL_HITTEST_RESIZE_RIGHT,
    resize_bottom_right = SDL_HITTEST_RESIZE_BOTTOMRIGHT,
    resize_bottom       = SDL_HITTEST_RESIZE_BOTTOM,
    resize_bottom_left  = SDL_HITTEST_RESIZE_BOTTOMLEFT,
    resize_left         = SDL_HITTEST_RESIZE_LEFT
};

// clang-format on

class window_ref;

class window {
public:
    enum flags : u32 {
        fullscreen         = SDL_WINDOW_FULLSCREEN,
        opengl             = SDL_WINDOW_OPENGL,
        shown              = SDL_WINDOW_SHOWN,
        hidden             = SDL_WINDOW_HIDDEN,
        borderless         = SDL_WINDOW_BORDERLESS,
        resizable          = SDL_WINDOW_RESIZABLE,
        minimized          = SDL_WINDOW_MINIMIZED,
        maximized          = SDL_WINDOW_MAXIMIZED,
        input_grabbed      = SDL_WINDOW_INPUT_GRABBED,
        input_focus        = SDL_WINDOW_INPUT_FOCUS,
        mouse_focus        = SDL_WINDOW_MOUSE_FOCUS,
        fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
        foreign            = SDL_WINDOW_FOREIGN,
        allow_high_dpi     = SDL_WINDOW_ALLOW_HIGHDPI,
        mouse_capture      = SDL_WINDOW_MOUSE_CAPTURE,
        always_on_top      = SDL_WINDOW_ALWAYS_ON_TOP,
        skip_taskbar       = SDL_WINDOW_SKIP_TASKBAR,
        utility            = SDL_WINDOW_UTILITY,
        tooltip            = SDL_WINDOW_TOOLTIP,
        popup_menu         = SDL_WINDOW_POPUP_MENU,
        vulkan             = SDL_WINDOW_VULKAN
    };

    window(class window_ref) = delete;
    explicit window(SDL_Window* w) noexcept : _window{w} {}
    auto get_pointer() const noexcept -> SDL_Window* { return _window.get(); }

    // clang-format off
    static constexpr auto centered      = int{SDL_WINDOWPOS_CENTERED};
    static constexpr auto undefined     = int{SDL_WINDOWPOS_UNDEFINED};
    static constexpr auto pos_centered  = point{centered, centered};
    static constexpr auto pos_undefined = point{undefined, undefined};
    // clang-format on

    static constexpr auto centered_on_display(int display_index) noexcept -> int
    {
        return SDL_WINDOWPOS_CENTERED_DISPLAY(display_index);
    }

    static constexpr auto undefined_on_display(int display_index) noexcept -> int
    {
        return SDL_WINDOWPOS_UNDEFINED_DISPLAY(display_index);
    }

    static constexpr auto pos_centered_on_display(int display_index) noexcept -> point
    {
        const auto value = centered_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto pos_undefined_on_display(int display_index) noexcept -> point
    {
        const auto value = undefined_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto is_centered(int coordinate) noexcept -> bool
    {
        return SDL_WINDOWPOS_ISCENTERED(coordinate);
    }

    static constexpr auto is_undefined(int coordinate) noexcept -> bool
    {
        return SDL_WINDOWPOS_ISUNDEFINED(coordinate);
    }

    static constexpr auto is_pos_centered(const point& position) noexcept -> bool
    {
        return is_centered(position.x) && is_centered(position.y);
    }

    static constexpr auto is_pos_undefined(const point& position) noexcept -> bool
    {
        return is_undefined(position.x) && is_undefined(position.y);
    }

    window(const char* title, const rect& bounds, window::flags flags)
        : window{SDL_CreateWindow(title, bounds.x, bounds.y, bounds.w, bounds.h, static_cast<u32>(flags))}
    {
        if (!_window) throw error{};
    }

    explicit window(void* native_window_data)
        : window{SDL_CreateWindowFrom(native_window_data)}
    {
        if (!_window) throw error{};
    }

    auto flags() const noexcept -> enum window::flags
    {
        return static_cast<enum window::flags>(SDL_GetWindowFlags(get_pointer()));
    }

    auto id() const noexcept -> window_id
    {
        return static_cast<window_id>(SDL_GetWindowID(get_pointer()));
    }

    auto title() const noexcept -> const char*
    {
        return SDL_GetWindowTitle(get_pointer());
    }

    void set_title(const char* title) noexcept
    {
        SDL_SetWindowTitle(get_pointer(), title);
    }

    auto position() const noexcept -> point
    {
        auto x = int{};
        auto y = int{};
        SDL_GetWindowPosition(get_pointer(), &x, &y);
        return point{x, y};
    }

    void set_position(const point& position) noexcept
    {
        SDL_SetWindowPosition(get_pointer(), position.x, position.y);
    }

    auto size() const noexcept -> sdl::size
    {
        auto width = int{};
        auto height = int{};
        SDL_GetWindowSize(get_pointer(), &width, &height);
        return {width, height};
    }

    void set_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowSize(get_pointer(), size.w, size.h);
    }

    void set_resizable(bool resizable) noexcept
    {
        SDL_SetWindowResizable(get_pointer(), static_cast<SDL_bool>(resizable));
    }

    auto border_sizes() const -> std::array<int, 4>
    {
        auto bsizes = std::array<int, 4>{};
        if (SDL_GetWindowBordersSize(get_pointer(), &bsizes[0], &bsizes[1], &bsizes[2], &bsizes[3]) < 0) {
            throw error{};
        } else {
            return bsizes;
        }
    }

    void set_bordered(bool bordered) noexcept
    {
        SDL_SetWindowBordered(get_pointer(), static_cast<SDL_bool>(bordered));
    }

    auto max_size() const noexcept -> sdl::size
    {
        auto sz = sdl::size{};
        SDL_GetWindowMaximumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_max_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowMaximumSize(get_pointer(), size.w, size.h);
    }

    auto min_size() const noexcept -> sdl::size
    {
        auto sz = sdl::size{};
        SDL_GetWindowMinimumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_min_size(const sdl::size& size) noexcept
    {
        SDL_SetWindowMinimumSize(get_pointer(), size.w, size.h);
    }

    auto is_input_grabbed() const noexcept -> bool
    {
        return static_cast<bool>(SDL_GetWindowGrab(get_pointer()));
    }

    void set_input_grab(bool is_grabbed) noexcept
    {
        SDL_SetWindowGrab(get_pointer(), static_cast<SDL_bool>(is_grabbed));
    }

    auto data(const char* name) const noexcept -> void*
    {
        return SDL_GetWindowData(get_pointer(), name);
    }

    auto set_data(const char* name, void* user_data) noexcept -> void*
    {
        return SDL_SetWindowData(get_pointer(), name, user_data);
    }

    auto surface() -> surface_ref
    {
        if (const auto ptr = SDL_GetWindowSurface(get_pointer())) {
            return surface_ref{ptr};
        } else {
            throw error{};
        }
    }

    auto opacity() const -> float
    {
        auto opacity = float{};
        if (SDL_GetWindowOpacity(get_pointer(), &opacity) == 0) {
            return opacity;
        } else {
            throw error{};
        }
    }

    void set_opacity(float opacity)
    {
        if (SDL_SetWindowOpacity(get_pointer(), opacity) == 0) {
            throw error{};
        }
    }

    auto display_mode() const -> sdl::display_mode
    {
        auto mode = SDL_DisplayMode{};
        if (SDL_GetWindowDisplayMode(get_pointer(), &mode) == 0) {
            return sdl::display_mode(mode);
        } else {
            throw error{};
        }
    }

    void set_display_mode(const sdl::display_mode* mode)
    {
        const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(mode);
        if (SDL_SetWindowDisplayMode(get_pointer(), pmode) < 0) {
            throw error{};
        }
    }

    auto display_index() const -> int
    {
        const auto index = SDL_GetWindowDisplayIndex(get_pointer());
        if (index < 0) {
            throw error{};
        } else {
            return index;
        }
    }

    auto pixel_format_type() const -> sdl::pixel_format_type
    {
        const auto format = SDL_GetWindowPixelFormat(get_pointer());
        if (format == SDL_PIXELFORMAT_UNKNOWN) {
            throw error{};
        } else {
            return static_cast<sdl::pixel_format_type>(format);
        }
    }

    void hide() noexcept
    {
        SDL_HideWindow(get_pointer());
    }

    void show() noexcept
    {
        SDL_ShowWindow(get_pointer());
    }

    void set_fullscreen_mode(window_fullscreen_mode mode)
    {
        const auto sdl_mode = static_cast<u32>(mode);
        if (SDL_SetWindowFullscreen(get_pointer(), sdl_mode) < 0) {
            throw error{};
        }
    }

    void maximize() noexcept
    {
        SDL_MaximizeWindow(get_pointer());
    }

    void minimize() noexcept
    {
        SDL_MinimizeWindow(get_pointer());
    }

    void restore() noexcept
    {
        SDL_RestoreWindow(get_pointer());
    }

    void raise() noexcept
    {
        SDL_RaiseWindow(get_pointer());
    }

    void set_input_focus()
    {
        if (SDL_SetWindowInputFocus(get_pointer()) < 0) {
            throw error{};
        }
    }

    void update_surface()
    {
        if (SDL_UpdateWindowSurface(get_pointer()) < 0) {
            throw error{};
        }
    }

    void update_surface_areas(span<const rect> areas)
    {
        const auto pwin = get_pointer();
        const auto sz = static_cast<int>(areas.size());
        if (SDL_UpdateWindowSurfaceRects(pwin, areas.data(), sz) < 0) {
            throw error{};
        }
    }

    void set_icon(sdl::surface_ref icon) noexcept
    {
        SDL_SetWindowIcon(get_pointer(), icon.get_pointer());
    }

    auto renderer() -> renderer_ref;

    void set_modal(const window& modal)
    {
        if (SDL_SetWindowModalFor(modal.get_pointer(), get_pointer()) < 0) {
            throw error{};
        }
    }

    template<typename HitTest>
    void set_hit_test(HitTest& ht);

    void set_hit_test(hit_test_result (*ht)(window_ref, const point&));

protected:
    std::unique_ptr<SDL_Window, detail::make_functor<SDL_DestroyWindow>> _window;
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(enum window::flags)

inline auto operator==(const window& lhs, const window& rhs) noexcept -> bool
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const window& lhs, const window& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

class window_ref : public window {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_window); }
    explicit window_ref(SDL_Window* w) noexcept : window{w} {}
    ~window_ref() { _window.release(); }
};

template<typename HitTest>
void window::set_hit_test(HitTest& ht)
{
    static_assert(std::is_invocable_r_v<hit_test_result, HitTest, window_ref, const point&>);
    constexpr auto sdl_callback = [](SDL_Window* win, const SDL_Point* area, void* data) -> SDL_HitTestResult {
        auto& test = *static_cast<HitTest*>(data);
        const auto result = test(window_ref{win}, *area);
        return static_cast<SDL_HitTestResult>(result);
    };
    if (SDL_SetWindowHitTest(get_pointer(), sdl_callback, &ht) < 0) {
        throw error{};
    }
}

inline void window::set_hit_test(hit_test_result (*ht)(window_ref, const point&))
{
    constexpr auto fp_sdl_callback = [](SDL_Window* win, const SDL_Point* area, void* data) -> SDL_HitTestResult {
        auto test = reinterpret_cast<hit_test_result (*)(window_ref, const point&)>(data);
        const auto result = test(window_ref{win}, *static_cast<const point*>(area));
        return static_cast<SDL_HitTestResult>(result);
    };
    if (SDL_SetWindowHitTest(get_pointer(), fp_sdl_callback, reinterpret_cast<void*>(ht)) < 0) {
        throw error{};
    }
}

inline auto get_window(window_id id) -> window_ref
{
    if (const auto ptr = SDL_GetWindowFromID(static_cast<u32>(id))) {
        return window_ref{ptr};
    } else {
        throw error{};
    }
}

inline auto get_grabbed_window() -> window_ref
{
    if (const auto ptr = SDL_GetGrabbedWindow()) {
        return window_ref{ptr};
    } else {
        throw error{};
    }
}

inline auto closest_display_mode(int display_index, const display_mode& m) -> display_mode
{
    const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(&m);
    auto closest = SDL_DisplayMode{};
    if (SDL_GetClosestDisplayMode(display_index, pmode, &closest)) {
        return display_mode{closest};
    } else {
        throw error{};
    }
}

inline auto current_display_mode(int display_index) -> display_mode
{
    auto current = SDL_DisplayMode{};
    if (SDL_GetCurrentDisplayMode(display_index, &current) == 0) {
        return display_mode{current};
    } else {
        throw error{};
    }
}

inline auto desktop_display_mode(int display_index) -> display_mode
{
    auto desktop = SDL_DisplayMode{};
    if (SDL_GetDesktopDisplayMode(display_index, &desktop) == 0) {
        return display_mode{desktop};
    } else {
        throw error{};
    }
}

inline auto display_bounds(int display_index) -> rect
{
    auto bounds = rect{};
    if (SDL_GetDisplayBounds(display_index, &bounds) == 0) {
        return bounds;
    } else {
        throw error{};
    }
}

inline auto display_dpi(int display_index) -> std::array<float, 3>
{
    auto ddpi = float{};
    auto hdpi = float{};
    auto vdpi = float{};
    if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) == 0) {
        return std::array{ddpi, hdpi, vdpi};
    } else {
        throw error{};
    }
}

inline auto get_display_mode(int display_index, int mode_index) -> display_mode
{
    auto m = SDL_DisplayMode{};
    if (SDL_GetDisplayMode(display_index, mode_index, &m) == 0) {
        return display_mode{m};
    } else {
        throw error{};
    }
}

inline auto get_display_name(int display_index) noexcept -> const char*
{
    return SDL_GetDisplayName(display_index);
}

inline auto display_usable_bounds(int display_index) -> rect
{
    auto r = rect{};
    if (SDL_GetDisplayUsableBounds(display_index, &r) == 0) {
        return r;
    } else {
        throw error{};
    }
}

inline auto num_display_modes(int display_index) -> int
{
    if (const auto count = SDL_GetNumDisplayModes(display_index); count < 0) {
        throw error{};
    } else {
        return count;
    }
}

inline auto num_video_displays() -> int
{
    if (const auto cnt = SDL_GetNumVideoDisplays(); cnt < 0) {
        throw error{};
    } else {
        return cnt;
    }
}

inline auto display_brightness(const window& win) noexcept -> float
{
    return SDL_GetWindowBrightness(win.get_pointer());
}

inline void set_display_brightness(const window& win, float brightness)
{
    if (SDL_SetWindowBrightness(win.get_pointer(), brightness) < 0) {
        throw error{};
    }
}

inline void get_display_gamma_ramp(const window& win, span<u16, 256> red, span<u16, 256> green, span<u16, 256> blue)
{
    const auto r = red.data();
    const auto g = green.data();
    const auto b = blue.data();
    if (SDL_GetWindowGammaRamp(win.get_pointer(), r, g, b) < 0) {
        throw error{};
    }
}

inline void set_display_gamma_ramp(const window& win, span<const u16, 256> red, span<const u16, 256> green, span<const u16, 256> blue)
{
    const auto r = red.data();
    const auto g = green.data();
    const auto b = blue.data();
    if (SDL_SetWindowGammaRamp(win.get_pointer(), r, g, b) < 0) {
        throw error{};
    }
}

struct screen_saver {
    static auto is_enabled() noexcept -> bool
    {
        return SDL_IsScreenSaverEnabled();
    }

    static void enable() noexcept
    {
        SDL_EnableScreenSaver();
    }

    static void disable() noexcept
    {
        SDL_DisableScreenSaver();
    }
};

inline auto num_video_drivers() noexcept -> int
{
    return SDL_GetNumVideoDrivers();
}

inline auto video_driver_name(int driver_index) noexcept -> const char*
{
    return SDL_GetVideoDriver(driver_index);
}

inline auto current_video_driver() noexcept -> const char*
{
    return SDL_GetCurrentVideoDriver();
}

struct video_subsystem {
    video_subsystem(const char* driver_name)
    {
        if (SDL_VideoInit(driver_name) < 0) {
            throw error{};
        }
    }

    ~video_subsystem() noexcept
    {
        SDL_VideoQuit();
    }
};

namespace gl {

class context {
public:
    context(class context_ref) = delete;
    explicit context(SDL_GLContext c) noexcept : _context{c} {}
    auto get_pointer() const noexcept -> SDL_GLContext { return _context.get(); }

    explicit context(window& w)
        : context{SDL_GL_CreateContext(w.get_pointer())}
    {
        if (!_context) throw error{};
    }

protected:
    std::unique_ptr<std::remove_pointer_t<SDL_GLContext>, detail::make_functor<SDL_GL_DeleteContext>> _context;
};

class context_ref : public context {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_context); }
    explicit context_ref(SDL_GLContext c) noexcept : context{c} {}
    ~context_ref() { _context.release(); }
};

enum class attr : std::underlying_type_t<SDL_GLattr> {
    red_size                   = SDL_GL_RED_SIZE,
    green_size                 = SDL_GL_GREEN_SIZE,
    blue_size                  = SDL_GL_BLUE_SIZE,
    alpha_size                 = SDL_GL_ALPHA_SIZE,
    buffer_size                = SDL_GL_BUFFER_SIZE,
    double_buffer              = SDL_GL_DOUBLEBUFFER,
    depth_size                 = SDL_GL_DEPTH_SIZE,
    stencil_size               = SDL_GL_STENCIL_SIZE,
    accum_red_size             = SDL_GL_ACCUM_RED_SIZE,
    accum_green_size           = SDL_GL_ACCUM_GREEN_SIZE,
    accum_blue_size            = SDL_GL_ACCUM_BLUE_SIZE,
    accum_alpha_size           = SDL_GL_ACCUM_ALPHA_SIZE,
    stereo                     = SDL_GL_STEREO,
    multisample_buffers        = SDL_GL_MULTISAMPLEBUFFERS,
    multisample_samples        = SDL_GL_MULTISAMPLESAMPLES,
    accelerated_visual         = SDL_GL_ACCELERATED_VISUAL,
    retained_backing           = SDL_GL_RETAINED_BACKING,
    context_major_version      = SDL_GL_CONTEXT_MAJOR_VERSION,
    context_minor_version      = SDL_GL_CONTEXT_MINOR_VERSION,
    context_egl                = SDL_GL_CONTEXT_EGL,
    context_flags              = SDL_GL_CONTEXT_FLAGS,
    context_profile_mask       = SDL_GL_CONTEXT_PROFILE_MASK,
    share_with_current_context = SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
    framebuffer_srgb_capable   = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
    context_release_behavior   = SDL_GL_CONTEXT_RELEASE_BEHAVIOR,
    context_reset_notification = SDL_GL_CONTEXT_RESET_NOTIFICATION,
    context_no_error           = SDL_GL_CONTEXT_NO_ERROR,
};

enum class profile : std::underlying_type_t<SDL_GLprofile> {
    core          = SDL_GL_CONTEXT_PROFILE_CORE,
    compatibility = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
    es            = SDL_GL_CONTEXT_PROFILE_ES
};

enum class context_flag : std::underlying_type_t<SDL_GLcontextFlag> {
    debug              = SDL_GL_CONTEXT_DEBUG_FLAG,
    forward_compatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    robust_access      = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    reset_isolation    = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
};

inline auto extension_supported(const char* extension) noexcept -> bool
{
    return static_cast<bool>(SDL_GL_ExtensionSupported(extension));
}

inline auto attribute(attr a) -> int
{
    auto result = int{};
    if (SDL_GL_GetAttribute(static_cast<SDL_GLattr>(a), &result) < 0) {
        throw error{};
    } else {
        return result;
    }
}

inline auto current_context() -> context_ref
{
    if (const auto p = SDL_GL_GetCurrentContext()) {
        return context_ref{p};
    } else {
        throw error{};
    }
}

inline auto current_window() -> window_ref
{
    if (const auto p = SDL_GL_GetCurrentWindow()) {
        return window_ref{p};
    } else {
        throw error{};
    }
}

inline auto drawable_size(const window& w) noexcept -> sdl::size
{
    auto s = sdl::size{};
    SDL_GL_GetDrawableSize(w.get_pointer(), &s.w, &s.h);
    return s;
}

inline auto proc_address(const char* proc) noexcept -> void*
{
    return SDL_GL_GetProcAddress(proc);
}

inline auto swap_interval() noexcept -> int
{
    return SDL_GL_GetSwapInterval();
}

inline void load_library(const char* path)
{
    if (SDL_GL_LoadLibrary(path) < 0) {
        throw error{};
    }
}

inline void make_current(const window& w, context_ref c)
{
    if (SDL_GL_MakeCurrent(w.get_pointer(), c.get_pointer()) < 0) {
        throw error{};
    }
}

inline void reset_attributes() noexcept
{
    SDL_GL_ResetAttributes();
}

template<typename IntegerOrEnum>
void set_attribute(attr a, IntegerOrEnum value)
{
    if (SDL_GL_SetAttribute(static_cast<SDL_GLattr>(a), static_cast<int>(value)) < 0) {
        throw error{};
    }
}

inline void set_swap_interval(int interval)
{
    if (SDL_GL_SetSwapInterval(interval) < 0) {
        throw error{};
    }
}

inline void swap_window(const window& w) noexcept
{
    return SDL_GL_SwapWindow(w.get_pointer());
}

inline void unload_library() noexcept
{
    SDL_GL_UnloadLibrary();
}

} // namespace gl

} // namespace sdl
