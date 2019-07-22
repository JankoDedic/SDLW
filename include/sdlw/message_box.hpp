#pragma once

#include <SDL2/SDL_messagebox.h>

#include <sdlw/video.hpp>

namespace sdlw {

enum class message_box_flag : u32 {
    error       = SDL_MESSAGEBOX_ERROR,
    warning     = SDL_MESSAGEBOX_WARNING,
    information = SDL_MESSAGEBOX_INFORMATION
};

class message_box {
};

inline
void
show_simple_message_box(
    message_box_flag flag,
    const char* title,
    const char* message)
{
    const auto flag_ = static_cast<u32>(flag);
    if (SDL_ShowSimpleMessageBox(flag_, title, message, nullptr) < 0) {
        throw error();
    }
}

inline
void
show_simple_message_box(
    message_box_flag flag,
    const char* title,
    const char* message,
    const window& parent)
{
    const auto flag_ = static_cast<u32>(flag);
    const auto pwindow = parent.get_pointer();
    if (SDL_ShowSimpleMessageBox(flag_, title, message, pwindow) < 0) {
        throw error();
    }
}

} // namespace sdlw
