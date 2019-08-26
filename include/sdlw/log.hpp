#pragma once

#include <type_traits>

#include <SDL2/SDL_log.h>

namespace sdl {

// clang-format off
enum class log_category {
    application = SDL_LOG_CATEGORY_APPLICATION,
    error       = SDL_LOG_CATEGORY_ERROR,
    assert      = SDL_LOG_CATEGORY_ASSERT,
    system      = SDL_LOG_CATEGORY_SYSTEM,
    audio       = SDL_LOG_CATEGORY_AUDIO,
    video       = SDL_LOG_CATEGORY_VIDEO,
    render      = SDL_LOG_CATEGORY_RENDER,
    input       = SDL_LOG_CATEGORY_INPUT,
    test        = SDL_LOG_CATEGORY_TEST,
    custom      = SDL_LOG_CATEGORY_CUSTOM
};

enum class log_priority : std::underlying_type_t<SDL_LogPriority> {
    verbose  = SDL_LOG_PRIORITY_VERBOSE,
    debug    = SDL_LOG_PRIORITY_DEBUG,
    info     = SDL_LOG_PRIORITY_INFO,
    warn     = SDL_LOG_PRIORITY_WARN,
    error    = SDL_LOG_PRIORITY_ERROR,
    critical = SDL_LOG_PRIORITY_CRITICAL
};
// clang-format on

template<typename... Args>
void log(const char* fmt, Args... args) noexcept
{
    SDL_Log(fmt, args...);
}

template<typename... Args>
void log(log_category c, log_priority p, const char* fmt, Args... args) noexcept
{
    SDL_LogMessage(static_cast<int>(c), static_cast<SDL_LogPriority>(p), fmt, args...);
}

template<typename... Args>
void log_critical(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogCritical(static_cast<int>(category), fmt, args...);
}

template<typename... Args>
void log_debug(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogDebug(static_cast<int>(category), fmt, args...);
}

template<typename... Args>
void log_error(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogError(static_cast<int>(category), fmt, args...);
}

template<typename... Args>
void log_info(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogInfo(static_cast<int>(category), fmt, args...);
}

template<typename... Args>
void log_verbose(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogVerbose(static_cast<int>(category), fmt, args...);
}

template<typename... Args>
void log_warn(log_category category, const char* fmt, Args... args) noexcept
{
    SDL_LogWarn(static_cast<int>(category), fmt, args...);
}

inline auto get_log_priority(log_category category) noexcept -> log_priority
{
    return static_cast<log_priority>(SDL_LogGetPriority(static_cast<int>(category)));
}

inline void set_log_priority(log_category c, log_priority p) noexcept
{
    SDL_LogSetPriority(static_cast<int>(c), static_cast<SDL_LogPriority>(p));
}

inline void set_log_priority(log_priority p) noexcept
{
    SDL_LogSetAllPriority(static_cast<SDL_LogPriority>(p));
}

inline void reset_log_priorities() noexcept
{
    SDL_LogResetPriorities();
}

class log_output_function {
public:
    using function_type = void(log_category, log_priority, const char* message);

    log_output_function(function_type* f) noexcept
        : _userdata{f}
    {
        _function = [](void* userdata, int category, SDL_LogPriority priority, const char* message) {
            const auto func = reinterpret_cast<function_type*>(userdata);
            func(log_category{category}, log_priority{priority}, message);
        };
    }

    template<typename Callable>
    log_output_function(Callable& c) noexcept
        : _userdata{&c}
    {
        static_assert(std::is_invocable_r_v<void, Callable, log_category, log_priority, const char*>);
        _function = [](void* userdata, int category, SDL_LogPriority priority, const char* message) {
            auto& func = *static_cast<Callable*>(userdata);
            func(log_category{category}, log_priority{priority}, message);
        };
    }

    log_output_function(SDL_LogOutputFunction f, void* userdata) noexcept
        : _function{f}
        , _userdata{userdata}
    {}

    auto function() const noexcept -> SDL_LogOutputFunction
    {
        return _function;
    }

    auto userdata() const noexcept -> void*
    {
        return _userdata;
    }

    void operator()(log_category category, log_priority priority, const char* message) noexcept
    {
        _function(_userdata, static_cast<int>(category), static_cast<SDL_LogPriority>(priority), message);
    }

    friend auto operator==(const log_output_function& lhs, const log_output_function& rhs) noexcept
    {
        return lhs.function() == rhs.function() && lhs.userdata() == rhs.userdata();
    }

    friend auto operator!=(const log_output_function& lhs, const log_output_function& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    static void set(log_output_function f) noexcept
    {
        SDL_LogSetOutputFunction(f._function, f._userdata);
    }

    static auto get() noexcept -> log_output_function
    {
        auto callback = SDL_LogOutputFunction{};
        auto userdata = static_cast<void*>(nullptr);
        SDL_LogGetOutputFunction(&callback, &userdata);
        return {callback, userdata};
    }

private:
    SDL_LogOutputFunction _function = nullptr;
    void* _userdata = nullptr;
};

} // namespace sdl
