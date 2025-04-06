#ifndef __SCREEN5STATE_HPP_
#define __SCREEN5STATE_HPP_

#include <cstdint>

class Screen5State {
public:
    static Screen5State& getInstance();

    uint8_t getInL1SliderValue() const;
    uint8_t getInR1SliderValue() const;
    uint8_t getInL2SliderValue() const;
    uint8_t getInR2SliderValue() const;

    void setInL1SliderValue(uint8_t value);
    void setInR1SliderValue(uint8_t value);
    void setInL2SliderValue(uint8_t value);
    void setInR2SliderValue(uint8_t value);

private:
    Screen5State();
    Screen5State(const Screen5State&) = delete;
    Screen5State& operator=(const Screen5State&) = delete;

    uint8_t inL1SliderValue;
    uint8_t inR1SliderValue;
    uint8_t inL2SliderValue;
    uint8_t inR2SliderValue;
};

#endif /* __SCREEN5STATE_HPP_ */ 