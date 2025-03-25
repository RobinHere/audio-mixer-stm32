#include "eq_fx.h"

void EqualizerInit(EqualizerFX* equalizerFX) {
    equalizerFX->gainAmpHighShelf = 1.0f;
    equalizerFX->gainAmpPeaking = 1.0f;
    equalizerFX->gainAmpLowShelf = 1.0f;
    equalizerFX->cutoffFreqHighShelf = 10000;
    equalizerFX->centerFreqPeaking = 5000;
    equalizerFX->cutoffFreqLowShelf = 300;
    equalizerFX->QPeaking = 0.59f;
}

static void BiquadFilter(volatile float* audioBuffer, uint16_t bufferSize, float b0, float b1,
                         float b2, float a0, float a1, float a2, BiquadState* state) {
    for (uint16_t i = 0; i < bufferSize; i++) {
        float currentInput = audioBuffer[i];
        float currentOutput = (b0 / a0) * currentInput + (b1 / a0) * state->previousInput1
                + (b2 / a0) * state->previousInput2 - (a1 / a0) * state->previousOutput1
                - (a2 / a0) * state->previousOutput2;

        state->previousInput2 = state->previousInput1;
        state->previousInput1 = currentInput;
        state->previousOutput2 = state->previousOutput1;
        state->previousOutput1 = currentOutput;

        if (currentOutput > 1.0f) {
        currentOutput = 1.0f;
        } 
        if (currentOutput < -1.0f) {
        currentOutput = -1.0f;
        }

        audioBuffer[i] = currentOutput;
    }
}

static void LowShelfFilter(volatile float* audioBuffer, uint16_t bufferSize, float gain,
                           float cutoffFreq, float sampleRate, BiquadState* state) {
    float A = sqrtf(gain);
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float cos_w0 = cosf(w0);
    float sin_w0 = sinf(w0);
    float alpha = sin_w0 / 2.0f * sqrtf((A + 1.0f / A) * (1.0f / 1.0f - 1.0f) + 2.0f);

    float b0 = A * ((A + 1.0f) - (A - 1.0f) * cos_w0 + 2.0f * sqrtf(A) * alpha);
    float b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cos_w0);
    float b2 = A * ((A + 1.0f) - (A - 1.0f) * cos_w0 - 2.0f * sqrtf(A) * alpha);
    float a0 = (A + 1.0f) + (A - 1.0f) * cos_w0 + 2.0f * sqrtf(A) * alpha;
    float a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cos_w0);
    float a2 = (A + 1.0f) + (A - 1.0f) * cos_w0 - 2.0f * sqrtf(A) * alpha;

    BiquadFilter(audioBuffer, bufferSize, b0, b1, b2, a0, a1, a2, state);
}

static void PeakingEQFilter(volatile float* audioBuffer, uint16_t bufferSize, float gain,
                            float centerFreq, float Q, float sampleRate, BiquadState* state) {
    float A = sqrtf(gain);
    float w0 = 2.0f * M_PI * centerFreq / sampleRate;
    float cos_w0 = cosf(w0);
    float sin_w0 = sinf(w0);
    float alpha = sin_w0 / (2.0f * Q);

    float b0 = 1.0f + alpha * A;
    float b1 = -2.0f * cos_w0;
    float b2 = 1.0f - alpha * A;
    float a0 = 1.0f + alpha / A;
    float a1 = -2.0f * cos_w0;
    float a2 = 1.0f - alpha / A;

    BiquadFilter(audioBuffer, bufferSize, b0, b1, b2, a0, a1, a2, state);
}

static void HighShelfFilter(volatile float* audioBuffer, uint16_t bufferSize, float gain,
                            float cutoffFreq, float sampleRate, BiquadState* state) {
    float A = sqrtf(gain);
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float cos_w0 = cosf(w0);
    float sin_w0 = sinf(w0);
    float alpha = sin_w0 / 2.0f * sqrtf((A + 1.0f / A) * (1.0f / 1.0f - 1.0f) + 2.0f);

    float b0 = A * ((A + 1.0f) + (A - 1.0f) * cos_w0 + 2.0f * sqrtf(A) * alpha);
    float b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cos_w0);
    float b2 = A * ((A + 1.0f) + (A - 1.0f) * cos_w0 - 2.0f * sqrtf(A) * alpha);
    float a0 = (A + 1.0f) - (A - 1.0f) * cos_w0 + 2.0f * sqrtf(A) * alpha;
    float a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cos_w0);
    float a2 = (A + 1.0f) - (A - 1.0f) * cos_w0 - 2.0f * sqrtf(A) * alpha;

    BiquadFilter(audioBuffer, bufferSize, b0, b1, b2, a0, a1, a2, state);
}

void updateGainLowShelf(EqualizerFX* equalizerFX, float gainAmp) {
    equalizerFX->gainAmpLowShelf = gainAmp;
}

void updateGainPeaking(EqualizerFX* equalizerFX, float gainAmp) {
    equalizerFX->gainAmpPeaking = gainAmp;
}

void updateGainHighShelf(EqualizerFX* equalizerFX, float gainAmp) {
    equalizerFX->gainAmpHighShelf = gainAmp;
}

void EqualizerFxProcess(EqualizerFX* equalizerFX, volatile float* audioBuffer, uint16_t bufferSize,
                        float sampleRate) {
    static BiquadState lowShelfState = {0.0f, 0.0f, 0.0f, 0.0f};
    static BiquadState peakState = {0.0f, 0.0f, 0.0f, 0.0f};
    static BiquadState highShelfState = {0.0f, 0.0f, 0.0f, 0.0f};

    LowShelfFilter(audioBuffer, bufferSize, equalizerFX->gainAmpLowShelf,
    equalizerFX->cutoffFreqLowShelf, sampleRate, &lowShelfState);

    PeakingEQFilter(audioBuffer, bufferSize, equalizerFX->gainAmpPeaking,
    equalizerFX->centerFreqPeaking, equalizerFX->QPeaking, sampleRate, &peakState);

    HighShelfFilter(audioBuffer, bufferSize, equalizerFX->gainAmpHighShelf,
    equalizerFX->cutoffFreqHighShelf, sampleRate, &highShelfState);
}
