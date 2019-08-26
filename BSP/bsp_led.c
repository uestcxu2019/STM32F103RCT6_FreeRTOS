// bsp borad support package

#include  "bsp_led.h"

// bsp borad support package
/*****************************************************************************
	*	Author	 : xuzhi Liu
	*	Date  	 : 2018.12.18
	*   File Name: bsp_led.c
	*
	*		Ӳ������˵��: ����Ұ��mini�������ԭ��ͼ֪:
	*				LED1  -->   PC2
	*				LED2  -->   PC3
	*
	*				�ߵ�ƽϨ��LED,�͵�ƽ����LED
	*
******************************************************************************/


/*
 *	˵	��:	LED���ų�ʼ������
 *	��	��: ��
 *	����ֵ: ��
**/
void Led_Init(void)
{
	//GPIO_InitTypeDefΪ�ṹ������
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//��ʱ��
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	//��ʼ���ṹ��
	GPIO_InitStruct.GPIO_Pin = LED1_GPIO_PIN + LED2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

	/*���ó�ʼ�������������úõĽṹ���Աд���Ĵ�����*/
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStruct);
	
	//��ʼ���ر�����LED��
	GPIO_SetBits(LED_GPIO_PORT, LED1_GPIO_PIN|LED2_GPIO_PIN);
}

/*
 *	˵	��: LED2����ߵ�ƽ
 *	��	��: flag,ֵΪ����֮һ:
 *	    		 0: �͵�ƽ,LEDϨ��
 *				 1: �ߵ�ƽ,LED����
 *	����ֵ: ��
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
 *	˵	��: LED2����ߵ�ƽ
 *	��	��: flag,ֵΪ����֮һ:
 *	    		 0: �͵�ƽ,LEDϨ��
 *				 1: �ߵ�ƽ,LED����
 *	����ֵ: ��
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
 *	˵	��: LED1����
 *	��	��: flag,ֵΪ����֮һ:
 *	    		 0: �͵�ƽ,LEDϨ��
 *				 1: �ߵ�ƽ,LED����
 *	����ֵ: ��
**/
void LED1_Toggle(void)
{
	GPIO_TogglePin(LED_GPIO_PORT,LED1_GPIO_PIN);
}



/*
 *	˵	��: LED2����
 *	��	��: flag,ֵΪ����֮һ:
 *	    		 0: �͵�ƽ,LEDϨ��
 *				 1: �ߵ�ƽ,LED����
 *	����ֵ: ��
**/
void LED2_Toggle(void)
{
	GPIO_TogglePin(LED_GPIO_PORT,LED2_GPIO_PIN);
}


