#ifndef GEAROENIX_PLATFORM_APPLICATION_HPP
#define GEAROENIX_PLATFORM_APPLICATION_HPP
#include "../core/macro/gx-cr-mcr-getter-setter.hpp"
#include "gx-plt-arguments.hpp"
#include "gx-plt-build-configuration.hpp"
#include "gx-plt-key.hpp"
#include "gx-plt-main-entry.hpp"
#include "gx-plt-runtime-configuration.hpp"
#include <chrono>
#include <memory>

namespace gearoenix::core {
struct Application;
}

namespace gearoenix::core::event {
struct Engine;
}

namespace gearoenix::render::engine {
struct Engine;
}

namespace gearoenix::platform {
struct Application;
struct BaseApplication final {
    friend struct Application;

    GX_GET_CREF_PRV(RuntimeConfiguration, configuration)
    GX_GET_CREF_PRV(Arguments, arguments)

    GX_GET_VAL_PRV(bool, running, true)

    GX_GET_VAL_PRV(bool, window_resizing, false)
    GX_GET_VAL_PRV(int, previous_window_width, -1)
    GX_GET_VAL_PRV(int, previous_window_height, -1)
    GX_GET_VAL_PRV(int, window_width, -1)
    GX_GET_VAL_PRV(int, window_height, -1)

    GX_GET_VAL_PRV(int, window_x, -1)
    GX_GET_VAL_PRV(int, window_y, -1)
    GX_GET_VAL_PRV(bool, window_is_up, false)

    GX_GET_VAL_PRV(double, mouse_x, -1.0)
    GX_GET_VAL_PRV(double, mouse_y, -1.0)
    /// From -window_aspect -> +window_aspect
    GX_GET_VAL_PRV(double, mouse_x_nrm, -1.0)
    /// From -1.0 -> +1.0
    GX_GET_VAL_PRV(double, mouse_y_nrm, -1.0)
    GX_GET_VAL_PRV(double, pre_mouse_x, -1.0)
    GX_GET_VAL_PRV(double, pre_mouse_y, -1.0)
    GX_GET_VAL_PRV(double, pre_mouse_x_nrm, -1.0)
    GX_GET_VAL_PRV(double, pre_mouse_y_nrm, -1.0)

    GX_GET_UPTR_PRV(render::engine::Engine, render_engine)
    GX_GET_UPTR_PRV(core::event::Engine, event_engine)
    GX_GET_UPTR_PRV(core::Application, core_application)

    // boost::container::flat_map<platform::key::Id, Point2D> pressed_mouse_buttons_state;
    // boost::container::flat_map<touch::FingerId, Point2D> touch_states;
    // boost::container::flat_set<platform::key::Id> pressed_keyboard_buttons;

private:
    std::chrono::high_resolution_clock::time_point last_time_window_resized;

    BaseApplication(GX_MAIN_ENTRY_ARGS_DEF, const RuntimeConfiguration& configuration) noexcept;
    ~BaseApplication() noexcept;

    void initialize_window_position(int x, int y) noexcept;

    void initialize_window_size(int w, int h) noexcept;
    void update_window_size(int w, int h) noexcept;
    void update_window() noexcept;

    void initialize_mouse_position(double x, double y) noexcept;
    void update_mouse_position(double x, double y) noexcept;
    void mouse_key(key::Id, key::Action) noexcept;
    void mouse_wheel(double v) noexcept;

    void keyboard_key(key::Id, key::Action) noexcept;
    void character_input(char16_t ch) noexcept;

    void initialize_engine(Application&) noexcept;
    void initialize_core_application(Application&, core::Application*) noexcept;

    void going_to_be_closed() noexcept;
    void terminate() noexcept;

    void update() noexcept;

public:
};
}

#if defined(GX_PLATFORM_INTERFACE_ANDROID)
#include "android/gx-plt-and-application.hpp"
#elif defined(GX_PLATFORM_INTERFACE_WIN32)
#include "win32/gx-plt-win-application.hpp"
#elif defined(GX_PLATFORM_INTERFACE_X11)
#include "x11/gx-plt-x11-application.hpp"
#elif defined(GX_PLATFORM_INTERFACE_SDL2)
#include "sdl2/gx-plt-sdl2-application.hpp"
#else
#error "Unspecified platform application interface."
#endif
#endif