#include "audio_module.h"

static volatile float RightInput1GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInput1GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float RightInput2GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInput2GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float OutputGainAmp = VOLUME_DEFAULT_GAIN;

static volatile int16_t entryBufferADC1[BUFFER_SIZE];
static volatile int16_t entryBufferADC2[BUFFER_SIZE];
static volatile int16_t exitBuffer[BUFFER_SIZE];
static volatile float normalizedBufferADC1[BUFFER_SIZE];
static volatile float normalizedBufferADC2[BUFFER_SIZE];
static volatile float normalizedExitBuffer[BUFFER_SIZE];

static volatile uint8_t entryBufferState = BUFFER_OFFSET_NONE;

static void NormalizeEntryBuffers(volatile int16_t* sourceBuffer1, 
                                  volatile int16_t* sourceBuffer2, 
                                  volatile float* normalizedBuffer1,
                                  volatile float* normalizedBuffer2,
                                  uint16_t bufferSize) {
    for(uint16_t i = 0; i < bufferSize; i++) {
        normalizedBuffer1[i] = ((float)sourceBuffer1[i] / INT_16_MAX_VALUE);
        normalizedBuffer2[i] = ((float)sourceBuffer2[i] / INT_16_MAX_VALUE);
    }
}

static void DenormalizeBuffer(volatile int16_t* destinationBuffer, volatile float* normalizedBuffer,
                              uint16_t bufferSize) {
    for(uint16_t i = 0; i < bufferSize; i++) {
        normalizedBuffer[i] *= OutputGainAmp;
        destinationBuffer[i] = (int16_t)(normalizedBuffer[i] * INT_16_MAX_VALUE);
    }
}

static void BasicMix(volatile float* sourceBuffer1, volatile float* sourceBuffer2,
                     volatile float* destinationBuffer, uint16_t bufferSize) {
    for (uint16_t i = 0; i < bufferSize; i++) {
        if (i % 2 == 0) {
            sourceBuffer1[i] *= LeftInput1GainAmp;
            sourceBuffer2[i] *= LeftInput2GainAmp;
        } else {
            sourceBuffer1[i] *= RightInput1GainAmp;
            sourceBuffer2[i] *= RightInput2GainAmp;
        }
        destinationBuffer[i] = sourceBuffer1[i] + sourceBuffer2[i];
        if (destinationBuffer[i] > 1.0f) {
            destinationBuffer[i] = 1.0f;
        } 
        if (destinationBuffer[i] < -1.0f) {
            destinationBuffer[i] = -1.0f;
        }
    }
}

static void StartAudioRxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* entryBuffer, uint16_t bufferSize) {
    HAL_SAI_Receive_DMA(saiBlock, (uint8_t*)entryBuffer, bufferSize);
}

static void StartAudioTxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* exitBuffer, uint16_t bufferSize) {
    HAL_SAI_Transmit_DMA(saiBlock, (uint8_t*)exitBuffer, bufferSize);
}

void UpdateGainFromSlider(GainType channel, float GainAmp) {
    switch(channel) {
        case GAIN_OUTPUT:
            OutputGainAmp = GainAmp;
            break;
        case GAIN_CH1L:
            LeftInput1GainAmp = GainAmp;
            break;
        case GAIN_CH1R:
            RightInput1GainAmp = GainAmp;
            break;
        case GAIN_CH2L:
            LeftInput2GainAmp = GainAmp;
            break;
        case GAIN_CH2R:
            RightInput2GainAmp = GainAmp;
            break;
        default:
            break;
    }
}

void AudioInit() {
    DelayFxInit(0.5f, 0.5f, 0.5f);
    StartAudioRxTransmission(&hsai_BlockA1, entryBufferADC1, BUFFER_SIZE);
    StartAudioRxTransmission(&hsai_BlockB2, entryBufferADC2, BUFFER_SIZE);
    StartAudioTxTransmission(&hsai_BlockB1, exitBuffer, BUFFER_SIZE);
}

void AudioHandling() {
    if (entryBufferState != BUFFER_OFFSET_NONE) {
        if (entryBufferState == BUFFER_OFFSET_HALF) {
            NormalizeEntryBuffers(&entryBufferADC1[0],
                                  &entryBufferADC2[0],
                                  &normalizedBufferADC1[0],
                                  &normalizedBufferADC2[0],
                                  BUFFER_SIZE / 2);
            BasicMix(&normalizedBufferADC1[0],
                     &normalizedBufferADC2[0],
                     &normalizedExitBuffer[0],
                     BUFFER_SIZE / 2);
            DelayFxProcess(&normalizedExitBuffer[0],  BUFFER_SIZE / 2);
            DenormalizeBuffer(&exitBuffer[0], 
                              &normalizedExitBuffer[0],
                              BUFFER_SIZE / 2);
        } else {
            NormalizeEntryBuffers(&entryBufferADC1[BUFFER_SIZE / 2],
                                  &entryBufferADC2[BUFFER_SIZE / 2],
                                  &normalizedBufferADC1[BUFFER_SIZE / 2],
                                  &normalizedBufferADC2[BUFFER_SIZE / 2],
                                  BUFFER_SIZE / 2);
            BasicMix(&normalizedBufferADC1[BUFFER_SIZE / 2],
                     &normalizedBufferADC2[BUFFER_SIZE / 2],
                     &normalizedExitBuffer[BUFFER_SIZE / 2],
                     BUFFER_SIZE / 2);
            DelayFxProcess(&normalizedExitBuffer[BUFFER_SIZE / 2],  BUFFER_SIZE / 2);
            DenormalizeBuffer(&exitBuffer[BUFFER_SIZE / 2],
                            &normalizedExitBuffer[BUFFER_SIZE / 2],
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
