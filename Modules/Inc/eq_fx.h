#ifndef __EQ_FX_H_
#define __EQ_FX_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <math.h>

typedef struct {
    volatile float gainAmpHighShelf;
    volatile float gainAmpPeaking;
    volatile float gainAmpLowShelf;
    uint16_t cutoffFreqHighShelf;
    uint16_t centerFreqPeaking;
    uint16_t cutoffFreqLowShelf;
    float QPeaking;
} EqualizerFX;

typedef struct {
    float previousInput1, previousInput2;
    float previousOutput1, previousOutput2;
} BiquadState;

void EqualizerInit(EqualizerFX* equalizerFX);
void updateGainLowShelf(EqualizerFX* equalizerFX, float gainAmp);
void updateGainPeaking(EqualizerFX* equalizerFX, float gainAmp);
void updateGainHighShelf(EqualizerFX* equalizerFX, float gainAmp);
void EqualizerFxProcess(EqualizerFX* equalizerFX, volatile float* audioBuffer, uint16_t bufferSize,
                        float sampleRate);

#ifdef __cplusplus
}
#endif

#endif /* __EQ_FX_H_ */
