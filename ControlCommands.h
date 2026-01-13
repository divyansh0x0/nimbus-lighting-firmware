#pragma once

namespace ROBO{
    enum class LEDState{
        ON = 0,
        OFF = 1,
        UNSET = 2,
    };
    struct ControlCommand{
        LEDState state;
        uint8_t id;

        ControlCommand() = default;

        ControlCommand(const uint8_t state, const uint8_t id) {
            switch (state) {
                case 0:
                    this->state = LEDState::OFF;
                    break;
                case 1:
                    this->state = LEDState::ON;
                    break;
                default:
                    this->state = LEDState::UNSET;
                    break;
            }

            this->id = id;
        }
    };
    // Any state other than 0 or 1 is invalid
    static ControlCommand NO_LED_STATE_CHANGE = {3, 0};

}
    