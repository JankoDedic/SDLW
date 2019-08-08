#pragma once

#include <memory>

#include <SDL2/SDL_surface.h>

#include <sdlw/utility.hpp>

namespace sdlw {

class surface_ref {
protected:
    SDL_Surface* _surface = nullptr;

public:
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(_surface);
    }

    surface_ref() noexcept
        : _surface{nullptr}
    {}

    explicit surface_ref(SDL_Surface* s) noexcept
        : _surface{s}
    {}

    auto get_pointer() const noexcept -> SDL_Surface*
    {
        return _surface;
    }
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
