#include "audio_module.h"

static volatile int16_t entryBufferADC1[BUFFER_SIZE];
static volatile int16_t entryBufferADC2[BUFFER_SIZE];
static volatile int16_t exitBuffer[BUFFER_SIZE];
static volatile uint8_t entryBufferState = BUFFER_OFFSET_NONE;

static void CopySamplesBuffer(volatile int16_t* sourceBuffer, volatile int16_t* destinationBuffer, uint16_t bufferSize) {
    for(uint16_t i = 0; i < bufferSize; i++) {
        destinationBuffer[i] = sourceBuffer[i];
    }
}

static void StartAudioRxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* entryBuffer, uint16_t bufferSize) {
    HAL_SAI_Receive_DMA(saiBlock, (uint8_t*)entryBuffer, bufferSize);
}

static void StartAudioTxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* exitBuffer, uint16_t bufferSize) {
    HAL_SAI_Transmit_DMA(saiBlock, (uint8_t*)exitBuffer, bufferSize);
}

void AudioInit() {
    StartAudioRxTransmission(&hsai_BlockA1, entryBufferADC1, BUFFER_SIZE);
    StartAudioTxTransmission(&hsai_BlockB1, exitBuffer, BUFFER_SIZE);
}

void AudioHandling() {
    if (entryBufferState != BUFFER_OFFSET_NONE) {
        if (entryBufferState == BUFFER_OFFSET_HALF) {
            CopySamplesBuffer(&entryBufferADC1[0], 
                              &exitBuffer[0], 
                              BUFFER_SIZE / 2);
        } else {
            CopySamplesBuffer(&entryBufferADC1[BUFFER_SIZE / 2],
                              &exitBuffer[BUFFER_SIZE / 2],
                              BUFFER_SIZE / 2);
        }
        entryBufferState = BUFFER_OFFSET_NONE;
    }
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
    entryBufferState = BUFFER_OFFSET_HALF;
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    entryBufferState = BUFFER_OFFSET_FULL;
}
