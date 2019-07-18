#pragma once

#include <array>

#include <SDL2/SDL.h>

#include <sdlw/video/rectangle.hpp>
#include <sdlw/video/window.hpp>
#include <sdlw/video/display/mode.hpp>

namespace sdlw {

inline
display_mode
closest_display_mode(int display_index, const display_mode& m)
{
    const auto pmode = reinterpret_cast<const SDL_DisplayMode*>(&m);
    auto closest = SDL_DisplayMode();
    if (SDL_GetClosestDisplayMode(display_index, pmode, &closest)) {
        return display_mode(closest);
    } else {
        throw error();
    }
}

inline
display_mode
current_display_mode(int display_index)
{
    auto current = SDL_DisplayMode();
    if (SDL_GetCurrentDisplayMode(display_index, &current) == 0) {
        return display_mode(current);
    } else {
        throw error();
    }
}

inline
display_mode
desktop_display_mode(int display_index)
{
    auto desktop = SDL_DisplayMode();
    if (SDL_GetDesktopDisplayMode(display_index, &desktop) == 0) {
        return display_mode(desktop);
    } else {
        throw error();
    }
}

inline
rectangle
display_bounds(int display_index)
{
    auto bounds = rectangle();
    if (SDL_GetDisplayBounds(display_index, &bounds) == 0) {
        return bounds;
    } else {
        throw error();
    }
}

inline
std::array<float, 3>
display_dpi(int display_index)
{
    auto ddpi = float();
    auto hdpi = float();
    auto vdpi = float();
    if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) == 0) {
        return std::array{ddpi, hdpi, vdpi};
    } else {
        throw error();
    }
}

inline
float
display_diagonal_dpi(int display_index)
{
    auto ddpi = float();
    if (SDL_GetDisplayDPI(display_index, &ddpi, nullptr, nullptr) == 0) {
        return ddpi;
    } else {
        throw error();
    }
}

inline
float
display_horizontal_dpi(int display_index)
{
    auto hdpi = float();
    if (SDL_GetDisplayDPI(display_index, nullptr, &hdpi, nullptr) == 0) {
        return hdpi;
    } else {
        throw error();
    }
}

inline
float
display_vertical_dpi(int display_index)
{
    auto vdpi = float();
    if (SDL_GetDisplayDPI(display_index, nullptr, nullptr, &vdpi) == 0) {
        return vdpi;
    } else {
        throw error();
    }
}

inline
display_mode
get_display_mode(int display_index, int mode_index)
{
    auto m = SDL_DisplayMode();
    if (SDL_GetDisplayMode(display_index, mode_index, &m) == 0) {
        return display_mode(m);
    } else {
        throw error();
    }
}

inline
const char*
get_display_name(int display_index) noexcept
{
    return SDL_GetDisplayName(display_index);
}

inline
rectangle
display_usable_bounds(int display_index)
{
    auto rect = rectangle();
    if (SDL_GetDisplayUsableBounds(display_index, &rect) == 0) {
        return rect;
    } else {
        throw error();
    }
}

inline
int
num_display_modes(int display_index)
{
    if (const auto count = SDL_GetNumDisplayModes(display_index); count < 0) {
        throw error();
    } else {
        return count;
    }
}

inline
int
num_video_displays()
{
    if (const auto cnt = SDL_GetNumVideoDisplays(); cnt < 0) {
        throw error();
    } else {
        return cnt;
    }
}

inline
float
display_brightness(const window& win) noexcept
{
    return SDL_GetWindowBrightness(win.get_pointer());
}

inline
void
set_display_brightness(const window& win, float brightness)
{
    if (SDL_SetWindowBrightness(win.get_pointer(), brightness) < 0) {
        throw error();
    }
}

inline
void
get_display_gamma_ramp(
    const window& win,
    span<u16, 256> red,
    span<u16, 256> green,
    span<u16, 256> blue)
{
    const auto r = red.data();
    const auto g = green.data();
    const auto b = blue.data();
    if (SDL_GetWindowGammaRamp(win.get_pointer(), r, g, b) < 0) {
        throw error();
    }
}

inline
void
set_display_gamma_ramp(
    const window& win,
    span<const u16, 256> red,
    span<const u16, 256> green,
    span<const u16, 256> blue)
{
    const auto r = red.data();
    const auto g = green.data();
    const auto b = blue.data();
    if (SDL_SetWindowGammaRamp(win.get_pointer(), r, g, b) < 0) {
        throw error();
    }
}

} // namespace sdlw
