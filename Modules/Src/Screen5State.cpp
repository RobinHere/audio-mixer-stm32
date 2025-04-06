#include "Screen5State.hpp"

Screen5State& Screen5State::getInstance() {
    static Screen5State instance;
    return instance;
}

Screen5State::Screen5State()
    : inL1SliderValue(10), inR1SliderValue(10),
      inL2SliderValue(10), inR2SliderValue(10) {}

uint8_t Screen5State::getInL1SliderValue() const { return inL1SliderValue; }
uint8_t Screen5State::getInR1SliderValue() const { return inR1SliderValue; }
uint8_t Screen5State::getInL2SliderValue() const { return inL2SliderValue; }
uint8_t Screen5State::getInR2SliderValue() const { return inR2SliderValue; }

void Screen5State::setInL1SliderValue(uint8_t value) { inL1SliderValue = value; }
void Screen5State::setInR1SliderValue(uint8_t value) { inR1SliderValue = value; }
void Screen5State::setInL2SliderValue(uint8_t value) { inL2SliderValue = value; }
void Screen5State::setInR2SliderValue(uint8_t value) { inR2SliderValue = value; }