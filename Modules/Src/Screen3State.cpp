#include "Screen3State.hpp"

Screen3State& Screen3State::getInstance() {
    static Screen3State instance;
    return instance;
}

Screen3State::Screen3State() : lowSliderValue(0), midSliderValue(0), highSliderValue(0) {}

int8_t Screen3State::getLowSliderValue() const { return lowSliderValue; }
int8_t Screen3State::getMidSliderValue() const { return midSliderValue; }
int8_t Screen3State::getHighSliderValue() const { return highSliderValue; }

void Screen3State::setLowSliderValue(int8_t value) { lowSliderValue = value; }
void Screen3State::setMidSliderValue(int8_t value) { midSliderValue = value; }
void Screen3State::setHighSliderValue(int8_t value) { highSliderValue = value; }
