#ifndef __SCREEN3STATE_HPP_
#define __SCREEN3STATE_HPP_

#include <cstdint>
#include "audio_module.h"

class Screen3State {
public:
    static Screen3State& getInstance();

    int8_t getLowSliderValue() const;
    int8_t getMidSliderValue() const;
    int8_t getHighSliderValue() const;

    void setLowSliderValue(int8_t value);
    void setMidSliderValue(int8_t value);
    void setHighSliderValue(int8_t value);

private:
    Screen3State();
    Screen3State(const Screen3State&) = delete;
    Screen3State& operator=(const Screen3State&) = delete;

    int8_t lowSliderValue;
    int8_t midSliderValue;
    int8_t highSliderValue;
};

#endif /* __SCREEN3STATE_HPP_ */
