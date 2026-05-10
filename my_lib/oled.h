#ifndef _OLED_H_
#define _OLED_H_

#include "oled_font.h"
#include "stm32f10x.h"


#define OLED_SLAVE_ADDR 0x78

extern const Font_TypeDef default_font;

#define OLED_COLOR_TRANSPARENT 0x00
#define OLED_COLOR_WHITE 0x01
#define OLED_COLOR_BLACK 0x02

#define PEN_COLOR_TRANSPARENT OLED_COLOR_TRANSPARENT
#define PEN_COLOR_WHITE OLED_COLOR_WHITE
#define PEN_COLOR_BLACK OLED_COLOR_BLACK

#define BRUSH_TRANSPARENT OLED_COLOR_TRANSPARENT
#define BRUSH_WHITE OLED_COLOR_WHITE
#define BRUSH_BLACK OLED_COLOR_BLACK

typedef struct {
  int (*i2c_write_cb)(uint8_t addr, const uint8_t *pdata, uint16_t size);
} OLED_InitTypeDef;

typedef struct {
  int (*i2c_write_cb)(uint8_t addr, const uint8_t *pdata, uint16_t size);

  uint8_t *pBuffer;
  const Font_TypeDef *Font;
  uint8_t PenColor;
  uint8_t PenWidth;
  uint8_t Brush;
  int16_t CursorX;
  int16_t CursorY;
  uint16_t RefreshProgress;

  int16_t ClipRegionX;
  int16_t ClipRegionY;
  uint16_t ClipRegionWidth;
  uint16_t ClipRegionHeight;

  int16_t TextRegionX;
  int16_t TextRegionY;
  uint16_t TextRegionWidth;
  uint16_t TextRegionHeight;

} OLED_TypeDef;

int OLED_Init(OLED_TypeDef *OLED, OLED_InitTypeDef *OLED_InitStruct);
void OLED_Clear(OLED_TypeDef *OLED);
uint16_t OLED_GetScreenWidth(OLED_TypeDef *OLED);
uint16_t OLED_GetScreenHeight(OLED_TypeDef *OLED);
int OLED_SendBuffer(OLED_TypeDef *OLED);
int OLED_StartSendBuffer(OLED_TypeDef *OLED);
int OLED_EndSendBuffer(OLED_TypeDef *OLED, uint8_t *pMoreOut);

void OLED_SetCursor(OLED_TypeDef *OLED, int16_t X, int16_t Y);
void OLED_SetCursorX(OLED_TypeDef *OLED, int16_t X);
void OLED_SetCursorY(OLED_TypeDef *OLED, int16_t Y);
void OLED_MoveCursor(OLED_TypeDef *OLED, int16_t dX, int16_t dY);
void OLED_MoveCursorX(OLED_TypeDef *OLED, int16_t dX);
void OLED_MoveCursorY(OLED_TypeDef *OLED, int16_t dY);
void OLED_GetCursor(OLED_TypeDef *OLED, int16_t *pXOut, int16_t *pYOut);
int16_t OLED_GetCursorX(OLED_TypeDef *OLED);
int16_t OLED_GetCursorY(OLED_TypeDef *OLED);

void OLED_SetPen(OLED_TypeDef *OLED, uint8_t Pen_Color, uint8_t Width);
void OLED_SetBrush(OLED_TypeDef *OLED, uint8_t Brush_Color);

void OLED_DrawDot(OLED_TypeDef *OLED);
void OLED_DrawLine(OLED_TypeDef *OLED, int16_t X, int16_t Y);
void OLED_LineTo(OLED_TypeDef *OLED, int16_t X, int16_t Y);
void OLED_DrawCircle(OLED_TypeDef *OLED, uint16_t Radius);
void OLED_DrawRect(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height);
void OLED_DrawBitmap(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height,
                     const uint8_t *pBitmap);

void OLED_DrawString(OLED_TypeDef *OLED, const char *Str);
void OLED_Printf(OLED_TypeDef *OLED, const char *Format, ...);
void OLED_StartTextRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y,
                          uint16_t Width, uint16_t Height);
void OLED_StopTextRegion(OLED_TypeDef *OLED);
void OLED_SetFont(OLED_TypeDef *OLED, const Font_TypeDef *Font);
uint16_t OLED_GetStrWidth(OLED_TypeDef *OLED, const char *Str);
uint16_t OLED_GetFontHeight(OLED_TypeDef *OLED);

void OLED_StartClipRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y,
                          uint16_t Width, uint16_t Height);
void OLED_StopClipRegion(OLED_TypeDef *OLED);

#endif
