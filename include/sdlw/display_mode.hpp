#pragma once

#include <SDL2/SDL.h>

#include <sdlw/types.hpp>
#include <sdlw/pixel_format_type.hpp>
#include <sdlw/size.hpp>

namespace sdlw {

class display_mode {
    SDL_DisplayMode _display_mode;

public:
    constexpr
    explicit
    operator SDL_DisplayMode() const noexcept
    {
        return _display_mode;
    }

    display_mode() noexcept = default;

    constexpr
    explicit
    display_mode(const SDL_DisplayMode& display_mode) noexcept
        : _display_mode(display_mode)
    {
    }

    constexpr
    pixel_format_type
    format() const noexcept
    {
        return static_cast<pixel_format_type>(_display_mode.format);
    }

    constexpr
    void
    set_format(pixel_format_type format) noexcept
    {
        _display_mode.format = static_cast<u32>(format);
    }

    constexpr
    sdlw::size
    size() const noexcept
    {
        return sdlw::size{_display_mode.w, _display_mode.h};
    }

    constexpr
    void
    set_size(const sdlw::size& size) noexcept
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

} // namespace sdlw
