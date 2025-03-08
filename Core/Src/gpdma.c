/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpdma.c
  * @brief   This file provides code for the configuration
  *          of the GPDMA instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "gpdma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DMA_HandleTypeDef handle_GPDMA1_Channel6;

/* GPDMA1 init function */
void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel6_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel6.Instance = GPDMA1_Channel6;
  handle_GPDMA1_Channel6.Init.Request = DMA_REQUEST_SW;
  handle_GPDMA1_Channel6.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  handle_GPDMA1_Channel6.Init.Direction = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel6.Init.SrcInc = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel6.Init.DestInc = DMA_DINC_FIXED;
  handle_GPDMA1_Channel6.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel6.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel6.Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel6.Init.SrcBurstLength = 1;
  handle_GPDMA1_Channel6.Init.DestBurstLength = 1;
  handle_GPDMA1_Channel6.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  handle_GPDMA1_Channel6.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel6.Init.Mode = DMA_NORMAL;
  if (HAL_DMA_Init(&handle_GPDMA1_Channel6) != HAL_OK)
  {
    Error_Handler();
  }
  DataHandlingConfig.DataExchange = DMA_EXCHANGE_DEST_BYTE;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel6, &DataHandlingConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel6, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

}
/* GPDMA2 init function */
void MX_GPDMA2_Init(void)
{

  /* USER CODE BEGIN GPDMA2_Init 0 */

  /* USER CODE END GPDMA2_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA2_CLK_ENABLE();

  /* GPDMA2 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA2_Channel4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA2_Channel4_IRQn);
    HAL_NVIC_SetPriority(GPDMA2_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA2_Channel5_IRQn);

  /* USER CODE BEGIN GPDMA2_Init 1 */

  /* USER CODE END GPDMA2_Init 1 */
  /* USER CODE BEGIN GPDMA2_Init 2 */

  /* USER CODE END GPDMA2_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
