/*****************************************************************************
 * Copyright &copy; 2013, Aaron
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
#include <stm32f4xx.h>				                    // Processor STM32F407IG

#define LED (*(volatile unsigned char*)(0x6C000000))
int ledStatus = 0;

// Timer 6 interrupt service routine
void TIM6_DAC_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) == SET){  // If update event flag
        LED ^= 0b00000001;                              // Toggle LED
        TIM_ClearFlag(TIM6, TIM_IT_Update);             // Clear interrupt flag
    }
    
}

// Timer 7 interrupt service routine
void TIM7_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM7, TIM_IT_Update) == SET){  // If update event flag
        LED ^= 0b00000010;                              // Toggle second LED
        TIM_ClearFlag(TIM7, TIM_IT_Update);             // Clear interrupt flag
    }
}

int main(void)
{
    TIM_TimeBaseInitTypeDef t;
    const int APB1_CLK = 42E6;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // Enable clock for TIM6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // Enable clock for TIM7

	LED = 0b00000001;

    __disable_irq();

    // Timer 6 configuration
    TIM_TimeBaseStructInit(&t);                          // Init TimeBaseStruct
    t.TIM_Prescaler = APB1_CLK / 1000 - 1;               // 0..41999 prescaler
    t.TIM_Period = 1000 - 1;                             // Count to 999
    TIM_TimeBaseInit(TIM6, &t);                          // Init TIM6
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);           // Enable update IRQ for TIM6
    NVIC_EnableIRQ(TIM6_DAC_IRQn);                       // Enable IRQs for TIM6

    // Timer 7 configuration
    TIM_TimeBaseStructInit(&t);                          // Init TimeBaseStruct
    t.TIM_Prescaler = APB1_CLK / 1000 - 1;               // 0..41999 prescaler
    t.TIM_Period = 250 - 1;                              // Count to 249
    TIM_TimeBaseInit(TIM7, &t);                          // Init TIM7
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);           // Enable update IRQ for TIM7
    NVIC_EnableIRQ(TIM7_IRQn);                           // Enable IRQs for TIM7
    
    __enable_irq();

    TIM_Cmd(TIM6, ENABLE);                               // Enable both timers
    TIM_Cmd(TIM7, ENABLE);

	while(1){
	}

	return 0;
}

