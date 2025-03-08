/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.22.0. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <KeySampler.hpp>
#include <cmsis_os2.h>
#include "main.h"
#include "FreeRTOS.h"
#include "stm32h5xx_ll_tim.h"

using namespace touchgfx;

extern "C" DMA_HandleTypeDef handle_GPDMA1_Channel6;
static DMA_HandleTypeDef* const DISPLAY_DMA = &handle_GPDMA1_Channel6;
static TIM_TypeDef* const LINE_TIMER = TIM7;

void initLCD(void);
static void DMA_TxCpltCallback(DMA_HandleTypeDef* hdma);
static void DMA_TxErrorCallback(DMA_HandleTypeDef* hdma);
void setDisplayWindow(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
extern "C" void DisplayDriver_TransferCompleteCallback(void);
static int32_t LCD_IO_SendDataDMA(uint8_t* pData, uint32_t Length);

static volatile bool isTransmittingBlock = false;
static volatile uint16_t tearingEffectCount = 0;
static volatile bool startRenderingImmediately = false;
KeySampler btnctrl;
osSemaphoreId_t xDisplayTransmitSemaphore = NULL;
extern bool newTouch;

void TouchGFXHAL::initialize()
{
    initLCD();
    setButtonController(&btnctrl);

    HAL_DMA_RegisterCallback(DISPLAY_DMA, HAL_DMA_XFER_CPLT_CB_ID, DMA_TxCpltCallback);
    HAL_DMA_RegisterCallback(DISPLAY_DMA, HAL_DMA_XFER_ERROR_CB_ID, DMA_TxErrorCallback);

    instrumentation.init();
    setMCUInstrumentation(&instrumentation);
    enableMCULoadCalculation(true);

    xDisplayTransmitSemaphore = osSemaphoreNew(1, 0, NULL);

    TouchGFXGeneratedHAL::initialize();
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const
{
    // Calling parent implementation of getTFTFrameBuffer().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    return TouchGFXGeneratedHAL::getTFTFrameBuffer();
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t* address)
{
    // Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    // Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
    // be called to notify the touchgfx framework that flush has been performed.
    // To calculate he start adress of rect,
    // use advanceFrameBufferToRect(uint8_t* fbPtr, const touchgfx::Rect& rect)
    // defined in TouchGFXGeneratedHAL.cpp

    TouchGFXGeneratedHAL::flushFrameBuffer(rect);
}

bool TouchGFXHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
    return TouchGFXGeneratedHAL::blockCopy(dest, src, numBytes);
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts()
{
    // Calling parent implementation of configureInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::configureInterrupts();
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts()
{
    // Calling parent implementation of enableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::enableInterrupts();
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts()
{
    // Calling parent implementation of disableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::disableInterrupts();
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt()
{
    // Calling parent implementation of enableLCDControllerInterrupt().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}

bool TouchGFXHAL::beginFrame()
{
    tearingEffectCount = 0;
    return TouchGFXGeneratedHAL::beginFrame();
}

void TouchGFXHAL::endFrame()
{
    TouchGFXGeneratedHAL::endFrame();
}

static __attribute__((noinline)) void FMC_BANK1_WriteReg(volatile uint8_t Reg)
{
    /* Write 8-bits Index, then write register */
    *(uint8_t*)(FMC_BANK1_REG) = Reg;
    __DSB();
}

static __attribute__((noinline)) void FMC_BANK1_WriteData(volatile uint8_t Data)
{
    /* Write 8-bits Reg */
    *(uint8_t*)(FMC_BANK1_MEM) = Data;
    __DSB();
}

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void DMA_TxCpltCallback(DMA_HandleTypeDef* hdma)
{
    if (hdma == DISPLAY_DMA)
    {
        /* Signal Transfer Done Event */
        isTransmittingBlock = false;

        DisplayDriver_TransferCompleteCallback();

        osSemaphoreRelease(xDisplayTransmitSemaphore);
    }
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void DMA_TxErrorCallback(DMA_HandleTypeDef* hdma)
{
    __disable_irq();
    while (1)
    {
    }
}

static int32_t LCD_IO_SendDataDMA(uint8_t* pData, uint32_t Length)
{
    if (HAL_DMA_Start_IT(DISPLAY_DMA, (uint32_t)pData, (uint32_t)FMC_BANK1_MEM, Length) != HAL_OK)
    {
        /* Transfer Error */
        return ERROR_BUS_FAILURE;
    }

    return 0;
}

void initLCD(void)
{
    /* Reset DISPLAY */
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(10); /* wait at least 10us according DISPLAY datasheet */
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(120); /* wait maximum 120ms according DISPLAY datasheet */

    /* set p-gamma */
    FMC_BANK1_WriteReg(0xE0);
    FMC_BANK1_WriteData(0x00);
    FMC_BANK1_WriteData(0x10);
    FMC_BANK1_WriteData(0x14);
    FMC_BANK1_WriteData(0x01);
    FMC_BANK1_WriteData(0x0E);
    FMC_BANK1_WriteData(0x04);
    FMC_BANK1_WriteData(0x33);
    FMC_BANK1_WriteData(0x56);
    FMC_BANK1_WriteData(0x48);
    FMC_BANK1_WriteData(0x03);
    FMC_BANK1_WriteData(0x0C);
    FMC_BANK1_WriteData(0x0B);
    FMC_BANK1_WriteData(0x2B);
    FMC_BANK1_WriteData(0x34);
    FMC_BANK1_WriteData(0x0F);

    /* set n-gamma */
    FMC_BANK1_WriteReg(0XE1);
    FMC_BANK1_WriteData(0x00);
    FMC_BANK1_WriteData(0x12);
    FMC_BANK1_WriteData(0x18);
    FMC_BANK1_WriteData(0x05);
    FMC_BANK1_WriteData(0x12);
    FMC_BANK1_WriteData(0x06);
    FMC_BANK1_WriteData(0x40);
    FMC_BANK1_WriteData(0x34);
    FMC_BANK1_WriteData(0x57);
    FMC_BANK1_WriteData(0x06);
    FMC_BANK1_WriteData(0x10);
    FMC_BANK1_WriteData(0x0C);
    FMC_BANK1_WriteData(0x3B);
    FMC_BANK1_WriteData(0x3F);
    FMC_BANK1_WriteData(0x0F);

    /* set power control 1 */
    FMC_BANK1_WriteReg(0XC0);
    FMC_BANK1_WriteData(0x0F);
    FMC_BANK1_WriteData(0x0C);

    /* set power control 2 */
    FMC_BANK1_WriteReg(0xC1);
    FMC_BANK1_WriteData(0x41);

    /* set VCOM control */
    FMC_BANK1_WriteReg(0xC5);
    FMC_BANK1_WriteData(0x00);
    FMC_BANK1_WriteData(0x25);
    FMC_BANK1_WriteData(0x80);

    /* memory access */
    FMC_BANK1_WriteReg(0x36);
    FMC_BANK1_WriteData(0x48);

    /* pixel format */
    FMC_BANK1_WriteReg(0x3A);
    FMC_BANK1_WriteData(0x55);

    /* interface mode control */
    FMC_BANK1_WriteReg(0xB0);
    FMC_BANK1_WriteData(0x00);

    /* frame rate control */
    FMC_BANK1_WriteReg(0xB1);
    FMC_BANK1_WriteData(0xA0);

    /* display function control */
    FMC_BANK1_WriteReg(0xB6);
    FMC_BANK1_WriteData(0x02);
    FMC_BANK1_WriteData(0x02);

    /* display inversion on */
    FMC_BANK1_WriteReg(0x21);

    /* sleep out */
    FMC_BANK1_WriteReg(0x11);
    HAL_Delay(120);

    /* tearing effect line on */
    FMC_BANK1_WriteReg(0x35);
    FMC_BANK1_WriteData(0x00);

    /* Set Tearing Effect */
    FMC_BANK1_WriteReg(DISPLAY_TEON);
    FMC_BANK1_WriteData(0x00);  // TE Mode 1

    /* Turn On Display */
    FMC_BANK1_WriteReg(DISPLAY_ON);
}

void setDisplayWindow(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
    // Set pixel x position
    FMC_BANK1_WriteReg(DISPLAY_CASET);
    FMC_BANK1_WriteData((uint8_t)(Xpos >> 8U));                    /* XS[15:8] */
    FMC_BANK1_WriteData((uint8_t)(Xpos & 0xFFU));                  /* XS[7:0] */
    FMC_BANK1_WriteData((uint8_t)((Xpos + Width - 1U) >> 8U));     /* XE[15:8] */
    FMC_BANK1_WriteData((uint8_t)((Xpos + Width - 1U) & 0xFFU));   /* XE[7:0] */

    // Set pixel y position
    FMC_BANK1_WriteReg(DISPLAY_RASET);
    FMC_BANK1_WriteData((uint8_t)(Ypos >> 8U));                    /* YS[15:8] */
    FMC_BANK1_WriteData((uint8_t)(Ypos & 0xFFU));                  /* YS[7:0] */
    FMC_BANK1_WriteData((uint8_t)((Ypos + Height - 1U) >> 8U));    /* YE[15:8] */
    FMC_BANK1_WriteData((uint8_t)((Ypos + Height - 1U) & 0xFFU));  /* YE[7:0] */

    FMC_BANK1_WriteReg(DISPLAY_GRAM);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == LCD_TE_Pin)
    {
        // The draw speed of displays varies based on factors such as design, production variance, temperature and so on
        // to achieve good performance on all displays the timer tick rate needs to be adjusted based on the actual draw time of the latest frame
        // The following code dynamically adjusts the prescaler on every new frame to match the draw rate
        static const uint32_t TARGET = 480;         // Number of lines in display drawing direction
        volatile uint32_t count = LL_TIM_GetCounter(LINE_TIMER);
        if (count < TARGET - 1 || count > TARGET + 1)
        {
            const uint16_t oldPrescaler = LL_TIM_GetPrescaler(LINE_TIMER);
            const uint16_t newPrescaler = (count * oldPrescaler + TARGET / 2) / TARGET;

            // Check if the proposed new prescaler is in a reasonable range
            if (newPrescaler > 5000 && newPrescaler < 11000)
            {
                LL_TIM_SetPrescaler(LINE_TIMER, newPrescaler);
                LL_TIM_GenerateEvent_UPDATE(LINE_TIMER); // Reinitialize the counter and generates an update of the registers
            }
        }
        LL_TIM_DisableCounter(LINE_TIMER);
        LL_TIM_SetCounter(LINE_TIMER, 0);

        tearingEffectCount++;

        /* VSync has occurred, increment TouchGFX engine vsync counter */
        touchgfx::HAL::getInstance()->vSync();

        /* VSync has occurred, signal TouchGFX engine */
        touchgfx::OSWrappers::signalVSync();

        GPIO::set(GPIO::VSYNC_FREQ);
        startRenderingImmediately = true;
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == LCD_TE_Pin)
    {
        LL_TIM_EnableCounter(LINE_TIMER);
        GPIO::clear(GPIO::VSYNC_FREQ);
        startRenderingImmediately = false;
    }
    else if (GPIO_Pin == TOUCH_IRQ_Pin)
    {
        newTouch = true;
    }
}

extern "C"
{
    portBASE_TYPE IdleTaskHook(void* p)
    {
        if ((int)p) //idle task sched out
        {
            touchgfx::HAL::getInstance()->setMCUActive(true);
        }
        else //idle task sched in
        {
            touchgfx::HAL::getInstance()->setMCUActive(false);
        }
        return pdTRUE;
    }
}

void TouchGFXHAL::taskEntry()
{
    enableLCDControllerInterrupt();
    enableInterrupts();
    for (;;)
    {
        if (!startRenderingImmediately) //if TE high start immediately, otherwise wait for next TE to synchronize with display
        {
            OSWrappers::waitForVSync();
        }
        startRenderingImmediately = false; // only start one new frame inside each TE high

        backPorchExited();
    }
}

// The TouchGFX framework calls this function to check if a transmission is in progress
extern "C" int touchgfxDisplayDriverTransmitActive()
{
    return isTransmittingBlock ? 1 : 0;
}

// The TouchGFX framework calls this function to start transmission of a framebuffer block
extern "C" void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    isTransmittingBlock = true;

    setDisplayWindow(x, y, w, h);

    LCD_IO_SendDataDMA((uint8_t*)pixels, w * h * 2);
}

// The TouchGFX framework calls this function to check if the transfer of a block should be started
extern "C" int touchgfxDisplayDriverShouldTransferBlock(uint16_t bottom)
{
    // Only allow block transfer if the display has drawn past the bottom of the requested block (plus a margin of two lines)
    // A timer is used to estimate how many lines have been drawn by setting the prescaler so the tick rate matches the line draw rate
    uint16_t lastLineDrawn = LL_TIM_GetCounter(LINE_TIMER);
    return bottom + 2 < lastLineDrawn || tearingEffectCount > 0;
}

// The TouchGFX framework calls this function to wait for the current transfer to be finished
void FrameBufferAllocatorWaitOnTransfer()
{
    osSemaphoreAcquire(xDisplayTransmitSemaphore, osWaitForever);
}

// The TouchGFX framework calls this function to wait for all transfers to be finished
void waitUntilTransmitEnd()
{
    while (isTransmittingBlock)
    {
        osSemaphoreAcquire(xDisplayTransmitSemaphore, osWaitForever);
    }
}

// The TouchGFX framework calls this function to sleep until the next block can be transfered
void waitUntilCanTransferBlock(uint16_t bottom)
{
    static const int LINES_DRAWN_PER_TICK = 30; // Number of display lines divided by the the number of ticks it takes to draw (e.g. 480 lines / 16 ticks)
    while (1)
    {
        const int numberOfLinesToWait = bottom - LL_TIM_GetCounter(LINE_TIMER);
        if (numberOfLinesToWait > LINES_DRAWN_PER_TICK)
        {
            OSWrappers::taskDelay(1);
        }
        else
        {
            break;
        }
    }
}

/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
