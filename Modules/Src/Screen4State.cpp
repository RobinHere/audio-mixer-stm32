#include "Screen4State.hpp"

Screen4State& Screen4State::getInstance() {
    static Screen4State instance;
    return instance;
}

Screen4State::Screen4State()
    : inL1SliderValue(0), inR1SliderValue(0),
      inL2SliderValue(0), inR2SliderValue(0) {}

int8_t Screen4State::getInL1SliderValue() const { return inL1SliderValue; }
int8_t Screen4State::getInR1SliderValue() const { return inR1SliderValue; }
int8_t Screen4State::getInL2SliderValue() const { return inL2SliderValue; }
int8_t Screen4State::getInR2SliderValue() const { return inR2SliderValue; }

void Screen4State::setInL1SliderValue(int8_t value) { inL1SliderValue = value; }
void Screen4State::setInR1SliderValue(int8_t value) { inR1SliderValue = value; }
void Screen4State::setInL2SliderValue(int8_t value) { inL2SliderValue = value; }
void Screen4State::setInR2SliderValue(int8_t value) { inR2SliderValue = value; }