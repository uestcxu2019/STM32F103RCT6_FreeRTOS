#include "stm32f10x.h"
#include "bsp_led.h"


int main(void)	
{
	Led_GPIO_Config();
	LED1(1);
	LED2(0);
	while(1)
	{
	}


}


