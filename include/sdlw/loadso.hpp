#pragma once

#include <memory>

#include <SDL2/SDL_loadso.h>

#include <sdlw/error.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

class shared_object {
public:
    explicit shared_object(const char* sofile)
        : _shared_object{SDL_LoadObject(sofile)}
    {
        if (!_shared_object) throw error{};
    }

    template<typename Function>
    auto load_function(const char* name) const -> Function
    {
        if (const auto fp = SDL_LoadFunction(_shared_object.get(), name)) {
            return reinterpret_cast<Function*>(fp);
        } else {
            throw error{};
        }
    }

private:
    std::unique_ptr<void, detail::make_functor<SDL_UnloadObject>> _shared_object;
};

} // namespace sdl
