#include "audio_module.h"

volatile uint8_t RightInput1VolumeMultiplier = VOLUME_DEFAULT_LEVEL;
volatile uint8_t LeftInput1VolumeMultiplier = VOLUME_DEFAULT_LEVEL;
volatile uint8_t RightInput2VolumeMultiplier = VOLUME_DEFAULT_LEVEL;
volatile uint8_t LeftInput2VolumeMultiplier = VOLUME_DEFAULT_LEVEL;

static volatile int16_t entryBufferADC1[BUFFER_SIZE];
static volatile int16_t entryBufferADC2[BUFFER_SIZE];
static volatile int16_t exitBuffer[BUFFER_SIZE];
static volatile uint8_t entryBufferState = BUFFER_OFFSET_NONE;

static void CopySamplesBuffer(volatile int16_t* sourceBuffer, volatile int16_t* destinationBuffer, uint16_t bufferSize) {
    for(uint16_t i = 0; i < bufferSize; i++) {
        destinationBuffer[i] = sourceBuffer[i];
    }
}

static uint16_t AdjustSampleVolume(int16_t sample, uint8_t volumeMultiplier) {
    return (int16_t)((float)sample * ((float)volumeMultiplier / (float)VOLUME_DEFAULT_LEVEL));
}

static void NaiveMix(volatile int16_t* sourceBuffer1, volatile int16_t* sourceBuffer2,
    volatile int16_t* destinationBuffer, uint16_t bufferSize) {
    //Mix Left Channel
    for(uint16_t i = 0; i < bufferSize; i+=2) {
        sourceBuffer1[i] = AdjustSampleVolume(sourceBuffer1[i], LeftInput1VolumeMultiplier);
        sourceBuffer2[i] = AdjustSampleVolume(sourceBuffer2[i], LeftInput2VolumeMultiplier);
        destinationBuffer[i] = (int16_t)(sourceBuffer1[i] + sourceBuffer2[i] / 2);
    }
    //Mix Right Channel
    for(uint16_t i = 1; i < bufferSize; i+=2) {
        sourceBuffer1[i] = AdjustSampleVolume(sourceBuffer1[i], RightInput1VolumeMultiplier);
        sourceBuffer2[i] = AdjustSampleVolume(sourceBuffer2[i], RightInput2VolumeMultiplier);
        destinationBuffer[i] = (int16_t)(sourceBuffer1[i] + sourceBuffer2[i] / 2);
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
    StartAudioRxTransmission(&hsai_BlockB2, entryBufferADC2, BUFFER_SIZE);
    StartAudioTxTransmission(&hsai_BlockB1, exitBuffer, BUFFER_SIZE);
}

void AudioHandling() {
    if (entryBufferState != BUFFER_OFFSET_NONE) {
        if (entryBufferState == BUFFER_OFFSET_HALF) {
            NaiveMix(&entryBufferADC1[0], 
                     &entryBufferADC2[0], 
                     &exitBuffer[0], 
                     BUFFER_SIZE / 2);
        } else {
            NaiveMix(&entryBufferADC1[BUFFER_SIZE / 2], 
                     &entryBufferADC2[BUFFER_SIZE / 2], 
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
