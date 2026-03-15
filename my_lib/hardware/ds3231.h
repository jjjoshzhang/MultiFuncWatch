/**
  ******************************************************************************
  * @file    ds3231.h
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.14.2026
  * @brief   ds3231 header file
  ******************************************************************************
  */

#ifndef _DS3231_H
#define _DS3231_H
#include "stm32f10x.h"





// requires Repeated START iic
int DS3231_RepStart(I2C_TypeDef *I2Cx, uint8_t regAddr, uint8_t *data);

#endif