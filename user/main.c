

#include <string.h>
#include "stm32f10x.h"
#include "ds18b20.h"
#include "oled.h"
#include "si2c.h"
#include "delay.h"
#include "w25q64.h"
#include "math.h"
#include "spi.h"


SI2C_TypeDef si2c;
OLED_TypeDef oled;

void My_SoftwareI2C_Init(void);
int i2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size);
void My_OLEDScreen_Init(void);


int main(void)
{
	ds18b20_GPIO_Init();

	My_SoftwareI2C_Init();
	My_OLEDScreen_Init();
	W25Q64_SPI_Init();
	
	float TL = LoadTemperature();
	
	if(LoadTemperature() < -50.0 || LoadTemperature() > 150)
	{
		TL= 0.00;
		
	}
	OLED_Clear(&oled);
	OLED_SetCursor(&oled,7,20);
	OLED_Printf(&oled,"LAST TEMP: %.2fC",	TL);
	OLED_SendBuffer(&oled);
	Delay(5000);


	float lastSavedT = -999.0f;

	while(1)
	{
		float T = ds18b20_GetTemp();

		if(fabs(T-lastSavedT)> 1.0){
		uint8_t bytes[4];
		uint32_t f = ds18b20_GetTemp();;
		memcpy(&f,&T,sizeof(f));
		bytes[0] = (f>>24)&0xFF;
		bytes[1] = (f>>16)&0xFF;
		bytes[2] = (f>>8)&0xFF;
		bytes[3] = (f)&0xFF;
			
		W25Q64_Save4Bytes(bytes);
		lastSavedT = T;
	}

		OLED_Clear(&oled);
		OLED_SetCursor(&oled,7,20);
		OLED_Printf(&oled,"TEMP: %.2fC",T);
		OLED_SendBuffer(&oled);
		Delay(5000);
	}
}



void My_SoftwareI2C_Init(void)
{
	si2c.SCL_GPIOx = GPIOA;
	si2c.SCL_GPIO_Pin = GPIO_Pin_8;
	si2c.SDA_GPIOx = GPIOA;
	si2c.SDA_GPIO_Pin = GPIO_Pin_9;
	
	My_SI2C_Init(&si2c);
}

int i2c_write_bytes(uint8_t addr, const uint8_t *pdata, uint16_t size)
{
	return My_SI2C_SendBytes(&si2c,addr,pdata,size);
}

void My_OLEDScreen_Init(void)
{
	OLED_InitTypeDef OLED_InitSturct;
	OLED_InitSturct.i2c_write_cb = i2c_write_bytes;
	OLED_Init(&oled,&OLED_InitSturct);
	
}


