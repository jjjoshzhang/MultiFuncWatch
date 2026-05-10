/**
  ******************************************************************************
  * @file    w25q64.c
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.12.2026
  * @brief   w25q64 source file
  ******************************************************************************
  */
#include <string.h>
#include "stm32f10x.h"
#include "oled.h"
#include "spi.h"
#include "w25q64.h"
#include "delay.h"



void W25Q64_SPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(W25Q64_PIN_CLK,ENABLE);

	GPIO_InitStruct.GPIO_Pin = W25Q64_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(W25Q64_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin = W25Q64_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(W25Q64_PORT,&GPIO_InitStruct);

	
	GPIO_InitStruct.GPIO_Pin = W25Q64_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(W25Q64_PORT,&GPIO_InitStruct);
	

	GPIO_InitStruct.GPIO_Pin = W25Q64_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	
	GPIO_Init(W25Q64_PORT,&GPIO_InitStruct);
	
    // Set high
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	
	// SPI Init
	
	RCC_APB2PeriphClockCmd(W25Q64_SPI_CLK,ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct = {0};
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // 1.125MHz
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(W25Q64_SPIPORT,&SPI_InitStruct);

    SPI_Cmd(W25Q64_SPIPORT, ENABLE);
	
    // set high
	SPI_NSSInternalSoftwareConfig(W25Q64_SPIPORT,SPI_NSSInternalSoft_Set);
	

} 


void W25Q64_Save4Bytes(const uint8_t *byte)
{
	uint8_t buffer[10];
	// 1. Write Enable
	// Enable Commands
	buffer[0] = 0x06;
	// choose W25Q64
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
	Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
	// Close
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	// 2. Sector Erase
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
	// Erase Commands
	buffer[0] = 0x20;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,4);
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	// 3. Wait till Busy 1 -> 0 
	while(1)
	{
		GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
		// Send Read Register Commands
		buffer[0] = 0x05;
		Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
		// Read Register 1
		buffer[0] = 0xff;
		Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
		GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
		
		if((buffer[0] & 0x01) == 0) break;

	}
	// 4. Write Enable
	// Enable Commands
	buffer[0] = 0x06;
	// choose W25Q64
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
	Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
	// Close
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	// 5. Page program
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
	// Program Commands
	buffer[0] = 0x02;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	// actual Data
	buffer[4] = byte[0];
    buffer[5] = byte[1];
    buffer[6] = byte[2];
    buffer[7] = byte[3];

	Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,8);
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	// 6. Wait till Busy 1 -> 0 
	while(1)
	{
		GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
		// Send Read Register Commands
		buffer[0] = 0x05;
		Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
		// Read Register 1
		buffer[0] = 0xff;
		Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,1);
		GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
		
		if((buffer[0] & 0x01) == 0) break;

	}
	

}

void W25Q64_LoadBytes(uint8_t *bytes)
{
	uint8_t buffer[10];
	
	// Read Data Commands
	buffer[0] = 0x03;
	// 24-bit address
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;

	buffer[4] = 0xFF;
	buffer[5] = 0xFF;
	buffer[6] = 0xFF;
	buffer[7] = 0xFF;
	// Read Data
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_RESET);
	
	Myy_SPI_MasterTransmitReceive(W25Q64_SPIPORT,buffer, buffer,8);
	
	GPIO_WriteBit(W25Q64_PORT,W25Q64_NSS,Bit_SET);
	
	bytes[0] = buffer[4];
	bytes[1] = buffer[5];
	bytes[2] = buffer[6];
	bytes[3] = buffer[7];
	
	
}


float LoadTemperature(void)
{
    uint8_t bytes[4];
    float value;

    W25Q64_LoadBytes(bytes);
		uint32_t f;
//		f = *(uint32_t*)&bytes;
	f = ((uint32_t)bytes[0]<<24) 
	| ((uint32_t)bytes[1]<<16) 
	| ((uint32_t)bytes[2]<<8) 
	| ((uint32_t)bytes[3]);
    memcpy(&value, &f, sizeof(float));

    return value;
}

