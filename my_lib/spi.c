#include "spi.h"

void Myy_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx,
                                   uint8_t *pDataRx, uint16_t Size) {
  if (Size == 0)
    return;

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
    ;

  SPI_I2S_SendData(SPIx, pDataTx[0]);

  for (uint16_t i = 0; i < Size - 1; i++) {

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
      ;

    SPI_I2S_SendData(SPIx, pDataTx[i + 1]);

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
      ;

    pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
  }

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    ;

  pDataRx[Size - 1] = SPI_I2S_ReceiveData(SPIx);
}
