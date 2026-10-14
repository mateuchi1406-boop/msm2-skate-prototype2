#include "game/Actor.h"
#include "game/HeroSystem.h"
#include "game/Transform.h"

#include <Windows.h>
#include <Xinput.h>
#include <atomic>
#include <chrono>
#include <cmath>
#include <thread>

#pragma comment(lib, "xinput9_1_0.lib")

namespace {
    enum class TrickKind {
        None,
        Ollie,
        Nollie,
        Kickflip,
        Heelflip,
        FrontsideShuvit,
        BacksideShuvit,
    };

    std::atomic_bool g_running { true };
    std::atomic_bool g_started { false };

    bool IsDown(int key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    bool WasPressed(int key, bool& previous) {
        const bool current = IsDown(key);
        const bool pressed = current && !previous;
        previous = current;
        return pressed;
    }

    float ApplyStickDeadzone(SHORT value) {
        constexpr float deadzone = 0.15f;
        const float normalized = static_cast<float>(value) / 32767.0f;
        const float magnitude = std::fabs(normalized);
        if (magnitude <= deadzone) {
            return 0.0f;
        }

        const float rescaled = (magnitude - deadzone) / (1.0f - deadzone);
        return normalized < 0.0f ? -rescaled : rescaled;
    }

    TrickKind DetectRightStickFlick(float x, float y, bool& ready) {
        const float magnitude = std::sqrt(x * x + y * y);
        if (magnitude < 0.35f) {
            ready = true;
            return TrickKind::None;
        }

        if (!ready) {
            return TrickKind::None;
        }

        ready = false;
        if (y > 0.55f && std::fabs(x) < 0.35f) {
            return TrickKind::Ollie;
        }
        if (y < -0.55f && std::fabs(x) < 0.35f) {
            return TrickKind::Nollie;
        }
        if (y > 0.35f && x > 0.35f) {
            return TrickKind::Kickflip;
        }
        if (y > 0.35f && x < -0.35f) {
            return TrickKind::Heelflip;
        }
        if (x > 0.55f && std::fabs(y) < 0.35f) {
            return TrickKind::FrontsideShuvit;
        }
        if (x < -0.55f && std::fabs(y) < 0.35f) {
            return TrickKind::BacksideShuvit;
        }

        return TrickKind::None;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}

extern "C" __declspec(dllexport) void script_enable() {
    if (g_started.exchange(true)) {
        return;
    }

    std::thread([]() {
        bool toggle_previous = false;
        bool ollie_previous = false;
        bool controller_toggle_previous = false;
        bool controller_ollie_previous = false;
        bool right_stick_ready = true;
        bool skate_mode = false;
        bool airborne = false;
        float ground_y = 0.0f;
        float vertical_velocity = 0.0f;

        auto last_tick = std::chrono::steady_clock::now();

        while (g_running.load()) {
            const auto now = std::chrono::steady_clock::now();
            float dt = std::chrono::duration<float>(now - last_tick).count();
            last_tick = now;
            dt = std::fmin(dt, 0.05f);

            XINPUT_STATE controller_state {};
            const bool controller_connected =
                XInputGetState(0, &controller_state) == ERROR_SUCCESS;
            const WORD controller_buttons = controller_connected
                ? controller_state.Gamepad.wButtons
                : 0;
            const bool controller_toggle = controller_connected &&
                (controller_buttons & XINPUT_GAMEPAD_BACK) != 0 &&
                (controller_buttons & XINPUT_GAMEPAD_START) != 0;
            const bool controller_ollie = controller_connected &&
                (controller_buttons & XINPUT_GAMEPAD_A) != 0;

            const bool toggle_pressed =
                WasPressed(VK_F7, toggle_previous) ||
                (controller_toggle && !controller_toggle_previous);
            const bool ollie_pressed =
                WasPressed(VK_SPACE, ollie_previous) ||
                (controller_ollie && !controller_ollie_previous);
            controller_toggle_previous = controller_toggle;
            controller_ollie_previous = controller_ollie;

            const float right_stick_x = controller_connected
                ? ApplyStickDeadzone(controller_state.Gamepad.sThumbRX)
                : 0.0f;
            const float right_stick_y = controller_connected
                ? ApplyStickDeadzone(controller_state.Gamepad.sThumbRY)
                : 0.0f;
            const TrickKind trick = DetectRightStickFlick(
                right_stick_x, right_stick_y, right_stick_ready);

            if (toggle_pressed) {
                skate_mode = !skate_mode;
                airborne = false;
                vertical_velocity = 0.0f;
            }

            HeroSystem* system = GetHeroSystem();
            Actor* hero = system ? system->GetHero() : nullptr;
            Transform* transform = hero ? hero->GetTransform() : nullptr;

            if (skate_mode && transform) {
                Vector3& position = transform->GetPosition();
                if (!airborne) {
                    ground_y = position.y;
                }

                float forward = (IsDown('W') ? 1.0f : 0.0f) - (IsDown('S') ? 1.0f : 0.0f);
                float sideways = (IsDown('D') ? 1.0f : 0.0f) - (IsDown('A') ? 1.0f : 0.0f);
                bool boost = IsDown(VK_SHIFT);

                if (controller_connected) {
                    const float controller_forward = ApplyStickDeadzone(
                        controller_state.Gamepad.sThumbLY);
                    const float controller_sideways = ApplyStickDeadzone(
                        controller_state.Gamepad.sThumbLX);
                    if (controller_forward != 0.0f || controller_sideways != 0.0f) {
                        forward = controller_forward;
                        sideways = controller_sideways;
                    }
                    boost = boost || controller_state.Gamepad.bRightTrigger > 160;
                }

                const float speed = boost ? 18.0f : 10.0f;
                position.x += forward * speed * dt;
                position.z += sideways * speed * dt;

                const bool trick_started = trick != TrickKind::None;
                if ((ollie_pressed || trick_started) && !airborne) {
                    airborne = true;
                    vertical_velocity = trick == TrickKind::Nollie ? 7.2f : 7.0f;

                    // Flip/shuvit names are separated so animation and board
                    // rotation can be attached in a later SDK milestone.
                    if (trick == TrickKind::FrontsideShuvit) {
                        position.z += 0.15f;
                    } else if (trick == TrickKind::BacksideShuvit) {
                        position.z -= 0.15f;
                    }
                }

                if (airborne) {
                    vertical_velocity -= 18.0f * dt;
                    position.y += vertical_velocity * dt;
                    if (position.y <= ground_y) {
                        position.y = ground_y;
                        vertical_velocity = 0.0f;
                        airborne = false;
                    }
                }
            } else {
                WasPressed(VK_SPACE, ollie_previous);
            }

            Sleep(16);
        }
    }).detach();
}

extern "C" __declspec(dllexport) void script_disable() {
    g_running.store(false);
}
