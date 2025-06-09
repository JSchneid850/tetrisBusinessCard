#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <map>
#include <optional>
#include "Action.cpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

enum Action {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    A,
    B
};

class Button{
    public:
        Action chosenAction;
        void handlePressOrHold() {
            if(chosenAction == DOWN){
                handleHold();
            }else{
                handlePress();
            }

        }

        void handlePress() {
            uint32_t currentTime = to_ms_since_boot(get_absolute_time());
            if ((currentTime - lastPressed) >= debounceMs) {
                lastPressed = currentTime;
                callback();
            }
        }

        void handleHold() {
                callback();
        }

        static void gpio_callback(uint gpio, uint32_t events) {
            if (events & GPIO_IRQ_EDGE_FALL) {
                auto instance = buttons.find(gpio);
                if (instance != buttons.end()) {
                    instance->second->handlePressOrHold();
                }
            }
        }

        void onPress(std::function<void()> callbackSet){
            callback = callbackSet;
        }

        Button(int pin, uint32_t debounce_ms = 200, std::optional<Action> action = std::nullopt ) : pin_(pin), debounceMs(debounce_ms){
            gpio_init(pin_);
            gpio_set_dir(pin_, GPIO_IN);
            gpio_pull_up(pin_);

            buttons[pin_] = this;

            gpio_set_irq_enabled_with_callback(pin_, GPIO_IRQ_EDGE_FALL, true, &Button::gpio_callback);
        }

    private:
        int pin_;
        uint32_t debounceMs;
        uint32_t lastPressed;
        std::function<void()> callback;
        static std::map<uint, Button*> buttons;
    };

    std::map<uint, Button*> Button::buttons;

#endif 