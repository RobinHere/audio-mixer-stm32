#ifndef __SCREEN4STATE_HPP_
#define __SCREEN4STATE_HPP_

#include <cstdint>

class Screen4State {
public:
    static Screen4State& getInstance();

    int8_t getInL1SliderValue() const;
    int8_t getInR1SliderValue() const;
    int8_t getInL2SliderValue() const;
    int8_t getInR2SliderValue() const;

    void setInL1SliderValue(int8_t value);
    void setInR1SliderValue(int8_t value);
    void setInL2SliderValue(int8_t value);
    void setInR2SliderValue(int8_t value);

private:
    Screen4State();
    Screen4State(const Screen4State&) = delete;
    Screen4State& operator=(const Screen4State&) = delete;

    int8_t inL1SliderValue;
    int8_t inR1SliderValue;
    int8_t inL2SliderValue;
    int8_t inR2SliderValue;
};

#endif /* __SCREEN4STATE_HPP_ */ 