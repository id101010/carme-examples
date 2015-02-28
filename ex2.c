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
#include <stm32f4xx.h>
#include <carme.h>
#include <lcd.h>
#include <lcd_lld.h>
#include <color.h>

#define GPIO_INPUT	0
#define GPIO_OUTPUT 1
#define NULL		0

// Thanks to t-moe
typedef struct pin_s {
	GPIO_TypeDef *GPIO;
	uint16_t number;
	uint8_t	purpose;
} pin_t;

pin_t *create_gpio(GPIO_TypeDef *GPIO, uint16_t number, uint8_t purpose)
{
	GPIO_InitTypeDef g;
	GPIO_StructInit(&g);

	if(!purpose){ 	// Input
		g.GPIO_Mode		=	GPIO_Mode_IN;
		g.GPIO_OType	=	GPIO_OType_OD;
		g.GPIO_Pin		=	number;
		g.GPIO_PuPd		=	GPIO_PuPd_UP;
		g.GPIO_Speed	=	GPIO_Low_Speed;
	} else { 		// output
		g.GPIO_Mode		=	GPIO_Mode_OUT;
		g.GPIO_OType	=	GPIO_OType_PP;
		g.GPIO_Pin		=	number;
		g.GPIO_PuPd		=	GPIO_PuPd_NOPULL;
		g.GPIO_Speed	=	GPIO_Low_Speed;
	}

	GPIO_Init(GPIO, &g);

	pin_t *pin = malloc(sizeof(pin_t));

	if(pin == NULL){ // out of memory
		return 0;
	}

	pin->GPIO 		= 	GPIO;
	pin->number 	= 	number;
	pin->purpose 	= 	purpose;

	return pin;
}

uint8_t read_gpio(pin_t *pin)
{
    uint8_t pinstate = 0;

    pinstate = GPIO_ReadInputDataBit(pin->GPIO, pin->number);
    
    return pinstate;
}

void set_gpio(pin_t *pin)
{
    GPIO_SetBits(pin->GPIO, pin->number);
}

void toggle_gpio(pin_t *pin)
{
    GPIO_ToggleBits(pin->GPIO, pin->number);
}

int main(void)
{
	// Some blue color looks nice
    LCD_Init();
    LCD_Clear(GUI_COLOR_BLUE);

    // Inputs
    pin_t *led0 = create_gpio(GPIOA, GPIO_Pin_0, GPIO_OUTPUT);
    pin_t *led1 = create_gpio(GPIOH, GPIO_Pin_11, GPIO_OUTPUT);
    pin_t *led2 = create_gpio(GPIOH, GPIO_Pin_12, GPIO_OUTPUT);
    pin_t *led3 = create_gpio(GPIOB, GPIO_Pin_8, GPIO_OUTPUT);

    // Outputs
    pin_t *btn0 = create_gpio(GPIOC, GPIO_Pin_7, GPIO_INPUT);
    pin_t *btn1 = create_gpio(GPIOB, GPIO_Pin_15, GPIO_INPUT);
    pin_t *btn2 = create_gpio(GPIOB, GPIO_Pin_14, GPIO_INPUT);
    pin_t *btn3 = create_gpio(GPIOI, GPIO_Pin_0, GPIO_INPUT);

    // State variables
    uint8_t new = 0;	// newest state
    uint8_t old = 0;	// previous state
    uint8_t edg = 0;	// rising edge

    while(1){

    	// Get the current states of the buttons and merge them
        new =   read_gpio(btn0) 		|
                read_gpio(btn1) << 1 	|
                read_gpio(btn2) << 2 	|
                read_gpio(btn3) << 3;
        
        edg = (new ^ old) & new;	// detect positive edge
        old = new;

        if(edg & 0b0001){ 			// button 1 changed
            toggle_gpio(led0);
        }

        if(edg & 0b0010){			// button 2 changed
            toggle_gpio(led1);
        }

        if(edg & 0b0100){			// button 3 changed
            toggle_gpio(led2);
        }

        if(edg & 0b1000){ 			// button 4 changed
            toggle_gpio(led3);
        }
    }

    // clean up the memory
    free(led0);
    free(led1);
    free(led2);
    free(led3);
    free(btn0);
    free(btn1);
    free(btn2);
    free(btn3);

    return 0;
}


