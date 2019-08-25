#include "bsp_task.h"


/**************************************************************************************************
*					功能描述:
*							互斥信号量实验(优先级翻转)
*							创建三个任务，高、中、低优先级
*
*
*							互斥信号量是一个拥有优先级继承的二值信号量，适用于互斥访问应用中
*							互斥信号量相当于一个钥匙，当任务需要使用资源时必须获得钥匙，使用完后需要归还
*							(相关概念：优先级继承，优先级翻转)
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

SemaphoreHandle_t MutexSem_Handle =NULL;		//信号量句柄



/********************************************************************************************
*	描	述:任务创建任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL();	//进入临界区
		
	//创建互斥信号量(需要打开configUSE_MUTEXES宏)
	MutexSem_Handle = xSemaphoreCreateMutex();		
	if(NULL != MutexSem_Handle)
	{
		printf("互斥信号量创建成功\n");
	}
//	xSemaphoreGive(MutexSem_Handle);	//释放互斥信号量
	
	//创建低优先级任务
	xReturn = xTaskCreate(LowPriority_Task,"LowPriority_Task",126,NULL,2,NULL);
	if(pdPASS == xReturn)
	{
		printf("LowPriority_Task任务创建成功\n");
	}
	
	//创建创建低优先级任务
	xReturn = xTaskCreate(MidPriority_Task,"MidPriority_Task",126,NULL,3,NULL);
	if(pdPASS == xReturn)
	{
		printf("MidPriority_Task任务创建成功\n");
	}
	
	//创建创建高优先级任务
	xReturn = xTaskCreate(HighPriority_Task,"HighPriority_Task",126,NULL,4,NULL);
	if(pdPASS == xReturn)
	{
		printf("HighPriority_Task任务创建成功\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//退出临界区
}


/********************************************************************************************
*	描	述:低优先级任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void LowPriority_Task(void *parameter)
{	
	uint32_t i;
	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("LowPriority_Task获取信号量\n");
		xReturn = xSemaphoreTake(MutexSem_Handle,portMAX_DELAY);
		if(pdPASS == xReturn)
			printf("LowPriority_Task运行中\n");
			for(i = 0;i< 2000000;i++)		//暂未理解
			{
				taskYIELD();
			}
		printf("LowPriority_Task释放信号量\n");
			xSemaphoreGive(MutexSem_Handle);
		vTaskDelay(500);
	}
}


/********************************************************************************************
*	描	述:中优先级任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void MidPriority_Task(void *parameter)
{	
//	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("MidPriority_Task运行中\n");
		vTaskDelay(500);
	}
}


/********************************************************************************************
*	描	述:高优先级任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
void HighPriority_Task(void *parameter)
{	
	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("HighPriority_Task获取信号量\n");
		xReturn = xSemaphoreTake(MutexSem_Handle,portMAX_DELAY);
		if(pdPASS == xReturn)
			printf("HighPriority_Task运行中\n");
			taskYIELD();
		printf("HighPriority_Task释放信号量\n");
			xSemaphoreGive(MutexSem_Handle);
		vTaskDelay(500);
	}
}
