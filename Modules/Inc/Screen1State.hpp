// Plik: Screen1State.hpp
#ifndef __SCREEN1STATE_HPP_
#define __SCREEN1STATE_HPP_

#include <cstdint>

class Screen1State {
public:
    static Screen1State& getInstance();

    int8_t getOutputSliderValue() const;
    int8_t getInL1SliderValue() const;
    int8_t getInR1SliderValue() const;
    int8_t getInL2SliderValue() const;
    int8_t getInR2SliderValue() const;

    void setOutputSliderValue(int8_t value);
    void setInL1SliderValue(int8_t value);
    void setInR1SliderValue(int8_t value);
    void setInL2SliderValue(int8_t value);
    void setInR2SliderValue(int8_t value);

private:
    Screen1State();
    Screen1State(const Screen1State&) = delete;
    Screen1State& operator=(const Screen1State&) = delete;

    int8_t outputSliderValue;
    int8_t inL1SliderValue;
    int8_t inR1SliderValue;
    int8_t inL2SliderValue;
    int8_t inR2SliderValue;
};

#endif /* __SCREEN1STATE_HPP_ */ 