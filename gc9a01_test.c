#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "hardware/structs/vreg_and_chip_reset.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "GC9A01/gc9a01.h"
#include "GC9A01/VGA1_16x32.h"
#include "GC9A01/VGA1b_16x32.h"


int32_t t_fine;
uint16_t dig_T1;
int16_t dig_T2, dig_T3;

gc9a01_GC9A01_obj_t create_lcd() {
    spi_init(spi0, 1000 * 62000); // Initialise spi0 at 62mHz
    //Initialise GPIO pins for SPI communication
    gpio_set_function(0, GPIO_FUNC_SPI);
    gpio_set_function(2, GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);

    // Configure Chip Select
    gpio_init(1); // Initialise CS Pin
    gpio_set_dir(1, GPIO_OUT); // Set CS as output
    gpio_put(1, 1); // Set CS High to indicate no currect SPI communication

    gpio_init(4); // Initialise RES Pin
    gpio_set_dir(4, GPIO_OUT); // Set RES as output
    gpio_put(4, 0);

    gpio_init(5); // Initialise DC Pin
    gpio_set_dir(5, GPIO_OUT); // Set DC as output
    gpio_put(5, 0);

    gpio_init(6); // Initialise BLK Pin
    gpio_set_dir(6, GPIO_OUT); // Set BLK as output
    gpio_put(6, 1);

    gc9a01_GC9A01_obj_t lcd;
    lcd.spi_obj = spi0;
    lcd.reset = 4;
    lcd.dc = 5;
    lcd.cs = 1;
    lcd.backlight = 6;
    lcd.xstart = 0;
    lcd.ystart = 0;

    lcd.display_width = 240;
    lcd.display_height = 240;
    lcd.rotation = 2;
    lcd.buffer_size = 2048;
    lcd.i2c_buffer = malloc(2048);
    return lcd;
}

int main() {

  const int  Display_Color_Black   = 0x0000;
  const int  Display_Color_Blue    = 0x001F;
  const int  Display_Color_Red     = 0xF800;
  const int  Display_Color_Green   = 0x07E0;
  const int  Display_Color_Cyan    = 0x07FF;
  const int  Display_Color_Magenta = 0xF81F;
  const int  Display_Color_Yellow  = 0xFFE0;
  const int  Display_Color_White   = 0xFFFF;

  int valid_clocks[] = {125000,
                        130000,133000,
                        140000,150000,
                        160000,170000,180000,
                        190000,200000,210000,
                        220000,230000,
                        232000,240000,
                        242000,250000,256000,258000,260000,
                        270000,
                        280000,288000,
                        294000,250000,256000,258000,260000,
                        270000,
                        280000,288000,
                        290400,296000,297000,
                        300000,302400,303000,304800,306000,307200,
                        308000,309000,309600,312000,314400,315000,316000,316800,
                        318000,319200,320000,321000,324000,327000,328000,333000,
                        332000,333000,336000,339000,340000,342000,344000,345000,
                        348000,351000,352000,354000,356000,357000,360000,363000,
                        364000,366000,368000,369000,372000,375000,376000,378000,
                        380000,381000,384000,387000,388000,390000,392000,393000,
                        396000,399000,400000,402000,404000,408000,412000,414000,
                        416000,420000,428000,432000,436000,438000,440000,448000,
                        0};

  vreg_set_voltage(VREG_VOLTAGE_1_30);
  set_sys_clock_khz(306000, true); //Last 306000
  stdio_init_all();

  int8_t rslt;
  char buf[10];

  GFXfont font = VGA1_16x32;
  GFXfont font_bold = VGA1b_16x32;
  gc9a01_GC9A01_obj_t lcd = create_lcd();
  uint16_t color = Display_Color_Yellow;

  gc9a01_init(&lcd);
  sleep_ms(100);
  gc9a01_fill(&lcd, 0);

  adc_init();

  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(26);
  // Select ADC input 0 (GPIO26)
  adc_select_input(0);

  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(27);
  // Select ADC input 0 (GPIO27)
  adc_select_input(1);

  while (true) {
        sprintf(buf, "%.2f C", M_TWOPI);
        gc9a01_text(&lcd, &font_bold, buf, 70, 100, CYAN, 0);
        sprintf(buf, "%.2f C", 20.0);
        gc9a01_text(&lcd, &font, buf, 75, 150, WHITE, 0);

        sprintf(buf, "%.2f hPa", 1013.25);
        gc9a01_text(&lcd, &font, buf, 55, 50, WHITE, 0);
        color = (color == BLACK) ? MAGENTA : BLACK;
        for (int i = 720; i>0; i--) {
            int x = 120+(int)(119.0*sin((180.0+i/2.0)*M_TWOPI/360.0));
            int y = 120+(int)(119.0*cos((180.0+i/2.0)*M_TWOPI/360.0));
            gc9a01_draw_pixel(&lcd, x, y, color);
        }
  }
  free(lcd.i2c_buffer);
}

