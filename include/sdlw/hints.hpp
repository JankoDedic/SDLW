#pragma once

#include <type_traits>

#include <SDL2/SDL_hints.h>

namespace sdl {

enum class hint_priority : std::underlying_type_t<SDL_HintPriority> {
    default_ = SDL_HINT_DEFAULT,
    normal = SDL_HINT_NORMAL,
    override_ = SDL_HINT_OVERRIDE
};

using hint_callback = void(const char*, const char*, const char*);

namespace detail {

inline void sdl_hint_callback(void* userdata, const char* name, const char* old_value, const char* new_value)
{
    const auto func = reinterpret_cast<hint_callback*>(userdata);
    func(name, old_value, new_value);
}

template<typename HintCallback>
struct hint_callback_generator {
    static void callback(void* userdata, const char* name, const char* old_value, const char* new_value)
    {
        auto& func = *reinterpret_cast<HintCallback*>(userdata);
        func(name, old_value, new_value);
    }
};

} // namespace detail

inline void add_hint_callback(const char* name, hint_callback* cb) noexcept
{
    SDL_AddHintCallback(name, detail::sdl_hint_callback, reinterpret_cast<void*>(cb));
}

inline void del_hint_callback(const char* name, hint_callback* cb) noexcept
{
    SDL_DelHintCallback(name, detail::sdl_hint_callback, reinterpret_cast<void*>(cb));
}

template<typename HintCallback>
void add_hint_callback(const char* name, HintCallback& cb) noexcept
{
    static_assert(std::is_invocable_r_v<void, HintCallback, const char*, const char*, const char*>);
    SDL_AddHintCallback(name, detail::hint_callback_generator<HintCallback>::callback, &cb);
}

template<typename HintCallback>
void del_hint_callback(const char* name, HintCallback& cb) noexcept
{
    static_assert(std::is_invocable_r_v<void, HintCallback, const char*, const char*, const char*>);
    SDL_DelHintCallback(name, detail::hint_callback_generator<HintCallback>::callback, &cb);
}

inline void clear_hints() noexcept
{
    SDL_ClearHints();
}

inline auto get_hint(const char* name) noexcept -> const char*
{
    return SDL_GetHint(name);
}

inline auto get_hint_boolean(const char* name, bool default_value) noexcept -> bool
{
    return static_cast<bool>(SDL_GetHintBoolean(name, static_cast<SDL_bool>(default_value)));
}

inline auto set_hint(const char* name, const char* value) noexcept -> bool
{
    return static_cast<bool>(SDL_SetHint(name, value));
}

inline auto set_hint(const char* name, const char* value, hint_priority priority) noexcept -> bool
{
    return static_cast<bool>(SDL_SetHintWithPriority(name, value, static_cast<SDL_HintPriority>(priority)));
}

} // namespace sdl
