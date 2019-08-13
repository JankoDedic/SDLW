#pragma once

#include <SDL2/SDL_messagebox.h>

#include <sdlw/video.hpp>

namespace sdl {

// clang-format off

enum class message_box_flag : u32 {
    error       = SDL_MESSAGEBOX_ERROR,
    warning     = SDL_MESSAGEBOX_WARNING,
    information = SDL_MESSAGEBOX_INFORMATION
};

// clang-format on

class message_box {};

inline void show_simple_message_box(message_box_flag f, const char* title, const char* message)
{
    const auto flag = static_cast<u32>(f);
    if (SDL_ShowSimpleMessageBox(flag, title, message, nullptr) < 0) {
        throw error{};
    }
}

inline void show_simple_message_box(message_box_flag f, const char* title, const char* message, const window& parent)
{
    const auto flag = static_cast<u32>(f);
    const auto pwindow = parent.get_pointer();
    if (SDL_ShowSimpleMessageBox(flag, title, message, pwindow) < 0) {
        throw error{};
    }
}

} // namespace sdl
