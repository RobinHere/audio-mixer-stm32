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
#define DEFAULT_PAN 1.0f // MONO
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

typedef enum {
    PAN_CH1L,
    PAN_CH1R,
    PAN_CH2L,
    PAN_CH2R,
} PanChannel;

extern EqualizerFX equalizerFX;
extern DelayFx delayFx;

void AudioInit();
void AudioHandling();
void UpdatePreAmpGainFromSlider(GainType channel, float GainAmp);
void UpdateFaderGainFromSlider(GainType channel, float DbGain);
void UpdatePanFromSlider(PanChannel channel, float PanRatio);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_MODULE_H_ */
