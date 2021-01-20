/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"

/* Connect BL or B-LED to 3.3V if the GLCD is hot */

int main(void)
{
    nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_set_cursor(5, 0);

    nokia_lcd_write_string("CPE301 ESD",1);
    nokia_lcd_set_cursor(5, 15);
    nokia_lcd_write_string("AWESOME!", 1);
    nokia_lcd_render();

    for (;;) {
        _delay_ms(1000);
    }
}
