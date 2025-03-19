#ifndef __AUDIO_MODULE_H_
#define __AUDIO_MODULE_H_

#include <stdint.h>
#include <math.h>
#include "sai.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BUFFER_SIZE 512
#define VOLUME_DEFAULT_GAIN 0
#define INT_16_MAX_VALUE 32767.0f

typedef enum {
    BUFFER_OFFSET_NONE,
    BUFFER_OFFSET_HALF,
    BUFFER_OFFSET_FULL,
} BufferState;

void AudioInit();
void AudioHandling();

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_MODULE_H_ */
