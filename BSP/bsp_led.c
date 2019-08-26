// bsp borad support package

#include  "bsp_led.h"

// bsp borad support package
/*****************************************************************************
	*	Author	 : xuzhi Liu
	*	Date  	 : 2018.12.18
	*   File Name: bsp_led.c
	*
	*		硬件连接说明: 根据野火mini开发板的原理图知:
	*				LED1  -->   PC2
	*				LED2  -->   PC3
	*
	*				高电平熄灭LED,低电平点亮LED
	*
******************************************************************************/


/*
 *	说	明:	LED引脚初始化函数
 *	参	数: 无
 *	返回值: 无
**/
void Led_Init(void)
{
	//GPIO_InitTypeDef为结构体类型
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//打开时钟
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	//初始化结构体
	GPIO_InitStruct.GPIO_Pin = LED1_GPIO_PIN + LED2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

	/*调用初始化函数，把配置好的结构体成员写到寄存器里*/
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStruct);
	
	//初始化关闭两个LED灯
	GPIO_SetBits(LED_GPIO_PORT, LED1_GPIO_PIN|LED2_GPIO_PIN);
}

/*
 *	说	明: LED2输出高电平
 *	参	数: flag,值为下列之一:
 *	    		 0: 低电平,LED熄灭
 *				 1: 高电平,LED点亮
 *	返回值: 无
**/
void LED1(uint8_t flag)
{
	if(flag == 0)
	{
		GPIO_SetBits(LED_GPIO_PORT, LED1_GPIO_PIN);
	}	
	else if(flag == 1)
	{
		GPIO_ResetBits(LED_GPIO_PORT, LED1_GPIO_PIN);
	}
}

/*
 *	说	明: LED2输出高电平
 *	参	数: flag,值为下列之一:
 *	    		 0: 低电平,LED熄灭
 *				 1: 高电平,LED点亮
 *	返回值: 无
**/
void LED2(uint8_t flag)
{
	if(flag == 0)
	{
		GPIO_SetBits(LED_GPIO_PORT, LED2_GPIO_PIN);
	}
	else if(flag == 1)
	{
		GPIO_ResetBits(LED_GPIO_PORT, LED2_GPIO_PIN);
	}	
}


/*
 *	说	明: LED1闪灯
 *	参	数: flag,值为下列之一:
 *	    		 0: 低电平,LED熄灭
 *				 1: 高电平,LED点亮
 *	返回值: 无
**/
void LED1_Toggle(void)
{
	GPIO_TogglePin(LED_GPIO_PORT,LED1_GPIO_PIN);
}



/*
 *	说	明: LED2闪灯
 *	参	数: flag,值为下列之一:
 *	    		 0: 低电平,LED熄灭
 *				 1: 高电平,LED点亮
 *	返回值: 无
**/
void LED2_Toggle(void)
{
	GPIO_TogglePin(LED_GPIO_PORT,LED2_GPIO_PIN);
}


