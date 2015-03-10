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
#include <stm32f4xx.h>              // Processor STM32F407IG
#include <carme.h>                  // CARME Module
#include <lcd.h>
#include <lcd_lld.h>
#include <color.h>

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

// Timer 6 interrupt service routine
void TIM6_DAC_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) == SET){  // If update event flag
        GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
        TIM_ClearFlag(TIM6, TIM_IT_Update);             // Clear interrupt flag
    }

}

// EXTI 9 interrupt service routine for T0
void EXTI9_5_IRQHandler()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    EXTI_ClearFlag(EXTI_Line7);
    // TODO: Set flag to write on display
}

// EXTI 15 interrupt service routine for T1
void EXTI15_10_IRQHandler()
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    EXTI_ClearFlag(EXTI_Line15);
    // TODO: Set flag to write on display
}

// GPIO initializations
void init_gpio(void)
{
    GPIO_InitTypeDef gpio;

    // OUT0: GPIOA, GPIO_Pin_0
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);

    // OUT1: GPIOH, GPIO_Pin_11
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_11;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOH, &gpio);

    // OUT2: GPIOH, GPIO_Pin_12
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_12;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOH, &gpio);

    // T0: GPIOC, GPIO_Pin_7
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_7;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);

    // T1: GPIOB, GPIO_Pin_15
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_15;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);
}

void init_timer()
{
    TIM_TimeBaseInitTypeDef t;
    const int APB1_CLK = 42E6;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // Enable clock for TIM6

    // Timer 6 configuration
    TIM_TimeBaseStructInit(&t);                          // Init TimeBaseStruct
    t.TIM_Prescaler = APB1_CLK / 1000 - 1;               // 0..41999 prescaler
    t.TIM_Period = 1000 - 1;                             // Count to 999
    TIM_TimeBaseInit(TIM6, &t);                          // Init TIM6
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);           // Enable update IRQ for TIM6
    NVIC_EnableIRQ(TIM6_DAC_IRQn);                       // Enable IRQs for TIM6

    TIM_Cmd(TIM6, ENABLE);                               // Enable both timers
}

void init_exti()
{
    EXTI_InitTypeDef exti;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);  // T0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15); // T1

    // EXTI on T0
    EXTI_StructInit(&exti);
    exti.EXTI_Line = EXTI_Line7;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    // EXTI on T1
    EXTI_StructInit(&exti);
    exti.EXTI_Line = EXTI_Line15;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    // Enable IRQs
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

int main(void)
{
    __disable_irq();
    init_gpio();
    init_timer();
    init_exti();
    __enable_irq();

    LCD_Init();
    LCD_Clear(GUI_COLOR_RED);

    while(1){
        // TODO Check LCD flags and update text on display
    }

    return 0;
}

