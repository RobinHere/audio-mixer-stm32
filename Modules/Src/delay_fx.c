#include "delay_fx.h"

static DelayFx delayFx;

void DelayFxInit(float delayTimeSeconds, float wetness, float feedback) {
    delayFx.delayTimeSeconds = delayTimeSeconds;
    if (delayTimeSeconds > MAX_DELAY_TIME_SECONDS) {
        delayFx.delayTimeSeconds = 1.0f;
    }
    delayFx.delayWetMix = wetness;
    delayFx.delayDryMix = 1.0f - wetness;
    delayFx.delayFeedback = feedback;
    if (feedback > 1.0f) {
        delayFx.delayFeedback = 1.0f;
    }
    for (uint32_t i = 0; i < MAX_DELAY_BUFFER_SIZE; i++) {
        delayFx.delayBuffer[i] = 0.0f;
    }
    delayFx.realSizeDelayBuffer = (uint32_t)(SAMPLE_RATE_HZ * delayTimeSeconds);
    delayFx.delayBufferIndex = 0;
    delayFx.delayFxStatus = DELAY_OFF;
}

void DelayFxProcess(volatile float* audioBuffer, uint16_t bufferSize) {
    if (delayFx.delayFxStatus == DELAY_ON) {
        for(uint16_t i = 0; i < bufferSize; i++) {
            audioBuffer[i] = (delayFx.delayBuffer[delayFx.delayBufferIndex] * delayFx.delayWetMix) +
                             (audioBuffer[i] * delayFx.delayDryMix);
            
            delayFx.delayBuffer[delayFx.delayBufferIndex] = delayFx.delayFeedback *
                (delayFx.delayBuffer[delayFx.delayBufferIndex] + audioBuffer[i]);
            
            if (delayFx.delayFeedback > 0.55) {
                delayFx.delayBuffer[delayFx.delayBufferIndex] *= 0.5f;
            }
            
            if (delayFx.delayBuffer[delayFx.delayBufferIndex] > 1.0f) {
                delayFx.delayBuffer[delayFx.delayBufferIndex] = 1.0f;
            }
                
            if (delayFx.delayBuffer[delayFx.delayBufferIndex] < -1.0f) {
                delayFx.delayBuffer[delayFx.delayBufferIndex] = -1.0f;
            }
            
            delayFx.delayBufferIndex++;
            if (delayFx.delayBufferIndex >= delayFx.realSizeDelayBuffer) delayFx.delayBufferIndex = 0;
        }
    }
}

void DelayFxUpdateDelayTime(float delayTimeSeconds) {
    delayFx.delayTimeSeconds = delayTimeSeconds;
    if (delayTimeSeconds > MAX_DELAY_TIME_SECONDS) {
        delayFx.delayTimeSeconds = 1.0f;
    }
    delayFx.realSizeDelayBuffer = (uint32_t)(SAMPLE_RATE_HZ * delayTimeSeconds);
}

void DelayFxUpdateFeedback(float feedback) {
    delayFx.delayFeedback = feedback;
    if (feedback > 1.0f) {
        delayFx.delayFeedback = 1.0f;
    }
}

void DelayFxUpdateWetness(float wetness) {
    delayFx.delayWetMix = wetness;
    delayFx.delayDryMix = 1.0f - wetness;
}

void DelayFxToggleOnOff(DelayFxStatus status) {
    delayFx.delayFxStatus = status;
}

DelayFxStatus GetDelayStatus() {
    return delayFx.delayFxStatus;
}