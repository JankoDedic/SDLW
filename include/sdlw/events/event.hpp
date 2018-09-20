#pragma once

#include <SDL2/SDL.h>

#include <sdlw/audio/device_id.hpp>
#include <sdlw/time/time.hpp>
#include <sdlw/video/window.hpp>
#include <sdlw/events/game_controller/axis.hpp>
#include <sdlw/events/game_controller/button.hpp>
#include <sdlw/events/gesture/id.hpp>
#include <sdlw/events/joystick/hat_position.hpp>
#include <sdlw/events/joystick/instance_id.hpp>
#include <sdlw/events/keyboard/key.hpp>
#include <sdlw/events/mouse/button_state.hpp>
#include <sdlw/events/mouse/wheel_direction.hpp>
#include <sdlw/events/touch/device_id.hpp>
#include <sdlw/events/touch/finger.hpp>

namespace sdlw::events {

class audio_device_event {
    SDL_AudioDeviceEvent _event;

    class added_t;
    class removed_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    bool
    is_capture_device() const noexcept
    {
        return static_cast<bool>(_event.iscapture);
    }

    const added_t&
    added() const noexcept;

    const removed_t&
    removed() const noexcept;
};

class audio_device_event::added_t : public audio_device_event {
public:
    u32
    audio_device_index() const noexcept
    {
        return _event.which;
    }
};

class audio_device_event::removed_t : public audio_device_event {
public:
    audio::device_id
    audio_device_id() const noexcept
    {
        return audio::device_id(_event.which);
    }
};

const audio_device_event::added_t&
audio_device_event::added() const noexcept
{
    return static_cast<const added_t&>(*this);
}

const audio_device_event::removed_t&
audio_device_event::removed() const noexcept
{
    return static_cast<const removed_t&>(*this);
}

class controller_axis_motion_event {
    SDL_ControllerAxisEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    game_controller::axis
    axis() const noexcept
    {
        return static_cast<game_controller::axis>(_event.axis);
    }

    i16
    value() const noexcept
    {
        return _event.value;
    }
};

class controller_button_event {
    SDL_ControllerButtonEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    game_controller::button
    button() const noexcept
    {
        return static_cast<game_controller::button>(_event.button);
    }
};

class controller_device_event {
    SDL_ControllerDeviceEvent _event;

    class added_t;
    class removed_t;
    class remapped_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    const added_t&
    added() const noexcept;

    const removed_t&
    removed() const noexcept;

    const remapped_t&
    remapped() const noexcept;
};

class controller_device_event::added_t : public controller_device_event {
public:
    i32
    joystick_device_index() const noexcept
    {
        return _event.which;
    }
};

class controller_device_event::removed_t : public controller_device_event {
public:
    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }
};

class controller_device_event::remapped_t : public controller_device_event {
public:
    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }
};

const controller_device_event::added_t&
controller_device_event::added() const noexcept
{
    return static_cast<const controller_device_event::added_t&>(*this);
}

const controller_device_event::removed_t&
controller_device_event::removed() const noexcept
{
    return static_cast<const controller_device_event::removed_t&>(*this);
}

const controller_device_event::remapped_t&
controller_device_event::remapped() const noexcept
{
    return static_cast<const controller_device_event::remapped_t&>(*this);
}

class dollar_event {
    SDL_DollarGestureEvent _event;

    class gesture_t;
    class record_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    touch::device_id
    touch_device_id() const noexcept
    {
        return touch::device_id(_event.touchId);
    }

    gesture::id
    gesture_id() const noexcept
    {
        return gesture::id(_event.gestureId);
    }

    const gesture_t&
    gesture() const noexcept;

    const record_t&
    record() const noexcept;
};

class dollar_event::gesture_t : public dollar_event {
public:
    u32
    number_of_fingers() const noexcept
    {
        return _event.numFingers;
    }

    float
    error() const noexcept
    {
        return _event.error;
    }

    float
    x() const noexcept
    {
        return _event.x;
    }

    float
    y() const noexcept
    {
        return _event.y;
    }
};

class dollar_event::record_t : public dollar_event {};

const dollar_event::gesture_t&
dollar_event::gesture() const noexcept
{
    return static_cast<const dollar_event::gesture_t&>(*this);
}

const dollar_event::record_t&
dollar_event::record() const noexcept
{
    return static_cast<const dollar_event::record_t&>(*this);
}

class drop_event {
    SDL_DropEvent _event;

    class file_t;
    class text_t;
    class begin_t;
    class complete_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    const file_t&
    file() const noexcept;

    const text_t&
    text() const noexcept;

    const begin_t&
    begin() const noexcept;

    const complete_t&
    complete() const noexcept;
};

class drop_event::file_t : public drop_event {
public:
    const char*
    filename() const noexcept
    {
        return _event.file;
    }
};

class drop_event::text_t : public drop_event {
public:
    const char*
    text() const noexcept
    {
        return _event.file;
    }
};

class drop_event::begin_t : public drop_event {};

class drop_event::complete_t : public drop_event {};

const drop_event::file_t&
drop_event::file() const noexcept
{
    return static_cast<const drop_event::file_t&>(*this);
}

const drop_event::text_t&
drop_event::text() const noexcept
{
    return static_cast<const drop_event::text_t&>(*this);
}

const drop_event::begin_t&
drop_event::begin() const noexcept
{
    return static_cast<const drop_event::begin_t&>(*this);
}

const drop_event::complete_t&
drop_event::complete() const noexcept
{
    return static_cast<const drop_event::complete_t&>(*this);
}

class touch_finger_event {
    SDL_TouchFingerEvent _event;

    class motion_t;
    class down_t;
    class up_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    touch::device_id
    touch_device_id() const noexcept
    {
        return touch::device_id(_event.touchId);
    }

    touch::finger_id
    finger_id() const noexcept
    {
        return touch::finger_id(_event.fingerId);
    }

    float
    x() const noexcept
    {
        return _event.x;
    }

    float
    y() const noexcept
    {
        return _event.y;
    }

    const motion_t&
    motion() const noexcept;

    const down_t&
    down() const noexcept;

    const up_t&
    up() const noexcept;
};

class touch_finger_event::motion_t : public touch_finger_event {
public:
    float
    x_distance() const noexcept
    {
        return _event.dx;
    }

    float
    y_distance() const noexcept
    {
        return _event.dy;
    }

    float
    pressure() const noexcept
    {
        return _event.pressure;
    }
};

class touch_finger_event::down_t : public touch_finger_event {
public:
    float
    pressure() const noexcept
    {
        return _event.pressure;
    }
};

class touch_finger_event::up_t : public touch_finger_event {};

const touch_finger_event::motion_t&
touch_finger_event::motion() const noexcept
{
    return static_cast<const touch_finger_event::motion_t&>(*this);
}

const touch_finger_event::down_t&
touch_finger_event::down() const noexcept
{
    return static_cast<const touch_finger_event::down_t&>(*this);
}

const touch_finger_event::up_t&
touch_finger_event::up() const noexcept
{
    return static_cast<const touch_finger_event::up_t&>(*this);
}

class key_event {
    SDL_KeyboardEvent _event;

    class up_t;
    class down_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    keyboard::key
    key() const noexcept
    {
        return keyboard::key(_event.keysym);
    }

    const up_t&
    up() const noexcept;

    const down_t&
    down() const noexcept;
};

class key_event::down_t : public key_event {
public:
    int
    repeat() const noexcept
    {
        return _event.repeat;
    }
};

class key_event::up_t : public key_event {};

const key_event::up_t&
key_event::up() const noexcept
{
    return static_cast<const key_event::up_t&>(*this);
}

const key_event::down_t&
key_event::down() const noexcept
{
    return static_cast<const key_event::down_t&>(*this);
}

class joy_axis_motion_event {
    SDL_JoyAxisEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    u8
    axis() const noexcept
    {
        return _event.axis;
    }

    i16
    value() const noexcept
    {
        return _event.value;
    }
};

class joy_ball_motion_event {
    SDL_JoyBallEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    u8
    ball() const noexcept
    {
        return _event.ball;
    }

    i16
    xrel() const noexcept
    {
        return _event.xrel;
    }

    i16
    yrel() const noexcept
    {
        return _event.yrel;
    }
};

class joy_hat_motion_event {
    SDL_JoyHatEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    u8
    hat() const noexcept
    {
        return _event.hat;
    }

    joystick::hat_position
    hat_position() const noexcept
    {
        return static_cast<joystick::hat_position>(_event.value);
    }
};

class joy_button_event {
    SDL_JoyButtonEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }

    u8
    button() const noexcept
    {
        return _event.button;
    }
};

class joy_device_event {
    SDL_JoyDeviceEvent _event;

    class added_t;
    class removed_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    const added_t&
    added() const noexcept;

    const removed_t&
    removed() const noexcept;
};

class joy_device_event::added_t : public joy_device_event {
public:
    i32
    joystick_device_index() const noexcept
    {
        return _event.which;
    }
};

class joy_device_event::removed_t : public joy_device_event {
public:
    joystick::instance_id
    joystick_instance_id() const noexcept
    {
        return joystick::instance_id(_event.which);
    }
};

const joy_device_event::added_t&
joy_device_event::added() const noexcept
{
    return static_cast<const joy_device_event::added_t&>(*this);
}

const joy_device_event::removed_t&
joy_device_event::removed() const noexcept
{
    return static_cast<const joy_device_event::removed_t&>(*this);
}

class mouse_motion_event {
    SDL_MouseMotionEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    u32
    mouse_instance_id() const noexcept
    {
        return _event.which;
    }

    mouse::button_state
    state() const noexcept
    {
        return static_cast<mouse::button_state>(_event.state);
    }

    i32
    x() const noexcept
    {
        return _event.x;
    }

    i32
    y() const noexcept
    {
        return _event.y;
    }

    i32
    xrel() const noexcept
    {
        return _event.xrel;
    }

    i32
    yrel() const noexcept
    {
        return _event.yrel;
    }
};

class mouse_button_event {
    SDL_MouseButtonEvent _event;

    class down_t;
    class up_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    u32
    mouse_instance_id() const noexcept
    {
        return _event.which;
    }

    mouse::button
    button() const noexcept
    {
        return mouse::button(_event.button);
    }

    i32
    x() const noexcept
    {
        return _event.x;
    }

    i32
    y() const noexcept
    {
        return _event.y;
    }

    const down_t&
    down() const noexcept;

    const up_t&
    up() const noexcept;
};

class mouse_button_event::down_t : public mouse_button_event {
public:
    u8
    clicks() const noexcept
    {
        return _event.clicks;
    }
};

class mouse_button_event::up_t : public mouse_button_event {};

const mouse_button_event::down_t&
mouse_button_event::down() const noexcept
{
    return static_cast<const mouse_button_event::down_t&>(*this);
}

const mouse_button_event::up_t&
mouse_button_event::up() const noexcept
{
    return static_cast<const mouse_button_event::up_t&>(*this);
}

class mouse_wheel_event {
    SDL_MouseWheelEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    u32
    mouse_instance_id() const noexcept
    {
        return _event.which;
    }

    i32
    x_scroll() const noexcept
    {
        return _event.x;
    }

    i32
    y_scroll() const noexcept
    {
        return _event.y;
    }

    mouse::wheel_direction
    direction() const noexcept
    {
        return static_cast<mouse::wheel_direction>(_event.direction);
    }
};

class multi_gesture_event {
    SDL_MultiGestureEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    touch::device_id
    touch_device_id() const noexcept
    {
        return touch::device_id(_event.touchId);
    }

    float
    rotation() const noexcept
    {
        return _event.dTheta;
    }

    float
    pinch() const noexcept
    {
        return _event.dDist;
    }

    float
    x() const noexcept
    {
        return _event.x;
    }

    float
    y() const noexcept
    {
        return _event.y;
    }

    u16
    number_of_fingers() const noexcept
    {
        return _event.numFingers;
    }
};

class quit_event {
    SDL_QuitEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }
};

class syswm_event {
    SDL_SysWMEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    SDL_SysWMmsg*
    message() const noexcept
    {
        return _event.msg;
    }
};

class text_editing_event {
    SDL_TextEditingEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    const char*
    text() const noexcept
    {
        return _event.text;
    }

    i32
    start() const noexcept
    {
        return _event.start;
    }

    i32
    length() const noexcept
    {
        return _event.length;
    }
};

class text_input_event {
    SDL_TextInputEvent _event;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    const char*
    text() const noexcept
    {
        return _event.text;
    }
};

enum class window_event_type : Uint8 {
    none = SDL_WINDOWEVENT_NONE,
    shown = SDL_WINDOWEVENT_SHOWN,
    hidden = SDL_WINDOWEVENT_HIDDEN,
    exposed = SDL_WINDOWEVENT_EXPOSED,
    moved = SDL_WINDOWEVENT_MOVED,
    resized = SDL_WINDOWEVENT_RESIZED,
    size_changed = SDL_WINDOWEVENT_SIZE_CHANGED,
    minimized = SDL_WINDOWEVENT_MINIMIZED,
    maximized = SDL_WINDOWEVENT_MAXIMIZED,
    restored = SDL_WINDOWEVENT_RESTORED,
    enter = SDL_WINDOWEVENT_ENTER,
    leave = SDL_WINDOWEVENT_LEAVE,
    focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
    focus_lost = SDL_WINDOWEVENT_FOCUS_LOST,
    close = SDL_WINDOWEVENT_CLOSE,
    take_focus = SDL_WINDOWEVENT_TAKE_FOCUS,
    hit_test = SDL_WINDOWEVENT_HIT_TEST
};

class window_event {
    SDL_WindowEvent _event;

    class moved_t;
    class resized_t;

public:
    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.timestamp));
    }

    window_event_type
    type() const noexcept
    {
        return static_cast<window_event_type>(_event.event);
    }

    video::window_id
    window_id() const noexcept
    {
        return static_cast<video::window_id>(_event.windowID);
    }

    const moved_t&
    moved() const noexcept;

    const resized_t&
    resized() const noexcept;
};

class window_event::moved_t : public window_event {
public:
    i32
    x() const noexcept
    {
        return _event.data1;
    }

    i32
    y() const noexcept
    {
        return _event.data2;
    }
};

class window_event::resized_t : public window_event {
public:
    i32
    width() const noexcept
    {
        return _event.data1;
    }

    i32
    height() const noexcept
    {
        return _event.data2;
    }
};

const window_event::moved_t&
window_event::moved() const noexcept
{
    return static_cast<const window_event::moved_t&>(*this);
}

const window_event::resized_t&
window_event::resized() const noexcept
{
    return static_cast<const window_event::resized_t&>(*this);
}

enum class event_type {
    first_event = SDL_FIRSTEVENT,
    quit = SDL_QUIT,
    app_terminating = SDL_APP_TERMINATING,
    app_low_memory = SDL_APP_LOWMEMORY,
    app_will_enter_background = SDL_APP_WILLENTERBACKGROUND,
    app_did_enter_background = SDL_APP_DIDENTERBACKGROUND,
    app_will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
    app_did_enter_foreground = SDL_APP_DIDENTERFOREGROUND,
    window = SDL_WINDOWEVENT,
    syswm = SDL_SYSWMEVENT,
    key_down = SDL_KEYDOWN,
    key_up = SDL_KEYUP,
    text_editing = SDL_TEXTEDITING,
    text_input = SDL_TEXTINPUT,
    keymap_changed = SDL_KEYMAPCHANGED,
    mouse_motion = SDL_MOUSEMOTION,
    mouse_button_down = SDL_MOUSEBUTTONDOWN,
    mouse_button_up = SDL_MOUSEBUTTONUP,
    mouse_wheel = SDL_MOUSEWHEEL,
    joy_axis_motion = SDL_JOYAXISMOTION,
    joy_ball_motion = SDL_JOYBALLMOTION,
    joy_hat_motion = SDL_JOYHATMOTION,
    joy_button_down = SDL_JOYBUTTONDOWN,
    joy_button_up = SDL_JOYBUTTONUP,
    joy_device_added = SDL_JOYDEVICEADDED,
    joy_device_removed = SDL_JOYDEVICEREMOVED,
    controller_axis_motion = SDL_CONTROLLERAXISMOTION,
    controller_button_down = SDL_CONTROLLERBUTTONDOWN,
    controller_button_up = SDL_CONTROLLERBUTTONUP,
    controller_device_added = SDL_CONTROLLERDEVICEADDED,
    controller_device_removed = SDL_CONTROLLERDEVICEREMOVED,
    controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
    finger_down = SDL_FINGERDOWN,
    finger_up = SDL_FINGERUP,
    finger_motion = SDL_FINGERMOTION,
    dollar_gesture = SDL_DOLLARGESTURE,
    dollar_record = SDL_DOLLARRECORD,
    multi_gesture = SDL_MULTIGESTURE,
    clipboard_update = SDL_CLIPBOARDUPDATE,
    drop_file = SDL_DROPFILE,
    drop_text = SDL_DROPTEXT,
    drop_begin = SDL_DROPBEGIN,
    drop_complete = SDL_DROPCOMPLETE,
    audio_device_added = SDL_AUDIODEVICEADDED,
    audio_device_removed = SDL_AUDIODEVICEREMOVED,
    render_targets_reset = SDL_RENDER_TARGETS_RESET,
    render_device_reset = SDL_RENDER_DEVICE_RESET,
    user_event = SDL_USEREVENT,
    last_event = SDL_LASTEVENT
};

class event {
    SDL_Event _event;

public:
    event() noexcept = default;

    event_type
    type() const noexcept
    {
        return static_cast<event_type>(_event.type);
    }

    time::clock::time_point
    timestamp() const noexcept
    {
        using namespace time;
        return clock::time_point(clock::duration(_event.common.timestamp));
    }

    const audio_device_event&
    audio_device() const noexcept
    {
        return reinterpret_cast<const audio_device_event&>(_event.adevice);
    }

    const controller_axis_motion_event&
    controller_axis_motion() const noexcept
    {
        const auto& e = _event.caxis;
        return reinterpret_cast<const controller_axis_motion_event&>(e);
    }

    const controller_button_event&
    controller_button() const noexcept
    {
        const auto& e = _event.cbutton;
        return reinterpret_cast<const controller_button_event&>(e);
    }

    const controller_device_event&
    controller_device() const noexcept
    {
        const auto& e = _event.cdevice;
        return reinterpret_cast<const controller_device_event&>(e);
    }

    const dollar_event&
    dollar() const noexcept
    {
        return reinterpret_cast<const dollar_event&>(_event.dgesture);
    }

    const drop_event&
    drop() const noexcept
    {
        return reinterpret_cast<const drop_event&>(_event.drop);
    }

    const touch_finger_event&
    touch_finger() const noexcept
    {
        return reinterpret_cast<const touch_finger_event&>(_event.tfinger);
    }

    const key_event&
    key() const noexcept
    {
        return reinterpret_cast<const key_event&>(_event.key);
    }

    const joy_axis_motion_event&
    joy_axis_motion() const noexcept
    {
        return reinterpret_cast<const joy_axis_motion_event&>(_event.jaxis);
    }

    const joy_ball_motion_event&
    joy_ball_motion() const noexcept
    {
        return reinterpret_cast<const joy_ball_motion_event&>(_event.jball);
    }

    const joy_hat_motion_event&
    joy_hat_motion() const noexcept
    {
        return reinterpret_cast<const joy_hat_motion_event&>(_event.jhat);
    }

    const joy_button_event&
    joy_button() const noexcept
    {
        return reinterpret_cast<const joy_button_event&>(_event.jbutton);
    }

    const joy_device_event&
    joy_device() const noexcept
    {
        return reinterpret_cast<const joy_device_event&>(_event.jdevice);
    }

    const mouse_motion_event&
    mouse_motion() const noexcept
    {
        return reinterpret_cast<const mouse_motion_event&>(_event.motion);
    }

    const mouse_button_event&
    mouse_button() const noexcept
    {
        return reinterpret_cast<const mouse_button_event&>(_event.button);
    }

    const mouse_wheel_event&
    mouse_wheel() const noexcept
    {
        return reinterpret_cast<const mouse_wheel_event&>(_event.wheel);
    }

    const multi_gesture_event&
    multi_gesture() const noexcept
    {
        return reinterpret_cast<const multi_gesture_event&>(_event.mgesture);
    }

    const quit_event&
    quit() const noexcept
    {
        return reinterpret_cast<const quit_event&>(_event.quit);
    }

    const syswm_event&
    syswm() const noexcept
    {
        return reinterpret_cast<const syswm_event&>(_event.syswm);
    }

    const text_editing_event&
    text_editing() const noexcept
    {
        return reinterpret_cast<const text_editing_event&>(_event.edit);
    }

    const text_input_event&
    text_input() const noexcept
    {
        return reinterpret_cast<const text_input_event&>(_event.text);
    }

    const window_event&
    window() const noexcept
    {
        return reinterpret_cast<const window_event&>(_event.window);
    }
};

} // namespace sdlw::events
