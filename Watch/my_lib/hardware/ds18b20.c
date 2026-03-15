/**
  ******************************************************************************
  * @file    ds18b20.c
  * @author  jjjoshzhang
  * @version V 1.0.0
  * @date    3.12.2026
  * @brief   ds18b20 source file
  ******************************************************************************
  */

#include "ds18b20.h"
#include "delay.h"
#include "stm32f10x.h"
void ds18b20_GPIO_Init(void)
{
  // call clock for only 1 time
  RCC_APB2PeriphClockCmd(DS18B20_PIN_CLK, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}



void SetPinOutput(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

}


void SetPinInput(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

}


// return 0 = initialized sucessfully 
// return -1 no presence pulse
// return -2 presence too long

int ds18b20_Check(void)
{
	SetPinInput();
  uint32_t start = GetUs();
      // wait for slave to pull LOW
  while(ds18b20_Read() == 1)
  {
    if(GetUs() - start > 100)
      return -1;   // no presence pulse
  }
    // wait for slave to release HIGH
  while(ds18b20_Read() == 0)
  {
    if(GetUs() - start > 300)
     return -2;   // presence too long
  }

  return 0;
}


int ds18b20_Init(void)
{
  SetPinOutput();
  ds18b20_Low();
  DelayUs(550);
	ds18b20_High();
  DelayUs(50);
	int check = ds18b20_Check();
  return check;

}

void ds18b20_WriteBit(uint8_t bit)
{
  SetPinOutput();
  ds18b20_Low();
  if(!bit)
  {
    DelayUs(70);
    ds18b20_High();

  }else{
    DelayUs(5);
    ds18b20_High();
    DelayUs(60);
  }
}


void ds18b20_WriteByte(uint8_t byte)
{
    for(int i = 0; i < 8; i++)
    {
        if(byte & 0x01)
        {
            ds18b20_WriteBit(1);
        }
        else
        {
            ds18b20_WriteBit(0);
        }

        byte >>= 1;
    }

}

// return 0 = read sucessfully 
// return -1 read unsucessfully 


uint8_t ds18b20_ReadBit(void)
{

    uint8_t bit;

    SetPinOutput();
    ds18b20_Low();
    DelayUs(5);        // initiate read slot

    SetPinInput();     // release bus
    DelayUs(10);       // wait before sampling

    bit = ds18b20_Read();   // sample

    DelayUs(50);       // finish the slot

    return bit;
}



uint8_t ds18b20_ReadByte(void)
{
  uint8_t buffer = 0;

    for(int i = 0; i < 8; i++)
    {
      buffer |= ds18b20_ReadBit() << i;
    }

  return buffer;

}

void ds18b20_Start(void)
{
  ds18b20_WriteByte(0xcc); // skip ROM
  ds18b20_WriteByte(0x44); // Convert
}

void ds18b20_Finish(void)
{
  ds18b20_WriteByte(0xcc); // skip ROM
  ds18b20_WriteByte(0xbe); // read temp
  
}

// all in one function

float ds18b20_GetTemp(void)
{
	int sign = 1;
  ds18b20_Init();
  ds18b20_Start();
	// wait for max conversion time 
  Delay(750);
  ds18b20_Init();
  ds18b20_Finish();
  

  uint8_t temp1 = ds18b20_ReadByte();
  uint8_t temp2 = ds18b20_ReadByte();
	
	// first 4 digits has 1 which means negative temp
	if(temp2>7)
	{
		temp1 = ~temp1;
		temp2 = ~temp2;
		sign = 0;
		
	}
		uint16_t raw = (temp2 << 8) | temp1;


  float T = raw * 0.0625;

  if (sign) return T;
	else return -T;

}
