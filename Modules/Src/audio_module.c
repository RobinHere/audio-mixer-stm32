#include "audio_module.h"

EqualizerFX equalizerFX;
DelayFx delayFx;

static volatile float RightInputFader1GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInputFader1GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float RightInputFader2GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInputFader2GainAmp = VOLUME_DEFAULT_GAIN;
static volatile float OutputFaderGainAmp = VOLUME_DEFAULT_GAIN;

static volatile float RightInput1GainPreAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInput1GainPreAmp = VOLUME_DEFAULT_GAIN;
static volatile float RightInput2GainPreAmp = VOLUME_DEFAULT_GAIN;
static volatile float LeftInput2GainPreAmp = VOLUME_DEFAULT_GAIN;

static volatile float LeftInput1Pan = DEFAULT_PAN;
static volatile float RightInput1Pan = DEFAULT_PAN;
static volatile float LeftInput2Pan = DEFAULT_PAN;
static volatile float RightInput2Pan = DEFAULT_PAN;

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
        normalizedBuffer[i] *= OutputFaderGainAmp;
        destinationBuffer[i] = (int16_t)(normalizedBuffer[i] * INT_16_MAX_VALUE);
    }
}

static void PreAmpGain(volatile float* sourceBuffer1, volatile float* sourceBuffer2,
                       uint16_t bufferSize) {
    for (uint16_t i = 0; i < bufferSize; i++) {
        if (i % 2 == 0) {
            sourceBuffer1[i] *= LeftInput1GainPreAmp;
            sourceBuffer2[i] *= LeftInput2GainPreAmp;
        } else {
            sourceBuffer1[i] *= RightInput1GainPreAmp;
            sourceBuffer2[i] *= RightInput2GainPreAmp;
        }
    }
}

static void BasicMix(volatile float* sourceBuffer1, volatile float* sourceBuffer2,
                     volatile float* destinationBuffer, uint16_t bufferSize) {
    uint16_t rightIndex = 0;
    uint16_t leftIndex = 0;

    float in1L = 0.0f, in1R = 0.0f, in2L = 0.0f, in2R = 0.0f;
    float in1LtoL, in1LtoR, in1RtoL, in1RtoR;
    float in2LtoL, in2LtoR, in2RtoL, in2RtoR;
    float mixedL = 0.0f, mixedR = 0.0f;

    for (uint16_t i = 0; i <= bufferSize; i += 2) {
        leftIndex = i;
        rightIndex = i + 1;

        in1L = sourceBuffer1[leftIndex]  * LeftInputFader1GainAmp;
        in1R = sourceBuffer1[rightIndex] * RightInputFader1GainAmp;
        in2L = sourceBuffer2[leftIndex]  * LeftInputFader2GainAmp;
        in2R = sourceBuffer2[rightIndex] * RightInputFader2GainAmp;

        in1LtoL = in1L * (LeftInput1Pan <= 1.0f ? 1.0f : 2.0f - LeftInput1Pan);
        in1LtoR = in1L * (LeftInput1Pan >= 1.0f ? 1.0f : LeftInput1Pan);

        in1RtoL = in1R * (RightInput1Pan <= 1.0f ? 1.0f : 2.0f - RightInput1Pan);
        in1RtoR = in1R * (RightInput1Pan >= 1.0f ? 1.0f : RightInput1Pan);

        in2LtoL = in2L * (LeftInput2Pan <= 1.0f ? 1.0f : 2.0f - LeftInput2Pan);
        in2LtoR = in2L * (LeftInput2Pan >= 1.0f ? 1.0f : LeftInput2Pan);

        in2RtoL = in2R * (RightInput2Pan <= 1.0f ? 1.0f : 2.0f - RightInput2Pan);
        in2RtoR = in2R * (RightInput2Pan >= 1.0f ? 1.0f : RightInput2Pan);

        mixedL = in1LtoL + in1RtoL + in2LtoL + in2RtoL;
        mixedR = in1LtoR + in1RtoR + in2LtoR + in2RtoR;

        destinationBuffer[leftIndex] = fmaxf(-1.0f, fminf(1.0f, mixedL));
        destinationBuffer[rightIndex] = fmaxf(-1.0f, fminf(1.0f, mixedR));
    }
}

static void StartAudioRxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* entryBuffer, uint16_t bufferSize) {
    HAL_SAI_Receive_DMA(saiBlock, (uint8_t*)entryBuffer, bufferSize);
}

static void StartAudioTxTransmission(SAI_HandleTypeDef* saiBlock, volatile int16_t* exitBuffer, uint16_t bufferSize) {
    HAL_SAI_Transmit_DMA(saiBlock, (uint8_t*)exitBuffer, bufferSize);
}

void UpdatePreAmpGainFromSlider(GainType channel, float GainAmp) {
    switch(channel) {
        case GAIN_CH1L:
            LeftInput1GainPreAmp = GainAmp;
            break;
        case GAIN_CH1R:
            RightInput1GainPreAmp = GainAmp;
            break;
        case GAIN_CH2L:
            LeftInput2GainPreAmp = GainAmp;
            break;
        case GAIN_CH2R:
            RightInput2GainPreAmp = GainAmp;
            break;
        default:
            break;
    }
}

void UpdateFaderGainFromSlider(GainType channel, float GainAmp) {
    switch(channel) {
        case GAIN_OUTPUT:
            OutputFaderGainAmp = GainAmp;
            break;
        case GAIN_CH1L:
            LeftInputFader1GainAmp = GainAmp;
            break;
        case GAIN_CH1R:
            RightInputFader1GainAmp = GainAmp;
            break;
        case GAIN_CH2L:
            LeftInputFader2GainAmp = GainAmp;
            break;
        case GAIN_CH2R:
            RightInputFader2GainAmp = GainAmp;
            break;
        default:
            break;
    }
}

void UpdatePanFromSlider(PanChannel channel, float PanRatio) {
    switch(channel) {
        case PAN_CH1L:
            LeftInput1Pan = PanRatio;
            break;
        case PAN_CH1R:
            RightInput1Pan = PanRatio;
            break;
        case PAN_CH2L:
            LeftInput2Pan  = PanRatio;
            break;
        case PAN_CH2R:
            RightInput2Pan  = PanRatio;
            break;
        default:
            break;
    }
}

void AudioInit() {
    DelayFxInit(&delayFx, 0.5f, 0.5f, 0.5f);
    EqualizerInit(&equalizerFX);
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
            PreAmpGain(&normalizedBufferADC1[0],
                       &normalizedBufferADC2[0],
                       BUFFER_SIZE / 2);
            BasicMix(&normalizedBufferADC1[0],
                     &normalizedBufferADC2[0],
                     &normalizedExitBuffer[0],
                     BUFFER_SIZE / 2);
            DelayFxProcess(&delayFx, &normalizedExitBuffer[0],  BUFFER_SIZE / 2);
            EqualizerFxProcess(&equalizerFX,
                               &normalizedExitBuffer[0],
                               BUFFER_SIZE / 2,
                               48000.0f);
            DenormalizeBuffer(&exitBuffer[0],
                              &normalizedExitBuffer[0],
                              BUFFER_SIZE / 2);
        } else {
            NormalizeEntryBuffers(&entryBufferADC1[BUFFER_SIZE / 2],
                                  &entryBufferADC2[BUFFER_SIZE / 2],
                                  &normalizedBufferADC1[BUFFER_SIZE / 2],
                                  &normalizedBufferADC2[BUFFER_SIZE / 2],
                                  BUFFER_SIZE / 2);
            PreAmpGain(&normalizedBufferADC1[BUFFER_SIZE / 2],
                       &normalizedBufferADC2[BUFFER_SIZE / 2],
                       BUFFER_SIZE / 2);
            BasicMix(&normalizedBufferADC1[BUFFER_SIZE / 2],
                     &normalizedBufferADC2[BUFFER_SIZE / 2],
                     &normalizedExitBuffer[BUFFER_SIZE / 2],
                     BUFFER_SIZE / 2);
            DelayFxProcess(&delayFx, &normalizedExitBuffer[BUFFER_SIZE / 2],  BUFFER_SIZE / 2);
            EqualizerFxProcess(&equalizerFX,
                               &normalizedExitBuffer[BUFFER_SIZE / 2],
                               BUFFER_SIZE / 2,
                               48000.0f);
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
