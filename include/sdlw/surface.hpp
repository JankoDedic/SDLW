#pragma once

#include <memory>

#include <SDL2/SDL_surface.h>

#include <sdlw/utility.hpp>

namespace sdlw {

class surface_ref {
public:
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_surface);
    }

    surface_ref() = default;

    explicit surface_ref(SDL_Surface* s) noexcept
        : _surface{s}
    {}

    auto get_pointer() const noexcept -> SDL_Surface*
    {
        return _surface;
    }

    auto format() const noexcept -> pixel_format_ref
    {
        return pixel_format_ref{_surface->format};
    }

    auto size() const noexcept -> sdlw::size
    {
        return {_surface->w, _surface->h};
    }

    auto pitch() const noexcept -> int
    {
        return _surface->pitch;
    }

    auto clip() const noexcept -> rect
    {
        return _surface->clip_rect;
    }

protected:
    SDL_Surface* _surface = nullptr;
};

class surface : public surface_ref {
public:
    using surface_ref::surface_ref;

    surface(const surface&) = delete;

    auto operator=(const surface&) -> surface& = delete;

    surface(surface&& other) noexcept
        : surface_ref{std::exchange(other._surface, nullptr)}
    {}

    auto operator=(surface&& other) noexcept -> surface&
    {
        SDL_FreeSurface(_surface);
        _surface = std::exchange(other._surface, nullptr);
        return *this;
    }

    ~surface() noexcept
    {
        SDL_FreeSurface(_surface);
        _surface = nullptr;
    }
};

inline auto operator==(const surface& lhs, const surface& rhs) noexcept -> bool
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline auto operator!=(const surface& lhs, const surface& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

} // namespace sdlw
