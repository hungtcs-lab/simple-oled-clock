#include <stdio.h>
#include <stdint.h>
#include <mcs51/8052.h>
#include "oled.h"
#include "delay.h"
#include "serial.h"
#include "ds1302.h"

int putchar(int c)
{
	while(!(SCON & 0x02));
	SCON &= ~0x02;
	SBUF = (char)c;
  return c;
}

void render(DS1302_DATETIME *datetime)
{
  uint8_t i;

  oled_put_chars_8x16(0, 0, "20");
  i=2;
  oled_put_char_8x16(i++, 0, (datetime -> year / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> year % 10) + 48);
  i++;
  oled_put_char_8x16(i++, 0, (datetime -> month / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> month % 10) + 48);
  i++;
  oled_put_char_8x16(i++, 0, (datetime -> date / 10) + 48);
  oled_put_char_8x16(i++, 0, (datetime -> date % 10) + 48);

  i=0;
  oled_put_char_16x32(i++, 2, (datetime -> hour / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> hour % 10) + 48);
  i++;
  oled_put_char_16x32(i++, 2, (datetime -> minute / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> minute % 10) + 48);
  i++;
  oled_put_char_16x32(i++, 2, (datetime -> second / 10) + 48);
  oled_put_char_16x32(i++, 2, (datetime -> second % 10) + 48);
}

void main(void)
{
  // uint8_t i;
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

  while(1)
  {
    ds1302_get_datetime(&datetime);
    printf("20%02d/%02d/%02d %02d:%02d:%02d - Wk.%d\n", datetime.year, datetime.month, datetime.date, datetime.hour, datetime.minute, datetime.second, datetime.day);
    render(&datetime);
    delay_ms(500);
  }
}
