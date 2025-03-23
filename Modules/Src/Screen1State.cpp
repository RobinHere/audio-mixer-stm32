#include "Screen1State.hpp"

Screen1State& Screen1State::getInstance() {
    static Screen1State instance;
    return instance;
}

Screen1State::Screen1State()
    : outputSliderValue(0), inL1SliderValue(0), inR1SliderValue(0),
      inL2SliderValue(0), inR2SliderValue(0) {}

int8_t Screen1State::getOutputSliderValue() const { return outputSliderValue; }
int8_t Screen1State::getInL1SliderValue() const { return inL1SliderValue; }
int8_t Screen1State::getInR1SliderValue() const { return inR1SliderValue; }
int8_t Screen1State::getInL2SliderValue() const { return inL2SliderValue; }
int8_t Screen1State::getInR2SliderValue() const { return inR2SliderValue; }

void Screen1State::setOutputSliderValue(int8_t value) { outputSliderValue = value; }
void Screen1State::setInL1SliderValue(int8_t value) { inL1SliderValue = value; }
void Screen1State::setInR1SliderValue(int8_t value) { inR1SliderValue = value; }
void Screen1State::setInL2SliderValue(int8_t value) { inL2SliderValue = value; }
void Screen1State::setInR2SliderValue(int8_t value) { inR2SliderValue = value; }