/*****************************************************************************
 * Copyright &copy; 2013, Aaron Schmocker.
 * All rights reserved.
 *
 * ##### GNU GENERAL PUBLIC LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 *****************************************************************************/
#include<stm32f4xx.h>
#include<carme.h>
#include<lcd.h>
#include<lcd_lld.h>
#include<color.h>
#include<stm32f4xx_adc.h>

#define SPEED   500000

uint16_t colors[] = {
    GUI_COLOR_BLACK,
    GUI_COLOR_WHITE,
    GUI_COLOR_LIGHT_GRAY,
    GUI_COLOR_DARK_GREY,
    GUI_COLOR_RED,
    GUI_COLOR_YELLOW,
    GUI_COLOR_ORANGE,
    GUI_COLOR_BROWN,
    GUI_COLOR_GREEN,
    GUI_COLOR_CYAN,
    GUI_COLOR_BLUE,
    GUI_COLOR_PINK,
    GUI_COLOR_MAGENTA
};

volatile unsigned char* LED = (volatile unsigned char*)0x6C000200;
volatile unsigned char* SWITCH = (volatile unsigned char*)0x6C000400;
GPIO_InitTypeDef g;

void init_gpio(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    g.GPIO_Pin      = GPIO_Pin_0;
    g.GPIO_Mode     = GPIO_Mode_OUT;
    g.GPIO_OType    = GPIO_OType_PP;
    g.GPIO_Speed    = GPIO_Speed_2MHz;
    g.GPIO_PuPd     = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &g);
}

int main(void)
{
    int i = 0, j = 0, m = 0;

    *LED = 0b00000001;

    LCD_Init();
    init_gpio();
    LCD_Clear(GUI_COLOR_RED);

    while(1){

        for(m = 0; (m <= 12); m++){

            LCD_Clear(colors[m]);

            GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1);

            for(i = 0; i < 7; i++){
                *LED<<=1;
                for(j = 0; j < SPEED; j++);
            }

            GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0);

            for(i = 7; i > 0; i--){
                *LED>>=1;
                for(j = 0; j < SPEED; j++);
            }
        }
    }

    return 0;
}

