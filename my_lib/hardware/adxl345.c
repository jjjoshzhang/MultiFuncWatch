/**
  ******************************************************************************
  * @file    adxl345.c
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.13.2026
  * @brief   adxl345 source file
  ******************************************************************************
  */

#include "adxl345.h"
#include "delay.h"
#include "spi.h"
#include "stm32f10x.h"
#include "math.h"

#define M_PI 3.1415926f


void ADXL345_SPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(ADXL345_PIN_CLK,ENABLE);

	GPIO_InitStruct.GPIO_Pin = ADXL345_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin = ADXL345_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);

	
	GPIO_InitStruct.GPIO_Pin = ADXL345_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	

	GPIO_InitStruct.GPIO_Pin = ADXL345_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	
	GPIO_Init(ADXL345_PORT,&GPIO_InitStruct);
	
    // Set high
	GPIO_WriteBit(ADXL345_PORT,ADXL345_NSS,Bit_SET);
	
	
	// SPI Init
	
	RCC_APB2PeriphClockCmd(ADXL345_SPI_CLK,ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct = {0};
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // 1.125MHz
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(ADXL345_SPIPORT,&SPI_InitStruct);

    SPI_Cmd(ADXL345_SPIPORT, ENABLE);
	
    // set high
	SPI_NSSInternalSoftwareConfig(ADXL345_SPIPORT,SPI_NSSInternalSoft_Set);
	

} 

// 0x2D 0x00
// 0x31 0x09 0000 1001
// 0x2C 0x0B 100Hz
// 0x2E 0x00 disable interrupts
// 0x2D 0x08

void ADXL345_Init()
{
    ADXL345_WriteBytes(0x2D,0x00);
    ADXL345_WriteBytes(0x31,0x09);
    ADXL345_WriteBytes(0x2C,0x0B);
    ADXL345_WriteBytes(0x2E,0x00);
    ADXL345_WriteBytes(0x2D,0x08);
}


void ADXL345_WriteBytes(uint8_t commands,uint8_t value)
{
    uint8_t buffer[2];
    buffer [0] = commands;
    buffer[1] = value;

    GPIO_WriteBit(ADXL345_PORT,ADXL345_NSS,Bit_RESET);
    My_SPI_MasterTransmitReceive(ADXL345_SPIPORT,buffer, buffer,2);
    GPIO_WriteBit(ADXL345_PORT,ADXL345_NSS,Bit_SET);

}


void ADXL345_ReadBytes(uint8_t reg, uint8_t *buf)
{
    uint8_t tx = reg | 0x80 | 0x40; // read + multi-byte

    // Select device
    GPIO_WriteBit(ADXL345_PORT, ADXL345_NSS, Bit_RESET);

    // Send register address
    uint8_t dummy_rx;
    My_SPI_MasterTransmitReceive(ADXL345_SPIPORT, &tx, &dummy_rx, 1);

    // Read all bytes
    for(uint8_t i = 0; i < 6; i++)
    {
        buf[i] = 0;
        My_SPI_MasterTransmitReceive(SPI1, &buf[i], &buf[i], 1);
    }

    // Deselect
    GPIO_WriteBit(ADXL345_PORT, ADXL345_NSS, Bit_SET);
}


void ADXL345_XYZ(float *xyz)
{
    uint8_t buffer[6] = {0};

   
    ADXL345_ReadBytes(0x32,buffer);

    int16_t x = (buffer[1] << 8) | buffer[0];
    int16_t y = (buffer[3] << 8) | buffer[2];
    int16_t z = (buffer[5] << 8) | buffer[4];

    xyz[0] = atan2(x, sqrt(y*y + z*z)) * 180.0f/ M_PI;
    xyz[1] = atan2(y, sqrt(x*x + z*z)) * 180.0f / M_PI;
    xyz[2] = atan2(sqrt(y*y + x*x),z) *  180.0f / M_PI;

}