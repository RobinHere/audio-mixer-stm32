#ifndef __SCREEN2STATE_HPP_
#define __SCREEN2STATE_HPP_

#include <cstdint>
#include "delay_fx.h"

class Screen2State {
public:
    static Screen2State& getInstance();

    uint8_t getFeedbackSliderValue() const;
    uint8_t getWetnessSliderValue() const;
    uint8_t getDelayTimeSliderValue() const;
    DelayFxStatus getButtonState() const;

    void setFeedbackSliderValue(uint8_t value);
    void setWetnessSliderValue(uint8_t value);
    void setDelayTimeSliderValue(uint8_t value);
    void setButtonState(DelayFxStatus state);

private:
    Screen2State();
    Screen2State(const Screen2State&) = delete;
    Screen2State& operator=(const Screen2State&) = delete;
    uint8_t feedbackSliderValue;
    uint8_t wetnessSliderValue;
    uint8_t delayTimeSliderValue;
    DelayFxStatus buttonState;
};

#endif /* __SCREEN2STATE_HPP_ */
