#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <mcs51/8052.h>
#include "oled.h"
#include "delay.h"
#include "serial.h"
#include "ds1302.h"
#include "ds18b20.h"

int putchar(int c)
{
	while(!(SCON & 0x02));
	SCON &= ~0x02;
	SBUF = (char)c;
  return c;
}

void render(DS1302_DATETIME *datetime, float temperature)
{
  int temp;
  uint8_t i;

  oled_put_chars_8x16(0, 0, "20");
  i=2;
  oled_put_char_8x16(i++, 0, (datetime -> year / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> year % 10) + 48);
  oled_put_char_8x16(i++, 0, '/');
  oled_put_char_8x16(i++, 0, (datetime -> month / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> month % 10) + 48);
  oled_put_char_8x16(i++, 0, '/');
  oled_put_char_8x16(i++, 0, (datetime -> date / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> date % 10) + 48);
  i += 2;

  oled_put_chars_8x16(i, 0, "Wk.");
  i += 3;
  oled_put_char_8x16(i, 0, (datetime -> day) + 48);

  i=0;
  oled_put_char_16x32(i++, 2, (datetime -> hour / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> hour % 10) + 48);
  oled_put_char_16x32(i++, 2, 10 + 48);
  oled_put_char_16x32(i++, 2, (datetime -> minute / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> minute % 10) + 48);
  oled_put_char_16x32(i++, 2, 10 + 48);
  oled_put_char_16x32(i++, 2, (datetime -> second / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> second % 10) + 48);

  i = 9;
  temp = (int)temperature;
  if(temp < 0)
  {
    oled_put_char_8x16(i++, 6, '-');
    temp = -temp;
  }
  else
  {
    i++;
  }
  oled_put_char_8x16(i++, 6, (temp / 10) + 48);
  oled_put_char_8x16(i++, 6, (temp % 10) + 48);
  oled_put_char_8x16(i++, 6, '.');
  temp = (int)(fabsf(temperature * 10));
  oled_put_char_8x16(i++, 6, (temp % 10) + 48);
  oled_put_char_8x16(i++, 6, 95 + 32);
  oled_put_char_8x16(i++, 6, 'C');
}

void main(void)
{
  float temperature;
  DS1302_DATETIME datetime;

  serial_init();
  oled_init();
  oled_clear();

  datetime.second = 0;
  datetime.minute = 0;
  datetime.hour = 0;
  datetime.date = 1;
  datetime.month = 1;
  datetime.year = 19;
  datetime.day = 2;

  ds1302_set_datetime(&datetime);
  ds1302_trickle_charger_disable();
  ds1302_start();

  ds18b20_measuring(&temperature, DS18B20_RESOLUTION_10_BIT);

  while(1)
  {
    ds1302_get_datetime(&datetime);
    ds18b20_measuring(&temperature, DS18B20_RESOLUTION_10_BIT);
    // printf("20%02d/%02d/%02d %02d:%02d:%02d - Wk.%d\n", datetime.year, datetime.month, datetime.date, datetime.hour, datetime.minute, datetime.second, datetime.day);
    render(&datetime, temperature);
    delay_ms(5);
  }
}
