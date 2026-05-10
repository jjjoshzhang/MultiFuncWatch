#ifndef _OLED_FONT_H_
#define _OLED_FONT_H_

#include <stdint.h>

typedef struct {
  const char *Name;
  uint32_t Encoding;

  uint16_t Swx0;
  uint16_t Swy0;
  uint16_t Dwx0;
  uint16_t Dwy0;

  uint16_t Swx1;
  uint16_t Swy1;
  uint16_t Dwx1;
  uint16_t Dwy1;

  uint16_t VVectorXoff;
  uint16_t VVectorYoff;

  uint16_t BBw;
  uint16_t BBh;
  int16_t BBxoff0x;
  int16_t BByoff0y;

  const uint16_t nBytes;
  const uint8_t *Bitmap;
} Glyph_TypeDef;

typedef struct {
  const char *SpecVersion;
  const char *FontName;
  uint16_t ContentVersion;
  uint8_t MetricsSet;
  uint16_t FontSize;
  uint16_t Xres;
  uint16_t Yres;
  uint16_t FBBx;
  uint16_t FBBy;
  int16_t FBBXoff;
  int16_t FBBYoff;
  uint16_t nChars;
  const uint32_t *Map;
  const Glyph_TypeDef *Glyphs;
} Font_TypeDef;

#endif
