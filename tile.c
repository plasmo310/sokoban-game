
/* BGタイル配列 */
unsigned char tile[] =
{
  /* 0x00 白背景 */
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  /* 0x01 ブロック */
  0xF7,0x08,0xF7,0x08,0xF7,0x08,0x00,0xFF,
  0x7F,0x80,0x7F,0x80,0x7F,0x80,0x00,0xFF,
  /* 0x02-0x05 エレキベア */
  0x00,0x0C,0x00,0x1E,0x01,0x1E,0x03,0x0C,
  0x00,0x0F,0x02,0x0F,0x01,0x0E,0x03,0x0F,
  0x00,0x1F,0x00,0x3E,0x00,0x3C,0x00,0x3C,
  0x00,0x3C,0x38,0x04,0x00,0x0F,0x00,0x15,
  0x40,0x30,0xC0,0x38,0xE0,0x18,0xC0,0x30,
  0x80,0x70,0x40,0xF0,0x80,0x70,0xC0,0xF0,
  0x00,0xF8,0x00,0x7C,0x00,0x3C,0x00,0x3C,
  0x00,0x3C,0x1C,0x20,0x00,0xF0,0x00,0xA8,
  /* 0x06-0x09 ポイント */
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x06,
  0x00,0x06,0x00,0x03,0x00,0x01,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x80,0x00,0xC0,0x00,0x60,
  0x00,0x60,0x00,0xC0,0x00,0x80,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  /* 0x0a-0x0d 箱 */
  0x00,0x00,0x7F,0x7F,0x7F,0x40,0x77,0x48,
  0x70,0x4F,0x65,0x5A,0x78,0x47,0x7B,0x44,
  0x60,0x5F,0x71,0x4E,0x71,0x4E,0x6A,0x55,
  0x7B,0x44,0x7F,0x40,0x7F,0x7F,0x00,0x00,
  0x00,0x00,0xFE,0xFE,0xFE,0x02,0xBE,0x42,
  0x06,0xFA,0x4E,0xB2,0x86,0x7A,0xB6,0x4A,
  0x86,0x7A,0xB6,0x4A,0x86,0x7A,0xB6,0x4A,
  0x86,0x7A,0xFE,0x02,0xFE,0xFE,0x00,0x00
};
