#ifndef __SCREEN3STATE_HPP_
#define __SCREEN3STATE_HPP_

#include <cstdint>
#include "audio_module.h"

class Screen3State {
public:
    static Screen3State& getInstance();

    uint8_t getLowSliderValue() const;
    uint8_t getMidSliderValue() const;
    uint8_t getHighSliderValue() const;

    void setLowSliderValue(uint8_t value);
    void setMidSliderValue(uint8_t value);
    void setHighSliderValue(uint8_t value);

private:
    Screen3State();
    Screen3State(const Screen3State&) = delete;
    Screen3State& operator=(const Screen3State&) = delete;

    uint8_t lowSliderValue;
    uint8_t midSliderValue;
    uint8_t highSliderValue;
};

#endif /* __SCREEN3STATE_HPP_ */
