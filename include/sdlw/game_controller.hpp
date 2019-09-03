#pragma once

#include <memory>

#include <SDL2/SDL_gamecontroller.h>

#include <sdlw/joystick.hpp>
#include <sdlw/rwops.hpp>

#include "sdlw/detail/utility.hpp"

namespace sdl {

// clang-format off

enum class game_controller_axis {
    invalid       = SDL_CONTROLLER_AXIS_INVALID,
    left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    right_y       = SDL_CONTROLLER_AXIS_RIGHTY,
    trigger_left  = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    max           = SDL_CONTROLLER_AXIS_MAX
};

inline auto to_game_controller_axis(const char* pch_string) noexcept -> game_controller_axis
{
    return static_cast<game_controller_axis>(SDL_GameControllerGetAxisFromString(pch_string));
}

inline auto to_string(game_controller_axis axis) noexcept -> const char*
{
    return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
}

enum class game_controller_button {
    invalid        = SDL_CONTROLLER_BUTTON_INVALID,
    a              = SDL_CONTROLLER_BUTTON_A,
    b              = SDL_CONTROLLER_BUTTON_B,
    x              = SDL_CONTROLLER_BUTTON_X,
    y              = SDL_CONTROLLER_BUTTON_Y,
    back           = SDL_CONTROLLER_BUTTON_BACK,
    guide          = SDL_CONTROLLER_BUTTON_GUIDE,
    start          = SDL_CONTROLLER_BUTTON_START,
    left_stick     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
    right_stick    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    left_shoulder  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    dpad_up        = SDL_CONTROLLER_BUTTON_DPAD_UP,
    dpad_down      = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    dpad_left      = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    dpad_right     = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    max            = SDL_CONTROLLER_BUTTON_MAX
};

inline auto to_game_controller_button(const char* pch_string) noexcept -> game_controller_button
{
    return static_cast<game_controller_button>(SDL_GameControllerGetButtonFromString(pch_string));
}

inline auto to_string(game_controller_button button) noexcept -> const char*
{
    return SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(button));
}

enum class game_controller_bind_type : std::underlying_type_t<SDL_GameControllerBindType> {
    none   = SDL_CONTROLLER_BINDTYPE_NONE,
    button = SDL_CONTROLLER_BINDTYPE_BUTTON,
    axis   = SDL_CONTROLLER_BINDTYPE_AXIS,
    hat    = SDL_CONTROLLER_BINDTYPE_HAT
};

class game_controller_button_bind {
public:
    explicit game_controller_button_bind(SDL_GameControllerButtonBind bind) noexcept
        : _bind{bind}
    {}

    auto type() const noexcept -> game_controller_bind_type
    {
        return static_cast<game_controller_bind_type>(_bind.bindType);
    }

    auto button() const noexcept -> int
    {
        return _bind.value.button;
    }

    auto axis() const noexcept -> int
    {
        return _bind.value.axis;
    }

    auto hat() const noexcept -> int
    {
        return _bind.value.hat.hat;
    }

    auto hat_mask() const noexcept -> int
    {
        return _bind.value.hat.hat_mask;
    }

private:
    SDL_GameControllerButtonBind _bind = {};
};

class game_controller_ref;

class game_controller {
public:
    game_controller(game_controller_ref) = delete;
    explicit game_controller(SDL_GameController* gc) noexcept : _game_controller{gc} {}
    auto get_pointer() const noexcept -> SDL_GameController* { return _game_controller.get(); }

    explicit game_controller(int joystick_index)
        : _game_controller{SDL_GameControllerOpen(joystick_index)}
    {
        if (!_game_controller) throw error{};
    }

    auto attached() const noexcept -> bool
    {
        return static_cast<bool>(SDL_GameControllerGetAttached(_game_controller.get()));
    }

    auto axis(game_controller_axis axis) const noexcept -> i16
    {
        return SDL_GameControllerGetAxis(_game_controller.get(), static_cast<SDL_GameControllerAxis>(axis));
    }

    auto bind(game_controller_axis axis) const noexcept -> game_controller_button_bind
    {
        const auto axis_ = static_cast<SDL_GameControllerAxis>(axis);
        const auto result = SDL_GameControllerGetBindForAxis(_game_controller.get(), axis_);
        return static_cast<game_controller_button_bind>(result);
    }

    auto bind(game_controller_button button) const noexcept -> game_controller_button_bind
    {
        const auto button_ = static_cast<SDL_GameControllerButton>(button);
        const auto result = SDL_GameControllerGetBindForButton(_game_controller.get(), button_);
        return static_cast<game_controller_button_bind>(result);
    }

    auto button(game_controller_button button) const noexcept -> bool
    {
        const auto button_ = static_cast<SDL_GameControllerButton>(button);
        const auto result = SDL_GameControllerGetButton(_game_controller.get(), button_);
        return static_cast<bool>(result);
    }

    auto joystick() noexcept -> joystick_ref
    {
        return joystick_ref{SDL_GameControllerGetJoystick(_game_controller.get())};
    }

    auto mapping() const noexcept -> std::unique_ptr<char[], detail::sdl_string_deleter>
    {
        return std::unique_ptr<char[], detail::sdl_string_deleter>{SDL_GameControllerMapping(_game_controller.get())};
    }

    auto name() const noexcept -> const char*
    {
        return SDL_GameControllerName(_game_controller.get());
    }

    static auto add_mapping(const char* mapping_string) -> bool
    {
        const auto result = SDL_GameControllerAddMapping(mapping_string);
        if (result < 0) throw error{};
        return static_cast<bool>(result);
    }

    static auto add_mappings(const char* filename) -> int
    {
        const auto result = SDL_GameControllerAddMappingsFromFile(filename);
        if (result < 0) throw error{};
        return result;
    }

    static auto add_mappings(stream& s) -> int
    {
        const auto result = SDL_GameControllerAddMappingsFromRW(s.get_pointer(), 0);
        if (result < 0) throw error{};
        return result;
    }

    static auto event_state() noexcept -> bool
    {
        return static_cast<bool>(SDL_GameControllerEventState(SDL_QUERY));
    }

    static void set_event_state(bool enabled) noexcept
    {
        SDL_GameControllerEventState(enabled ? SDL_ENABLE : SDL_DISABLE);
    }

    static auto get(joystick_id joyid) noexcept -> game_controller_ref;

    static auto mapping(joystick_guid guid) noexcept -> std::unique_ptr<char[], detail::sdl_string_deleter>
    {
        const auto guid_ = static_cast<SDL_JoystickGUID>(guid);
        const auto str = SDL_GameControllerMappingForGUID(guid_);
        return std::unique_ptr<char[], detail::sdl_string_deleter>{str};
    }

    static auto name(int joystick_index) noexcept -> const char*
    {
        return SDL_GameControllerNameForIndex(joystick_index);
    }

    static void update() noexcept
    {
        SDL_GameControllerUpdate();
    }

protected:
    std::unique_ptr<SDL_GameController, detail::make_functor<SDL_GameControllerClose>> _game_controller;
};

class game_controller_ref : public game_controller {
public:
    explicit operator bool() const noexcept { return static_cast<bool>(_game_controller); }
    explicit game_controller_ref(SDL_GameController* gc) noexcept : game_controller{gc} {}
    ~game_controller_ref() { _game_controller.release(); }
};

inline auto game_controller::get(joystick_id joyid) noexcept -> game_controller_ref
{
    return game_controller_ref{SDL_GameControllerFromInstanceID(joyid)};
}

inline auto is_game_controller(int joystick_index) noexcept -> bool
{
    return static_cast<bool>(SDL_IsGameController(joystick_index));
}

// clang-format on

} // namespace sdl
