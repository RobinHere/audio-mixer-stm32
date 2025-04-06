#ifndef __DELAY_FX_H_
#define __DELAY_FX_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define MAX_DELAY_TIME_SECONDS 1
#define SAMPLE_RATE_HZ 48000
#define MAX_DELAY_BUFFER_SIZE MAX_DELAY_TIME_SECONDS * SAMPLE_RATE_HZ

typedef enum {
    DELAY_OFF,
    DELAY_ON
} DelayFxStatus;

typedef struct {
    volatile float delayTimeSeconds;
    volatile float delayWetMix;
    volatile float delayDryMix;
    volatile float delayFeedback;
    volatile float delayBuffer[MAX_DELAY_BUFFER_SIZE];
    volatile uint32_t realSizeDelayBuffer;
    volatile uint32_t delayBufferIndex;
    volatile DelayFxStatus delayFxStatus;
} DelayFx;

void DelayFxInit(DelayFx* delayFx, float delayTimeSeconds, float wetness, float feedback);
void DelayFxProcess(DelayFx* delayFx, volatile float* audioBuffer, uint16_t bufferSize);
void DelayFxUpdateDelayTime(DelayFx* delayFx, float delayTimeSeconds);
void DelayFxUpdateFeedback(DelayFx* delayFx, float feedback);
void DelayFxUpdateWetness(DelayFx* delayFx, float wetness);
void DelayFxToggleOnOff(DelayFx* delayFx, DelayFxStatus status);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_FX_H_ */