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
#include "i2c.h"

// addr = 0xD0


void DS3231_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	
	// Init IO pins
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0}; 
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	// RESET
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,DISABLE);
	
	I2C_InitTypeDef I2C_InitStruct = {0};
	
	// 400 khz
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                        
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
    I2C_InitStruct.I2C_OwnAddress1 = 0x00; 
	I2C_Init(I2C1,&I2C_InitStruct);
	
	I2C_Cmd(I2C1,ENABLE);

    // set up
    DS3231_EnableOscillator(I2C1);
    DS3231_RewriteOCF(I2C1);


    DS3231_SetTime(I2C1,0xD0,0x00,0x00,0x00,0x12,0x01,0x01,0x26);


}



void DS3231_EnableOscillator(I2C_TypeDef *I2Cx) 
{
    // 0x0E control register
    uint8_t control = DS3231_ReadRegister(I2Cx, 0xD0, 0x0E);
    
    control &= 0x7F; 
    
    DS3231_WriteRegister(I2Cx, 0xD0, 0x0E, control);
}

void DS3231_RewriteOCF(I2C_TypeDef *I2Cx)
{
    // 0x0F status register
    uint8_t status = DS3231_ReadRegister(I2Cx, 0xD0, 0x0F);

    // set time only at beginning
    if(status & 0x80)
    {
        DS3231_SetTime(I2C1,0xD0,0x00,0x00,0x00,0x12,0x01,0x01,0x26);
    }
    
    status &= 0x07; 
    
    DS3231_WriteRegister(I2Cx, 0xD0, 0x0F, status);

}

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
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    return data;

}


uint8_t Dec_to_BCD(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

uint8_t BCD_to_Dec(uint8_t bcd)
{

    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}



int DS3231_SetTime(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t commands,
                     uint8_t sec, uint8_t min, uint8_t hour, 
                     uint8_t day, uint8_t date, uint8_t month, uint8_t year) {

    uint8_t timeData[8] = {0};
    timeData[0] = commands;
    timeData[1] = Dec_to_BCD(sec);
    timeData[2] = Dec_to_BCD(min);
    timeData[3] = Dec_to_BCD(hour); // 24hr mode
    timeData[4] = Dec_to_BCD(day);
    timeData[5] = Dec_to_BCD(date);
    timeData[6] = Dec_to_BCD(month);
    timeData[7] = Dec_to_BCD(year);

    // debug purposes

    return My_I2C_SendBytes(I2C1,Addr,timeData,8);


}


