

#include "stm32f10x.h"

void Myy_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx,
                                   uint8_t *pDataRx, uint16_t Size);
