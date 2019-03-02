#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>
#include <sdlw/video/pixels/pixel_format_type.hpp>
#include <sdlw/video/size.hpp>

namespace sdlw::video::display {

class mode {
    SDL_DisplayMode _display_mode;

public:
    constexpr
    explicit
    operator SDL_DisplayMode() const noexcept
    {
        return _display_mode;
    }

    mode() noexcept = default;

    constexpr
    explicit
    mode(const SDL_DisplayMode& display_mode) noexcept
        : _display_mode(display_mode)
    {
    }

    constexpr
    pixels::pixel_format_type
    format() const noexcept
    {
        return static_cast<pixels::pixel_format_type>(_display_mode.format);
    }

    constexpr
    void
    set_format(pixels::pixel_format_type format) noexcept
    {
        _display_mode.format = static_cast<u32>(format);
    }

    constexpr
    sdlw::video::size
    size() const noexcept
    {
        return sdlw::video::size{_display_mode.w, _display_mode.h};
    }

    constexpr
    void
    set_size(const sdlw::video::size& size) noexcept
    {
        _display_mode.w = size.w;
        _display_mode.h = size.h;
    }

    constexpr
    const int&
    refresh_rate() const noexcept
    {
        return _display_mode.refresh_rate;
    }

    constexpr
    int&
    refresh_rate() noexcept
    {
        return _display_mode.refresh_rate;
    }

    constexpr
    void* const&
    driver_data() const noexcept
    {
        return _display_mode.driverdata;
    }

    constexpr
    void*&
    driver_data() noexcept
    {
        return _display_mode.driverdata;
    }
};

} // namespace sdlw::video::display
