#include "Screen2State.hpp"

Screen2State& Screen2State::getInstance() {
    static Screen2State instance;
    return instance;
}

Screen2State::Screen2State()
    : feedbackSliderValue(50), wetnessSliderValue(50),
      delayTimeSliderValue(50), buttonState(DELAY_OFF) {}

uint8_t Screen2State::getFeedbackSliderValue() const { return feedbackSliderValue; }
uint8_t Screen2State::getWetnessSliderValue() const { return wetnessSliderValue; }
uint8_t Screen2State::getDelayTimeSliderValue() const { return delayTimeSliderValue; }
DelayFxStatus Screen2State::getButtonState() const { return buttonState; }

void Screen2State::setFeedbackSliderValue(uint8_t value) { feedbackSliderValue = value; }
void Screen2State::setWetnessSliderValue(uint8_t value) { wetnessSliderValue = value; }
void Screen2State::setDelayTimeSliderValue(uint8_t value) { delayTimeSliderValue = value; }
void Screen2State::setButtonState(DelayFxStatus state) { buttonState = state; }