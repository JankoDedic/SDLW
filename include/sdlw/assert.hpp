#pragma once

#include <type_traits>

#include <SDL2/SDL_assert.h>

// clang-format off

#define SDL_ASSERT          SDL_assert
#define SDL_ASSERT_RELEASE  SDL_assert_release
#define SDL_ASSERT_PARANOID SDL_assert_paranoid

namespace sdl {

enum class assert_state : std::underlying_type_t<SDL_AssertState> {
    retry         = SDL_ASSERTION_RETRY,
    break_        = SDL_ASSERTION_BREAK,
    abort         = SDL_ASSERTION_ABORT,
    ignore        = SDL_ASSERTION_IGNORE,
    always_ignore = SDL_ASSERTION_ALWAYS_IGNORE
};

class assert_data_ref {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_assert_data); }
    explicit assert_data_ref(const SDL_AssertData* ad) noexcept : _assert_data{ad} {}
    auto get_pointer() const noexcept -> const SDL_AssertData* { return _assert_data; }

    auto always_ignore() const noexcept -> bool            { return static_cast<bool>(_assert_data->always_ignore); }
    auto trigger_count() const noexcept -> unsigned        { return _assert_data->trigger_count;                    }
    auto condition()     const noexcept -> const char*     { return _assert_data->condition;                        }
    auto filename()      const noexcept -> const char*     { return _assert_data->filename;                         }
    auto line_number()   const noexcept -> int             { return _assert_data->linenum;                          }
    auto function()      const noexcept -> const char*     { return _assert_data->function;                         }
    auto next()          const noexcept -> assert_data_ref { return assert_data_ref{_assert_data->next};            }

private:
    const SDL_AssertData* _assert_data = nullptr;
};

// clang-format on

class assertion_handler {
public:
    using function_type = assert_state(assert_data_ref);

    assertion_handler(function_type* f) noexcept
        : _userdata{reinterpret_cast<void*>(f)}
    {
        _function = [](const SDL_AssertData* data, void* userdata) -> SDL_AssertState {
            const auto func = reinterpret_cast<function_type*>(userdata);
            const auto result = func(assert_data_ref{data});
            return static_cast<SDL_AssertState>(result);
        };
    }

    template<typename Callable>
    assertion_handler(Callable& c) noexcept
        : _userdata{&c}
    {
        static_assert(std::is_invocable_r_v<assert_state, Callable, assert_data_ref>);
        _function = [](const SDL_AssertData* data, void* userdata) -> SDL_AssertState {
            auto& func = *static_cast<Callable*>(userdata);
            const auto result = func(assert_data_ref{data});
            return static_cast<SDL_AssertState>(result);
        };
    }

    assertion_handler(SDL_AssertionHandler assertion_handler, void* userdata) noexcept
        : _function{assertion_handler}
        , _userdata{userdata}
    {}

    auto function() const noexcept -> SDL_AssertionHandler
    {
        return _function;
    }

    auto userdata() const noexcept -> void*
    {
        return _userdata;
    }

    auto operator()(assert_data_ref ad) const noexcept -> assert_state
    {
        _function(ad.get_pointer(), _userdata);
    }

    friend auto operator==(const assertion_handler& lhs, const assertion_handler& rhs) noexcept -> bool
    {
        return lhs.function() == rhs.function() && lhs.userdata() == rhs.userdata();
    }

    friend auto operator!=(const assertion_handler& lhs, const assertion_handler& rhs) noexcept -> bool
    {
        return !(lhs == rhs);
    }

    static void set(assertion_handler handler) noexcept
    {
        SDL_SetAssertionHandler(handler.function(), handler.userdata());
    }

    static auto get() noexcept -> assertion_handler
    {
        auto userdata = static_cast<void*>(nullptr);
        auto function = SDL_GetAssertionHandler(&userdata);
        return {function, userdata};
    }

    static auto get_default() noexcept -> assertion_handler
    {
        return {SDL_GetDefaultAssertionHandler(), nullptr};
    }

private:
    SDL_AssertionHandler _function = nullptr;
    void* _userdata = nullptr;
};

inline auto get_assertion_report() noexcept -> assert_data_ref
{
    return assert_data_ref{SDL_GetAssertionReport()};
}

inline void reset_assertion_report() noexcept
{
    SDL_ResetAssertionReport();
}

inline void trigger_breakpoint() noexcept
{
    SDL_TriggerBreakpoint();
}

} // namespace sdl
