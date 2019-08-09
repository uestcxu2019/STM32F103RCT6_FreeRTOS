#include "bsp_key.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK |KEY2_GPIO_CLK, ENABLE);
	
	//按键1
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
	
	//按键2
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

uint16_t KEY_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		//等待按键释放
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}
