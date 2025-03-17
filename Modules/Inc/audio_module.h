#ifndef __AUDIO_MODULE_H_
#define __AUDIO_MODULE_H_

#include <stdint.h>
#include "sai.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BUFFER_SIZE 512

typedef enum {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF = 1,
    BUFFER_OFFSET_FULL = 2,
} bufferState;

void AudioInit();
void AudioHandling();

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_MODULE_H_ */
