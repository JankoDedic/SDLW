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

class window_ref;
struct window;   // for hit_test
class renderer; // for window::renderer

enum class window_flags : u32 {
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

SDLW_DETAIL_DEFINE_FLAG_OPERATIONS(window_flags)

enum class window_id : u32 {};

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

using hit_test = hit_test_result(window &, const point &, void *);

struct window_border_sizes {
    int top;
    int left;
    int bottom;
    int right;
};

class window_ref {
protected:
    SDL_Window *_pwindow;

public:
    using size_type = size;
    using renderer_type = renderer;
    using surface_type = surface;

    window_ref() noexcept : _pwindow(nullptr) {};

    window_ref(SDL_Window *pointer) noexcept : _pwindow(pointer) {}

    auto get_pointer() const noexcept -> SDL_Window * {
        return _pwindow;
    }

    auto flags() const noexcept -> window_flags {
        return static_cast<window_flags>(SDL_GetWindowFlags(get_pointer()));
    }

    auto id() const noexcept -> window_id {
        return static_cast<window_id>(SDL_GetWindowID(get_pointer()));;
    }

    auto title() const noexcept -> const char * {
        return SDL_GetWindowTitle(get_pointer());
    }

    void set_title(const char *title) noexcept {
        SDL_SetWindowTitle(get_pointer(), title);
    }

    auto position() const noexcept -> point {
        auto x = int();
        auto y = int();
        SDL_GetWindowPosition(get_pointer(), &x, &y);
        return point{x, y};
    }

    void set_position(const point &position) noexcept {
        SDL_SetWindowPosition(get_pointer(), position.x, position.y);
    }

    auto size() const noexcept -> size_type {
        auto width = int();
        auto height = int();
        SDL_GetWindowSize(get_pointer(), &width, &height);
        return size_type{width, height};
    }

    void set_size(const size_type &size) noexcept {
        SDL_SetWindowSize(get_pointer(), size.w, size.h);
    }

    void set_resizable(bool resizable) noexcept {
        SDL_SetWindowResizable(get_pointer(), static_cast<SDL_bool>(resizable));
    }

    auto border_sizes() const -> window_border_sizes {
        const auto pwin = get_pointer();
        auto wbsizes = window_border_sizes();
        if (SDL_GetWindowBordersSize(pwin, &wbsizes.top, &wbsizes.left, &wbsizes.bottom, &wbsizes.right) < 0) {
            throw error();
        } else {
            return wbsizes;
        }
    }

    auto top_border_size() const -> int {
        const auto pwin = get_pointer();
        auto top = int();
        if (SDL_GetWindowBordersSize(pwin, &top, nullptr, nullptr, nullptr) == 0) {
            return top;
        } else {
            throw error();
        }
    }

    auto left_border_size() const -> int {
        const auto pwin = get_pointer();
        auto left = int();
        if (SDL_GetWindowBordersSize(pwin, nullptr, &left, nullptr, nullptr) == 0) {
            return left;
        } else {
            throw error();
        }
    }

    auto bottom_border_size() const -> int {
        const auto pwin = get_pointer();
        auto bottom = int();
        if (SDL_GetWindowBordersSize(pwin, nullptr, nullptr, &bottom, nullptr) == 0) {
            return bottom;
        } else {
            throw error();
        }
    }

    auto right_border_size() const -> int {
        const auto pwin = get_pointer();
        auto right = int();
        if (SDL_GetWindowBordersSize(pwin, nullptr, nullptr, nullptr, &right) < 0) {
            return right;
        } else {
            throw error();
        }
    }

    void set_bordered(bool bordered) noexcept {
        SDL_SetWindowBordered(get_pointer(), static_cast<SDL_bool>(bordered));
    }

    auto max_size() const noexcept -> size_type {
        auto sz = size_type();
        SDL_GetWindowMaximumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_max_size(const size_type &size) noexcept {
        SDL_SetWindowMaximumSize(get_pointer(), size.w, size.h);
    }

    auto min_size() const noexcept -> size_type {
        auto sz = size_type();
        SDL_GetWindowMinimumSize(get_pointer(), &sz.w, &sz.h);
        return sz;
    }

    void set_min_size(const size_type &size) noexcept {
        SDL_SetWindowMinimumSize(get_pointer(), size.w, size.h);
    }

    auto is_input_grabbed() const noexcept -> bool {
        return static_cast<bool>(SDL_GetWindowGrab(get_pointer()));
    }

    void set_input_grab(bool is_grabbed) noexcept {
        SDL_SetWindowGrab(get_pointer(), static_cast<SDL_bool>(is_grabbed));
    }

    auto data(const char *name) const noexcept -> void * {
        return SDL_GetWindowData(get_pointer(), name);
    }

    auto set_data(const char *name, void *user_data) noexcept -> void * {
        return SDL_SetWindowData(get_pointer(), name, user_data);
    }

    auto surface() const -> const surface_type & {
        static auto s = ::sdlw::detail::storage<surface_type>();
        if (const auto ptr = SDL_GetWindowSurface(get_pointer())) {
            return *new (&s) surface_type(ptr);
        } else {
            throw error();
        }
    }

    auto surface() -> surface_type & {
        static auto s = ::sdlw::detail::storage<surface_type>();
        if (const auto psurface = SDL_GetWindowSurface(get_pointer())) {
            return *new (&s) surface_type(psurface);
        } else {
            throw error();
        }
    }

    auto opacity() const -> float {
        auto opacity = float();
        if (SDL_GetWindowOpacity(get_pointer(), &opacity) == 0) {
            return opacity;
        } else {
            throw error();
        }
    }

    void set_opacity(float opacity) {
        if (SDL_SetWindowOpacity(get_pointer(), opacity) == 0) {
            throw error();
        }
    }

    auto display_mode() const -> display::mode {
        auto mode = SDL_DisplayMode();
        if (SDL_GetWindowDisplayMode(get_pointer(), &mode) == 0) {
            return display::mode(mode);
        } else {
            throw error();
        }
    }

    void set_display_mode(const display::mode *mode) {
        const auto pmode = reinterpret_cast<const SDL_DisplayMode *>(mode);
        if (SDL_SetWindowDisplayMode(get_pointer(), pmode) < 0) {
            throw error();
        }
    }

    auto display_index() const -> int {
        const auto index = SDL_GetWindowDisplayIndex(get_pointer());
        if (index < 0) {
            throw error();
        } else {
            return index;
        }
    }

    auto pixel_format_type() const -> pixels::pixel_format_type {
        const auto format = SDL_GetWindowPixelFormat(get_pointer());
        if (format == SDL_PIXELFORMAT_UNKNOWN) {
            throw error();
        } else {
            return static_cast<pixels::pixel_format_type>(format);
        }
    }

    void hide() noexcept {
        SDL_HideWindow(get_pointer());
    }

    void show() noexcept {
        SDL_ShowWindow(get_pointer());
    }

    void set_fullscreen_mode(window_fullscreen_mode mode) {
        const auto sdl_mode = static_cast<u32>(mode);
        if (SDL_SetWindowFullscreen(get_pointer(), sdl_mode) < 0) {
            throw error();
        }
    }

    void maximize() noexcept {
        SDL_MaximizeWindow(get_pointer());
    }

    void minimize() noexcept {
        SDL_MinimizeWindow(get_pointer());
    }

    void restore() noexcept {
        SDL_RestoreWindow(get_pointer());
    }

    void raise() noexcept {
        SDL_RaiseWindow(get_pointer());
    }

    void set_input_focus() {
        if (SDL_SetWindowInputFocus(get_pointer()) < 0) {
            throw error();
        }
    }

    void update_surface() {
        if (SDL_UpdateWindowSurface(get_pointer()) < 0) {
            throw error();
        }
    }

    void update_surface_areas(span<const rectangle> areas) {
        const auto pwin = get_pointer();
        const auto sz = static_cast<int>(areas.size());
        if (SDL_UpdateWindowSurfaceRects(pwin, areas.data(), sz) < 0) {
            throw error();
        }
    }

    void set_icon(const ::sdlw::video::surface &icon) noexcept {
        SDL_SetWindowIcon(get_pointer(), icon.get_pointer());
    }

    auto renderer() const -> const renderer_type &;
    auto renderer() -> renderer_type &;

    void set_modal(window_ref modal) {
        if (SDL_SetWindowModalFor(modal.get_pointer(), get_pointer()) < 0) {
            throw error();
        }
    }

    template<hit_test HitTest>
    void set_hit_test(void *data = nullptr) {
        /* static_assert(std::is_invocable_r_v<hit_test_result, decltype(HitTest), window &, const point &, void *>); */
        constexpr auto callback = [] (SDL_Window *pwin, const SDL_Point *area, void *data) -> SDL_HitTestResult {
            auto win_storage = ::sdlw::detail::storage<window>();
            auto &win = *new (&win_storage) window(pwin);
            const auto result = HitTest(win, *area, data);
            return static_cast<SDL_HitTestResult>(result);
        };
        if (SDL_SetWindowHitTest(get_pointer(), callback, data) < 0) {
            throw error();
        }
    }
};

struct window : window_ref {
    static constexpr auto centered = SDL_WINDOWPOS_CENTERED;
    static constexpr auto undefined = SDL_WINDOWPOS_UNDEFINED;
    static constexpr auto pos_centered = point{centered, centered};
    static constexpr auto pos_undefined = point{undefined, undefined};

    static constexpr auto centered_on_display(int display_index) noexcept -> int {
        return SDL_WINDOWPOS_CENTERED_DISPLAY(display_index);
    }

    static constexpr auto undefined_on_display(int display_index) noexcept -> int {
        return SDL_WINDOWPOS_UNDEFINED_DISPLAY(display_index);
    }

    static constexpr auto pos_centered_on_display(int display_index) noexcept -> point {
        const auto value = centered_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto pos_undefined_on_display(int display_index) noexcept -> point {
        const auto value = undefined_on_display(display_index);
        return point{value, value};
    }

    static constexpr auto is_centered(int coordinate) noexcept -> bool {
        return SDL_WINDOWPOS_ISCENTERED(coordinate);
    }

    static constexpr auto is_undefined(int coordinate) noexcept -> bool {
        return SDL_WINDOWPOS_ISUNDEFINED(coordinate);
    }

    static constexpr auto is_pos_centered(const point &position) noexcept -> bool {
        return is_centered(position.x) && is_centered(position.y);
    }

    static constexpr auto is_pos_undefined(const point &position) noexcept -> bool {
        return is_undefined(position.x) && is_undefined(position.y);
    }

    using window_ref::window_ref;

    window() noexcept = default;

    window(const char *title, const rectangle &bounds, window_flags flags)
    {
        /* const auto &[x, y, w, h] = bounds; */
        const auto flags_ = static_cast<u32>(flags);
        if (const auto ptr = SDL_CreateWindow(title, bounds.x, bounds.y, bounds.w, bounds.h, flags_)) {
            _pwindow = ptr;
        } else {
            throw error();
        }
    }

    window(void *native_window_data)
    {
        if (const auto ptr = SDL_CreateWindowFrom(native_window_data)) {
            _pwindow = ptr;
        } else {
            throw error();
        }
    }

    window(const window &) = delete;
    auto operator=(const window &) -> window & = delete;

    window(window &&other) noexcept
        : window_ref(std::exchange(other._pwindow, nullptr))
    {
    }

    auto operator=(window &&other) noexcept -> window & {
        SDL_DestroyWindow(_pwindow);
        _pwindow = std::exchange(other._pwindow, nullptr);
        return *this;
    }

    ~window() noexcept
    {
        SDL_DestroyWindow(get_pointer());
        _pwindow = nullptr;
    }
};

inline auto operator==(const window &lhs, const window &rhs) noexcept -> bool {
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const window &lhs, const window &rhs) noexcept -> bool {
    return !(lhs == rhs);
}

inline auto get_window(window_id id) -> window_ref {
    /* static auto s = ::sdlw::detail::storage<window>(); */
    /* if (const auto ptr = SDL_GetWindowFromID(static_cast<u32>(id))) { */
    /*     return *new (&s) window(ptr); */
    /* } else { */
    /*     throw error(); */
    /* } */

    if (const auto ptr = SDL_GetWindowFromID(static_cast<u32>(id))) {
        return window_ref(ptr);
    } else {
        throw error();
    }
}

inline auto get_grabbed_window() -> window_ref {
    /* static auto s = ::sdlw::detail::storage<window>(); */
    /* if (const auto ptr = SDL_GetGrabbedWindow()) { */
    /*     return *new (&s) window(ptr); */
    /* } else { */
    /*     throw error(); */
    /* } */

    if (const auto ptr = SDL_GetGrabbedWindow()) {
        return window_ref(ptr);
    } else {
        throw error();
    }
}

} // namespace sdlw::video
