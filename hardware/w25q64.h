/**
  ******************************************************************************
  * @file    w25q64.h
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.13.2026
  * @brief   w25q64 header file
  ******************************************************************************
  */



#ifndef _W25Q64_H
#define _W25Q64_H
#include "stm32f10x.h"
#include "oled.h"


extern OLED_TypeDef oled;

// adjust based on your connection

// SPI

#define W25Q64_PORT GPIOA
#define W25Q64_SPIPORT SPI1
#define W25Q64_SPI_CLK RCC_APB2Periph_SPI1
#define W25Q64_PIN_CLK RCC_APB2Periph_GPIOA

#define W25Q64_NSS GPIO_Pin_4
#define W25Q64_SCK GPIO_Pin_5
#define W25Q64_MISO GPIO_Pin_6
#define W25Q64_MOSI GPIO_Pin_7

void W25Q64_SPI_Init(void);
void W25Q64_Save4Bytes(const uint8_t *byte);
void W25Q64_LoadBytes(uint8_t *bytes);
float LoadTemperature(void);

#endif



