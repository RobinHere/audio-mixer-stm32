#ifndef __AUDIO_MODULE_H_
#define __AUDIO_MODULE_H_

#include <stdint.h>
#include <math.h>
#include "sai.h"
#include "delay_fx.h"
#include "eq_fx.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BUFFER_SIZE 512
#define VOLUME_DEFAULT_GAIN 1.0f
#define INT_16_MAX_VALUE 32767.0f

typedef enum {
    BUFFER_OFFSET_NONE,
    BUFFER_OFFSET_HALF,
    BUFFER_OFFSET_FULL,
} BufferState;

typedef enum {
    GAIN_OUTPUT,
    GAIN_CH1L,
    GAIN_CH1R,
    GAIN_CH2L,
    GAIN_CH2R,
} GainType;

extern EqualizerFX equalizerFX;

void AudioInit();
void AudioHandling();
void UpdateGainFromSlider(GainType channel, float DbGain);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_MODULE_H_ */
