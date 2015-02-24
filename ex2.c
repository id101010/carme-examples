/*****************************************************************************
 * Copyright &copy; 2013, Bern University of Applied Sciences.
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
#include <stm32f4xx.h>
#include <carme.h>
#include <lcd.h>
#include <lcd_lld.h>
#include <color.h>

GPIO_InitTypeDef gpio;

void init_gpio(void)
{
    // Set clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

    // OUT0: GPIOA, GPIO_Pin_0
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);

    // OUT1: GPIOH, GPIO_Pin_11
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_11;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOH, &gpio);
    
    // OUT2: GPIOH, GPIO_Pin_12
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_12;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOH, &gpio);
    
    // OUT3: GPIOB, GPIO_Pin_8
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_8;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio);
    
    // T0: GPIOC, GPIO_Pin_7
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_7;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);
    
    // T0: GPIOB, GPIO_Pin_15
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_15;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
    
    // T0: GPIOB, GPIO_Pin_14
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_14;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
    
    // T0: GPIOI, GPIO_Pin_0
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOI, &gpio);
}

int main(void)
{
    LCD_Init();
    init_gpio();

    LCD_Clear(GUI_COLOR_BLUE);

    char new0 = 0, new1 = 0, new2 = 0, new3 = 0; // new state
    char old0 = 0, old1 = 0, old2 = 0, old3 = 0; // old state

    while(1){

        // -----------------------------------------------------  T0
        new0 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);

        if(new0 != old0 && old0 == 0){
            GPIO_ToggleBits(GPIOA, GPIO_Pin_0);
        }

        old0 = new0;
        
        // ------------------------------------------------------ T1
        new1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

        if(new1 != old1 && old1 == 0){
            GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
        }

        old1 = new1;

        // ------------------------------------------------------ T2
        new2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);

        if(new2 != old2 && old2 == 0){
            GPIO_ToggleBits(GPIOH, GPIO_Pin_12);
        }

        old2 = new2;
        
        // ------------------------------------------------------ T3
        new3 = GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_0);

        if(new3 != old3 && old3 == 0){
            GPIO_ToggleBits(GPIOB, GPIO_Pin_8);
        }

        old3 = new3;
    }

    return 0;
}

