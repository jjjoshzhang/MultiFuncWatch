/**
  ******************************************************************************
  * @file    ds3231.c
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.14.2026
  * @brief   ds3231 source file
  ******************************************************************************
  */

#include "ds3231.h"


uint8_t DS3231_RepStart(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t regAddr) {

    uint8_t data = 0;
    
    // sending phase 
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);
	I2C_GenerateSTART(I2Cx, ENABLE);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET);
    I2C_SendData(I2Cx, Addr & 0xfe);

    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET);
		
    I2C_ReadRegister(I2Cx, I2C_Register_SR1);
	I2C_ReadRegister(I2Cx, I2C_Register_SR2);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) == RESET);
    I2C_SendData(I2Cx, regAddr);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET);

	// reading phase
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET);
	I2C_SendData(I2Cx, Addr | 0x01);
	
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET);
		
	I2C_AcknowledgeConfig(I2Cx, DISABLE); // NACK
		
	I2C_ReadRegister(I2Cx, I2C_Register_SR1);
	I2C_ReadRegister(I2Cx, I2C_Register_SR2);
		
	I2C_GenerateSTOP(I2Cx, ENABLE);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
	data = I2C_ReceiveData(I2Cx);
    return data;

}
	
	

