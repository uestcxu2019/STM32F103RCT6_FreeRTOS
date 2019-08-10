#include "bsp_task.h"


/**************************************************************************************************
*			当多个任务具有相同的部分时，可以通过传入参数改变,从而不用创建多个任务
*		   当使用软件延时函数时，不会出现阻塞态，创建多个任务时，只有高优先级的任务运行
*		   当采用vTaskDelay()函数时，会出现阻塞态，多个任务会轮流执行
**************************************************************************************************/

/**************************************************************************************************
*									变量定义
**************************************************************************************************/
#define Queue_Length    4		//队列的长度
#define Queue_ItemSize		4		//队列中每个消息的大小(字节)


/**************************************************************************************************
*									任务句柄
**************************************************************************************************/
TaskHandle_t AppTaskCreate_Handle = NULL;
TaskHandle_t LED1_Task_Handle = NULL;
TaskHandle_t LED2_Task_Handle = NULL;

TaskHandle_t KEY_SendTask_Handle = NULL;
TaskHandle_t Queue_ReceiveTask_Handle = NULL;

QueueHandle_t  Queue_Handle = NULL;

/********************************************************************************************
*	描	述:任务创建任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = 3;
	taskENTER_CRITICAL();	//进入临界区
	
	//创建消息队列
	Queue_Handle = xQueueCreate(Queue_Length,Queue_ItemSize);
	if(NULL != Queue_Handle)
	{
		printf("消息队列创建成功\n");
	}
		
	//创建按键发送消息任务
	xReturn = xTaskCreate(KEY_SendTask,"KEY_SendTask",126,NULL,5,&KEY_SendTask_Handle);
	if(pdPASS == xReturn)
	{
		printf("KEY_SendTask任务创建成功\n");
	}
	//创建接收消息任务
	xReturn = xTaskCreate(Queue_ReceiveTask,"Queue_ReceiveTask",126,NULL,3,&Queue_ReceiveTask_Handle);
	if(pdPASS == xReturn)
	{
		printf("Queue_ReceiveTask任务创建成功\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//退出临界区
}


/********************************************************************************************
*	描	述:LED1点亮任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void LED1_Task(void *parameter)
{	uint32_t i = 0;
	TickType_t pxPreviousWakeTime;
	TickType_t xTimeIncrement = pdMS_TO_TICKS(500);
	pxPreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		//使用绝对延时
		vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement);
		printf("time1 = %d\n",xTaskGetTickCount());
		LED1(1);
		printf("LED1 is On\n");
//		vTaskDelay(500);
		vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement);
		printf("time2 = %d\n",xTaskGetTickCount());
		LED1(0);
		printf("LED1 is Off\n");
//		vTaskDelay(500);
		i++;
		printf("%d\n",i);
	}
}


/********************************************************************************************
*	描	述:LED2点亮任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void LED2_Task(void *parameter)
{	
	
	while(1)
	{
		LED2(1);
		printf("LED2 is On\n");
		vTaskDelay(500);
		LED2(0);
		printf("LED2 is Off\n");
		vTaskDelay(500);
	}
}


/********************************************************************************************
*	描	述:按键扫描任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void KEY_Task(void *parameter)
{	
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("LED1任务挂起\n");
			vTaskSuspend(LED1_Task_Handle);	//挂起LED1
			printf("LED1任务挂起成功\n");
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("LED1任务恢复\n");
			vTaskResume(LED1_Task_Handle);
			printf("LED1任务恢复成功\n");
		}
		vTaskDelay(500);
	}
}



/********************************************************************************************
*	描	述:按键发送消息队列任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void KEY_SendTask(void *parameter)
{	
	BaseType_t xReturn = pdPASS;
	uint32_t data1 = 1;
	uint32_t data2 = 2;
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("发送消息1\n");
			xReturn = xQueueSend(Queue_Handle,&data1,0);
			if(xReturn == pdPASS)
			{
				printf("发送消息1成功\n");
			}
		}
		
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("发送消息2\n");
			xReturn = xQueueSend(Queue_Handle,&data2,0);
			if(xReturn == pdPASS)
			{
				printf("发送消息2成功\n");
			}
		}
		vTaskDelay(20);
	}
	
}


/********************************************************************************************
*	描	述:按键发送消息队列任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void Queue_ReceiveTask(void *parameter)
{
	BaseType_t xReturn = pdPASS;
	uint32_t rece_data = 0;
	while(1)
	{
		xReturn = xQueueReceive(Queue_Handle,&rece_data,0);
		if(pdPASS == xReturn)
		{
			printf("接收消息成功\n");
			printf("接收到的数据是:%d\n",rece_data);
		}
		vTaskDelay(20);
	}
}
