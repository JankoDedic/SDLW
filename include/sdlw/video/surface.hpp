#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include <sdlw/utility.hpp>

namespace sdlw::video {

class surface {
    using deleter = detail::make_functor<SDL_FreeSurface>;

    std::unique_ptr<SDL_Surface, deleter> _surface;

public:
    surface() noexcept = default;

    surface(SDL_Surface* pointer) noexcept
        : _surface(pointer)
    {
    }

    SDL_Surface*
    get_pointer() const noexcept
    {
        return _surface.get();
    }
};

inline
bool
operator==(const surface& lhs, const surface& rhs) noexcept
{
    return lhs.get_pointer() == rhs.get_pointer();
}

inline
bool
operator!=(const surface& lhs, const surface& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace sdlw::video
