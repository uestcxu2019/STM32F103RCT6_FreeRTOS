#include "bsp_task.h"


/**************************************************************************************************
*					功能描述:
*							 任务通知学习(模拟消息队列实验,用于向任务发送消息)
*							 
*							 
*							 
*							 
*					任务通知
*							1.无需创建
*						    2.任务/中断均能发送通知
*						    3.只有任务可以等待通知，中断中不允许等待通知
*							4.任务通知只能有一个接收任务
*								
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

TaskHandle_t taskSend_Handle = NULL;			//创建任务任务句柄
TaskHandle_t taskReceive1_Handle = NULL;		//创建任务任务句柄
TaskHandle_t taskReceive2_Handle = NULL;		//创建任务任务句柄

/********************************************************************************************
*	描	述:任务创建任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn;
	taskENTER_CRITICAL();	//进入临界区
			
	//创建按键任务
	 xReturn = xTaskCreate(taskSend,"taskSend",126,NULL,4,&taskSend_Handle);
	if(pdPASS == xReturn)
	{
		printf("taskSend创建成功\n");
	}
	
	xReturn = xTaskCreate(taskReceive1,"taskReceive1",126,NULL,3,&taskReceive1_Handle);
	if(pdPASS == xReturn)
	{
		printf("taskReceive1_Handle创建成功\n");
	}
	
	xReturn = xTaskCreate(taskReceive2,"taskReceive2",126,NULL,2,&taskReceive2_Handle);
	if(pdPASS == xReturn)
	{
		printf("taskReceive2_Handle创建成功\n\n");
	}
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//退出临界区
}



/********************************************************************************************
*	描	述:任务通知发送任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void taskSend(void *parameter)
{
	uint8_t send_data1[] = "Send data";
	while(1)
	{
		if(KEY_ON == KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			LED1_Toggle();
			//发送任务通知给接收任务1
			xTaskNotify(taskReceive1_Handle,(uint32_t )&send_data1,eSetValueWithOverwrite);
			printf("向接收任务1发送通知值成功\n");
		}
		
		if(KEY_ON == KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			LED1_Toggle();
			//发送任务通知给接收任务1
			xTaskNotify(taskReceive2_Handle,2,eSetValueWithOverwrite);
			printf("向接收任务2发送通知值成功\n");
		}
		vTaskDelay(20);
	}
}


/********************************************************************************************
*	描	述:任务通知接收任务函数1
*	参	数:无
*	返回值:无
********************************************************************************************/
void taskReceive1(void *parameter)
{
	uint32_t data =0;
	char *r_char; 
	while(1)
	{
		xTaskNotifyWait(0x00,0xFF,(uint32_t *)&r_char,portMAX_DELAY);
		printf("taskReceive1接收到任务通知,通知值为%s\n\n",r_char);
		vTaskDelay(20);
	}
}


/********************************************************************************************
*	描	述:任务通知接收任务函数2
*	参	数:无
*	返回值:无
********************************************************************************************/
void taskReceive2(void *parameter)
{
	uint32_t data =0;
	while(1)
	{
		xTaskNotifyWait(0x00,0xFF,&data,portMAX_DELAY);
		printf("taskReceive2接收到任务通知,通知值为%d\n\n",data);
		vTaskDelay(20);
	}
}




