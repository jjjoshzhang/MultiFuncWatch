/**
  ******************************************************************************
  * @file    adxl345.h
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.13.2026
  * @brief   adxl345 header file
  ******************************************************************************
  */

#ifndef _ADXL345_H
#define _ADXL345_H
#include "stm32f10x.h"


// adjust based on your connection

// SPI Protocols

#define ADXL345_PORT GPIOA
#define ADXL345_SPIPORT SPI1
#define ADXL345_SPI_CLK RCC_APB2Periph_SPI1
#define ADXL345_PIN_CLK RCC_APB2Periph_GPIOA

#define ADXL345_NSS GPIO_Pin_4
#define ADXL345_SCK GPIO_Pin_5
#define ADXL345_MISO GPIO_Pin_6
#define ADXL345_MOSI GPIO_Pin_7

void ADXL345_SPI_Init(void);

#endif 