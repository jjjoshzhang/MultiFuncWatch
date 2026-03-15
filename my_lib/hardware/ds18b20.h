/**
  ******************************************************************************
  * @file    ds18b20.h
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.12.2026
  * @brief   ds18b20 header file
  ******************************************************************************
  */

#ifndef _DS18B20_H
#define _DS18B20_H
#include "stm32f10x.h"


// adjust based on your connection

// make sure to add a 4.7k-5k ohms pull up resistor for DQ
#define DS18B20_PORT GPIOB
#define DS18B20_PIN GPIO_Pin_0
#define DS18B20_PIN_CLK RCC_APB2Periph_GPIOB

#define ds18b20_Low() GPIO_ResetBits(DS18B20_PORT, DS18B20_PIN)
#define ds18b20_High() GPIO_SetBits(DS18B20_PORT, DS18B20_PIN)
#define ds18b20_Read() GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_PIN)

void ds18b20_GPIO_Init(void);
void SetPinOutput(void);
void SetPinInput(void);
int ds18b20_Check(void);
int ds18b20_Init(void);
void ds18b20_Start(void);
void ds18b20_Finish(void);
void ds18b20_WriteBit(uint8_t bit);
void ds18b20_WriteByte(uint8_t byte);
uint8_t ds18b20_ReadBit(void);
uint8_t ds18b20_ReadByte(void);
float ds18b20_GetTemp(void);


#endif 

