#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>

#include <SDL2/SDL.h>

#include <sdlw/error.hpp>
#include <sdlw/utility.hpp>
#include <sdlw/video/display/mode.hpp>
#include <sdlw/video/rectangle.hpp>
#include <sdlw/video/surface.hpp>

namespace sdlw::video {

class window;   // for hit_test
class renderer; // for window::renderer

enum class window_flags : u32 {
    fullscreen = SDL_WINDOW_FULLSCREEN,
    opengl = SDL_WINDOW_OPENGL,
    shown = SDL_WINDOW_SHOWN,
    hidden = SDL_WINDOW_HIDDEN,
    borderless = SDL_WINDOW_BORDERLESS,
    resizable = SDL_WINDOW_RESIZABLE,
    minimized = SDL_WINDOW_MINIMIZED,
    maximized = SDL_WINDOW_MAXIMIZED,
    input_grabbed = SDL_WINDOW_INPUT_GRABBED,
    input_focus = SDL_WINDOW_INPUT_FOCUS,
    mouse_focus = SDL_WINDOW_MOUSE_FOCUS,
    fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    foreign = SDL_WINDOW_FOREIGN,
    allow_high_dpi = SDL_WINDOW_ALLOW_HIGHDPI,
    mouse_capture = SDL_WINDOW_MOUSE_CAPTURE,
    always_on_top = SDL_WINDOW_ALWAYS_ON_TOP,
    skip_taskbar = SDL_WINDOW_SKIP_TASKBAR,
    utility = SDL_WINDOW_UTILITY,
    tooltip = SDL_WINDOW_TOOLTIP,
    popup_menu = SDL_WINDOW_POPUP_MENU,
    vulkan = SDL_WINDOW_VULKAN
};

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(window_flags);

enum class window_id : u32 {};

enum class window_fullscreen_mode : u32 {
    normal = SDL_WINDOW_FULLSCREEN,
    fake_fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
    windowed = 0
};

enum class hit_test_result {
    normal = SDL_HITTEST_NORMAL,
    draggable = SDL_HITTEST_DRAGGABLE,
    resize_top_left = SDL_HITTEST_RESIZE_TOPLEFT,
    resize_top = SDL_HITTEST_RESIZE_TOP,
    resize_top_right = SDL_HITTEST_RESIZE_TOPRIGHT,
    resize_right = SDL_HITTEST_RESIZE_RIGHT,
    resize_bottom_right = SDL_HITTEST_RESIZE_BOTTOMRIGHT,
    resize_bottom = SDL_HITTEST_RESIZE_BOTTOM,
    resize_bottom_left = SDL_HITTEST_RESIZE_BOTTOMLEFT,
    resize_left = SDL_HITTEST_RESIZE_LEFT
};

using hit_test = hit_test_result(window&, const point&);

namespace detail {

    template<typename T, class U>
    using map = std::unordered_map<T, U>;

    inline auto hit_tests = map<window_id, std::function<hit_test>>();

} // namespace detail

class window {
    static
    SDL_HitTestResult
    sdl_hit_test_callback(SDL_Window* pwin, const SDL_Point* area, void* data)
    {
        auto win_storage = ::sdlw::detail::storage<window>();
        auto& win = *new (&win_storage) window(pwin);
        auto& pred = *reinterpret_cast<std::function<hit_test>*>(data);
        const auto res = pred(win, *area);
        return static_cast<SDL_HitTestResult>(res);
    }

    using deleter = ::sdlw::detail::make_functor<SDL_DestroyWindow>;

    std::unique_ptr<SDL_Window, deleter> _window;

public:
    using size_type = size;
    using renderer_type = renderer;
    using surface_type = surface;

    static constexpr auto centered = SDL_WINDOWPOS_CENTERED;
    static constexpr auto undefined = SDL_WINDOWPOS_UNDEFINED;
    static constexpr auto pos_centered = point{centered, centered};
    static constexpr auto pos_undefined = point{undefined, undefined};

    static constexpr
    int
    centered_on_display(int display_index) noexcept
    {
        return SDL_WINDOWPOS_CENTERED_DISPLAY(display_index);
    }

    static constexpr
    int
    undefined_on_display(int display_index) noexcept
    {
        return SDL_WINDOWPOS_UNDEFINED_DISPLAY(display_index);
    }

    static constexpr
    point
    pos_centered_on_display(int display_index) noexcept
    {
        const auto value = centered_on_display(display_index);
        return point{value, value};
    }

    static constexpr
    point
    pos_undefined_on_display(int display_index) noexcept
    {
        const auto value = undefined_on_display(display_index);
        return point{value, value};
    }

    static constexpr
    bool
    is_centered(int coordinate) noexcept
    {
        return SDL_WINDOWPOS_ISCENTERED(coordinate);
    }

    static constexpr
    bool
    is_undefined(int coordinate) noexcept
    {
        return SDL_WINDOWPOS_ISUNDEFINED(coordinate);
    }

    static constexpr
    bool
    is_pos_centered(const point& position) noexcept
    {
        return is_centered(position.x) && is_centered(position.y);
    }

    static constexpr
    bool
    is_pos_undefined(const point& position) noexcept
    {
        return is_undefined(position.x) && is_undefined(position.y);
    }

    window() noexcept = default;

    window(SDL_Window* pointer) noexcept
        : _window(pointer)
    {
    }

    window(const char* title, const rectangle& bounds, window_flags flags)
    {
        const auto [x, y, w, h] = bounds;
        const auto flags_ = static_cast<u32>(flags);
        if (const auto ptr = SDL_CreateWindow(title, x, y, w, h, flags_)) {
            _window.reset(ptr);
        } else {
            throw error();
        }
    }

    window(void* native_window_data)
    {
        if (const auto ptr = SDL_CreateWindowFrom(native_window_data)) {
            _window.reset(ptr);
        } else {
            throw error();
        }
    }

    window(const window&) = delete;

    window&
    operator=(const window&) = delete;

    window(window&&) = default;

    window&
    operator=(window&&) = default;

    ~window() noexcept
    {
        detail::hit_tests.erase(id());
    }

    SDL_Window*
    get_pointer() const noexcept
    {
        return _window.get();
    }

    window_flags
    flags() const noexcept
    {
        return static_cast<window_flags>(SDL_GetWindowFlags(get_pointer()));
    }

    window_id
    id() const noexcept
    {
        return static_cast<window_id>(SDL_GetWindowID(get_pointer()));;
    }

    const char*
    title() const noexcept
    {
        return SDL_GetWindowTitle(get_pointer());
    }

    void
    set_title(const char* title) noexcept
    {
        SDL_SetWindowTitle(get_pointer(), title);
    }

    point
    position() const noexcept
    {
        auto x = int();
        auto y = int();
        SDL_GetWindowPosition(get_pointer(), &x, &y);
        return point{x, y};
    }

    void
    set_position(const point& position) noexcept
    {
        SDL_SetWindowPosition(get_pointer(), position.x, position.y);
    }

    size_type
    size() const noexcept
    {
        auto width = int();
        auto height = int();
        SDL_GetWindowSize(get_pointer(), &width, &height);
        return size_type{width, height};
    }

    void
    set_size(const size_type& size) noexcept
    {
        SDL_SetWindowSize(get_pointer(), size.w, size.h);
    }

    void
    set_resizable(bool resizable) noexcept
    {
        const auto b = static_cast<SDL_bool>(resizable);
        SDL_SetWindowResizable(get_pointer(), b);
    }

    std::array<int, 4>
    border_sizes() const
    {
        const auto pwin = get_pointer();
        auto top = int();
        auto left = int();
        auto bottom = int();
        auto right = int();
        if (SDL_GetWindowBordersSize(pwin, &top, &left, &bottom, &right) < 0) {
            throw error();
        } else {
            return std::array{top, left, bottom, right};
        }
    }

    int
    top_border_size() const
    {
        const auto pwin = get_pointer();
        auto top = int();
        const auto null = nullptr;
        if (SDL_GetWindowBordersSize(pwin, &top, null, null, null) == 0) {
            return top;
        } else {
            throw error();
        }
    }

    int
    left_border_size() const
    {
        const auto pwin = get_pointer();
        auto left = int();
        const auto null = nullptr;
        if (SDL_GetWindowBordersSize(pwin, null, &left, null, null) == 0) {
            return left;
        } else {
            throw error();
        }
    }

    int
    bottom_border_size() const
    {
        const auto pwin = get_pointer();
        auto bot = int();
        const auto null = nullptr;
        if (SDL_GetWindowBordersSize(pwin, null, null, &bot, null) == 0) {
            return bot;
        } else {
            throw error();
        }
    }

    int
    right_border_size() const
    {
        const auto pwin = get_pointer();
        auto r = int();
        const auto null = nullptr;
        if (SDL_GetWindowBordersSize(pwin, null, null, null, &r) < 0) {
            return r;
        } else {
            throw error();
        }
    }

    void
    set_bordered(bool bordered) noexcept
    {
        const auto b = static_cast<SDL_bool>(bordered);
        SDL_SetWindowBordered(get_pointer(), b);
    }

    size_type
    max_size() const noexcept
    {
        auto sz = size_type();
        SDL_GetWindowMaximumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void
    set_max_size(const size_type& size) noexcept
    {
        SDL_SetWindowMaximumSize(get_pointer(), size.w, size.h);
    }

    size_type
    min_size() const noexcept
    {
        auto sz = size_type();
        SDL_GetWindowMinimumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void
    set_min_size(const size_type& size) noexcept
    {
        SDL_SetWindowMinimumSize(get_pointer(), size.w, size.h);
    }

    bool
    is_input_grabbed() const noexcept
    {
        return static_cast<bool>(SDL_GetWindowGrab(get_pointer()));
    }

    void
    set_input_grab(bool is_grabbed) noexcept
    {
        SDL_SetWindowGrab(get_pointer(), static_cast<SDL_bool>(is_grabbed));
    }

    void*
    data(const char* name) const noexcept
    {
        return SDL_GetWindowData(get_pointer(), name);
    }

    void*
    set_data(const char* name, void* user_data) noexcept
    {
        return SDL_SetWindowData(get_pointer(), name, user_data);
    }

    const surface_type&
    surface() const
    {
        static auto s = ::sdlw::detail::storage<surface_type>();
        if (const auto ptr = SDL_GetWindowSurface(get_pointer())) {
            return *new (&s) surface_type(ptr);
        } else {
            throw error();
        }
    }

    surface_type&
    surface()
    {
        static auto s = ::sdlw::detail::storage<surface_type>();
        if (const auto psurface = SDL_GetWindowSurface(get_pointer())) {
            return *new (&s) surface_type(psurface);
        } else {
            throw error();
        }
    }

    float
    opacity() const
    {
        auto opacity = float();
        if (SDL_GetWindowOpacity(get_pointer(), &opacity) == 0) {
            return opacity;
        } else {
            throw error();
        }
    }

    void
    set_opacity(float opacity)
    {
        if (SDL_SetWindowOpacity(get_pointer(), opacity) == 0) {
            throw error();
        }
    }

    display::mode
    display_mode() const
    {
        auto mode = SDL_DisplayMode();
        if (SDL_GetWindowDisplayMode(get_pointer(), &mode) == 0) {
            return display::mode(mode);
        } else {
            throw error();
        }
    }

    void
    set_display_mode(const display::mode* mode)
    {
        const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(mode);
        if (SDL_SetWindowDisplayMode(get_pointer(), pmode) < 0) {
            throw error();
        }
    }

    int
    display_index() const
    {
        const auto index = SDL_GetWindowDisplayIndex(get_pointer());
        if (index < 0) {
            throw error();
        } else {
            return index;
        }
    }

    pixels::pixel_format_type
    pixel_format_type() const
    {
        const auto format = SDL_GetWindowPixelFormat(get_pointer());
        if (format == SDL_PIXELFORMAT_UNKNOWN) {
            throw error();
        } else {
            return static_cast<pixels::pixel_format_type>(format);
        }
    }

    void
    hide() noexcept
    {
        SDL_HideWindow(get_pointer());
    }

    void
    show() noexcept
    {
        SDL_ShowWindow(get_pointer());
    }

    void
    set_fullscreen_mode(window_fullscreen_mode mode)
    {
        const auto sdl_mode = static_cast<u32>(mode);
        if (SDL_SetWindowFullscreen(get_pointer(), sdl_mode) < 0) {
            throw error();
        }
    }

    void
    maximize() noexcept
    {
        SDL_MaximizeWindow(get_pointer());
    }

    void
    minimize() noexcept
    {
        SDL_MinimizeWindow(get_pointer());
    }

    void
    restore() noexcept
    {
        SDL_RestoreWindow(get_pointer());
    }

    void
    raise() noexcept
    {
        SDL_RaiseWindow(get_pointer());
    }

    void
    set_input_focus()
    {
        if (SDL_SetWindowInputFocus(get_pointer()) < 0) {
            throw error();
        }
    }

    void
    update_surface()
    {
        if (SDL_UpdateWindowSurface(get_pointer()) < 0) {
            throw error();
        }
    }

    void
    update_surface_areas(span<const rectangle> areas)
    {
        const auto pwin = get_pointer();
        const auto sz = static_cast<int>(areas.size());
        if (SDL_UpdateWindowSurfaceRects(pwin, areas.data(), sz) < 0) {
            throw error();
        }
    }

    void
    set_icon(const ::sdlw::video::surface& icon) noexcept
    {
        SDL_SetWindowIcon(get_pointer(), icon.get_pointer());
    }

    const renderer_type&
    renderer() const;

    renderer_type&
    renderer();

    void
    set_modal(const window& modal)
    {
        if (SDL_SetWindowModalFor(modal.get_pointer(), get_pointer()) < 0) {
            throw error();
        }
    }

    void
    set_hit_test(std::function<hit_test> ht)
    {
        using detail::hit_tests;
        if (ht) {
            const auto elem = std::make_pair(id(), std::move(ht));
            const auto [it, success] = hit_tests.emplace(std::move(elem));
            SDLW_ASSERT(success);
            constexpr auto callback = sdl_hit_test_callback;
            auto& f = it->second;
            if (SDL_SetWindowHitTest(get_pointer(), callback, &f) < 0) {
                throw error();
            }
        } else {
            if (SDL_SetWindowHitTest(get_pointer(), nullptr, nullptr) == 0) {
                hit_tests.erase(id());
            } else {
                throw error();
            }
        }
    }
};

inline
bool
operator==(const window& lhs, const window& rhs) noexcept
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline
bool
operator!=(const window& lhs, const window& rhs) noexcept
{
    return !(lhs == rhs);
}

inline
window&
get_window(window_id id)
{
    static auto s = ::sdlw::detail::storage<window>();
    if (const auto ptr = SDL_GetWindowFromID(static_cast<u32>(id))) {
        return *new (&s) window(ptr);
    } else {
        throw error();
    }
}

inline
window&
get_grabbed_window()
{
    static auto s = ::sdlw::detail::storage<window>();
    if (const auto ptr = SDL_GetGrabbedWindow()) {
        return *new (&s) window(ptr);
    } else {
        throw error();
    }
}

} // namespace sdlw::video
