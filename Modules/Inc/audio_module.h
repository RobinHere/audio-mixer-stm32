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

static volatile int16_t entryBufferADC1[BUFFER_SIZE];
static volatile int16_t exitBuffer[BUFFER_SIZE];
static volatile uint8_t entryBufferState = BUFFER_OFFSET_NONE; 

static void CopySamplesBuffer(volatile int16_t* sourceBuffer, volatile int16_t* destinationBuffer, uint16_t bufferSize);
static void StartAudioRxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* entryBuffer, uint16_t bufferSize);
static void StartAudioTxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* exitBuffer, uint16_t bufferSize);
void AudioInit();
void AudioHandling();

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_MODULE_H_ */
