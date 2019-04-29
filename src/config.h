#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <mcs51/8051.h>

// spi pins
#define SPI_SCK        P2_4
#define SPI_MOSI       P2_3
#define SPI_MISO       P2_5

// oled pins
#define OLED_CS        P2_0
#define OLED_DC        P2_1
#define OLED_RESET     P2_2

// ds1302 pins
#define DS1302_RST     P1_0
#define DS1302_SDA     P1_1
#define DS1302_SCK     P1_2

// ds18b20 pin
#define DS18B20_PIN    P0_0

#endif // __CONFIG_H__
