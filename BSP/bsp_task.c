#include "bsp_task.h"


/**************************************************************************************************
*					功能描述:
*							 事件(组)的学习
*							 创建3个任务：按键任务，检测到标志位任务，以及检测到所有标志位任务
*							 现象:按键1或按键2按下，LED1 toggle，同时串口打印数据，当按键1和按键2同时按下
*							 LED2 toggle，同时串口打印数据
*							 事件用于单个与多个或多个与多个任务的同步，事件用作标志位
*					相关概念：1.事件位：用于表明某个事件是否发生，用作事件标志
*							  2.事件组: 事件组就是一组事件位，事件组中的事件位通过位编号来访问
*								对于stm32来说，一个事件组最多可以存储24个事件位。
**************************************************************************************************/

//			优先级:数字越大，优先级越高

/**************************************************************************************************
*		   当多个任务具有相同的部分时，可以通过传入参数改变,从而不用创建多个任务
*		   当使用软件延时函数时，不会出现阻塞态，创建多个任务时，只有高优先级的任务运行
*		   当采用vTaskDelay()函数时，会出现阻塞态，多个任务会轮流执行，
*		   因此每个任务执行都有必要的延时，否则会出现高优先级任务一直占用CPU
**************************************************************************************************/

/**************************************************************************************************
*									变量定义
**************************************************************************************************/


/**************************************************************************************************
*									任务句柄
**************************************************************************************************/
TaskHandle_t AppTaskCreate_Handle = NULL;		//创建任务任务句柄

EventGroupHandle_t Event_Handle =NULL;			//事件组量句柄



/********************************************************************************************
*	描	述:任务创建任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL();	//进入临界区
		
	//创建事件组
	Event_Handle = xEventGroupCreate();		
	if(NULL != Event_Handle)
	{
		printf("事件组创建成功\n\n");
	}	
	//创建任务
	xReturn = xTaskCreate(KEY_Task,"KEY_Task",126,NULL,4,NULL);
	if(pdPASS == xReturn)
	{
		printf("KEY_Task任务创建成功\n");
	}
	
	//创建任务
	xReturn = xTaskCreate(Event_Task,"Event_Task",126,NULL,3,NULL);
	if(pdPASS == xReturn)
	{
		printf("Event_Task任务创建成功\n");
	}

	//创建任务
	xReturn = xTaskCreate(EventAll_task,"EventAll_task",126,NULL,2,NULL);
	if(pdPASS == xReturn)
	{
		printf("EventAll_task任务创建成功\n\n");
	}
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//退出临界区
}


/********************************************************************************************
*	描	述:低优先级任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void KEY_Task(void *parameter)
{	
	uint16_t keyValue;
	while(1)
	{
		if(Event_Handle != NULL)
		{
			keyValue = KEY_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
			if(KEY_ON == keyValue)
			{
				printf("按键1按下\n\n");
				xEventGroupSetBits(Event_Handle,0x01);
			}
			
			keyValue = KEY_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
			if(KEY_ON == keyValue)
			{
				printf("按键2按下\n\n");
				xEventGroupSetBits(Event_Handle,0x02);
			}
		}
		vTaskDelay(20);
	}
}


/********************************************************************************************
*	描	述:当多个标志位满足时才执行任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void EventAll_task(void *parameter)
{	
	EventBits_t value;
	while(1)
	{
		if(Event_Handle != NULL)
		{
			//当多个标志位都满足时才执行
			value = xEventGroupWaitBits(Event_Handle,0x03, pdTRUE,pdTRUE,portMAX_DELAY);
			printf("事件组标志位值为:%d\n",value);
			LED1_Toggle();
		}
		vTaskDelay(20);
	}
}


/********************************************************************************************
*	描	述:一个或多个满足就可执行任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void Event_Task(void *parameter)
{
	EventBits_t value;
	while(1)
	{
		if(Event_Handle != NULL)
		{
			//只有有一个满足就执行
			value = xEventGroupGetBits(Event_Handle);
			if(value != 0x00)
			{
				printf("获取到标志位,值为:%d\n\n",value);
				LED2_Toggle();
				//清除标志位
				xEventGroupClearBits(Event_Handle,0x03);
			}
		}
		vTaskDelay(20);
	}
}
