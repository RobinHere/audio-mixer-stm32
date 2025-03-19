#include "audio_module.h"

static volatile int8_t RightInput1GainDb = VOLUME_DEFAULT_GAIN;
static volatile int8_t LeftInput1GainDb = VOLUME_DEFAULT_GAIN;
static volatile int8_t RightInput2GainDb = VOLUME_DEFAULT_GAIN;
static volatile int8_t LeftInput2GainDb = VOLUME_DEFAULT_GAIN;
static volatile int8_t OutputGainDb = VOLUME_DEFAULT_GAIN;

static volatile int16_t entryBufferADC1[BUFFER_SIZE];
static volatile int16_t entryBufferADC2[BUFFER_SIZE];
static volatile int16_t exitBuffer[BUFFER_SIZE];
static volatile uint8_t entryBufferState = BUFFER_OFFSET_NONE;

static void CopySamplesBuffer(volatile int16_t* sourceBuffer, volatile int16_t* destinationBuffer, uint16_t bufferSize) {
    for(uint16_t i = 0; i < bufferSize; i++) {
        destinationBuffer[i] = sourceBuffer[i];
    }
}

static float decibelToAmplitudeMultiplier(int8_t gainDb) {
    return powf(10.f, gainDb / 20.f);
}

static float AdjustSampleGain(float sample, int8_t gainDb) {
    return sample * decibelToAmplitudeMultiplier(gainDb);
}

static void BasicMix(volatile int16_t* sourceBuffer1, volatile int16_t* sourceBuffer2,
                     volatile int16_t* destinationBuffer, uint16_t bufferSize) {
    float floatMixedSample = 0.0f;
    float sample1Norm = 0.0f;
    float sample2Norm = 0.0f;
    //Mix Left Channel
    for(uint16_t i = 0; i < bufferSize; i++) {
        sample1Norm = ((float)sourceBuffer1[i] / INT_16_MAX_VALUE);
        sample2Norm = ((float)sourceBuffer2[i] / INT_16_MAX_VALUE);
        if (i % 2 == 0) {
            sample1Norm = AdjustSampleGain(sample1Norm, LeftInput1GainDb);
            sample2Norm = AdjustSampleGain(sample2Norm, LeftInput2GainDb);
        } else {
            sample1Norm = AdjustSampleGain(sample1Norm, RightInput1GainDb);
            sample2Norm = AdjustSampleGain(sample2Norm, RightInput2GainDb);
        }
        floatMixedSample = sample1Norm + sample2Norm;
        floatMixedSample = AdjustSampleGain(floatMixedSample, OutputGainDb);
        if (floatMixedSample > 1.0f) {
            floatMixedSample = 1.0f;
        } 
        if (floatMixedSample < -1.0f) {
            floatMixedSample = -1.0f;
        }
        destinationBuffer[i] = (int16_t)(floatMixedSample * INT_16_MAX_VALUE);
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
            BasicMix(&entryBufferADC1[0], 
                     &entryBufferADC2[0], 
                     &exitBuffer[0], 
                     BUFFER_SIZE / 2);
        } else {
            BasicMix(&entryBufferADC1[BUFFER_SIZE / 2], 
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
