#ifndef  __BSP_LED_H
#define  __BSP_LED_H

#include "stm32f10x.h"

#define   LED_GPIO_CLK       RCC_APB2Periph_GPIOC
#define   LED_GPIO_PORT      GPIOC
#define   LED1_GPIO_PIN      GPIO_Pin_2
#define   LED2_GPIO_PIN      GPIO_Pin_3

void Led_Init(void);
void LED1(uint8_t flag);
void LED2(uint8_t flag);



#endif  /*__BSP_LED_H*/
