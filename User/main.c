#include "stm32f10x.h"
#include "bsp_led.h"
#include "FreeRTOS.h"			  
#include "task.h" 


/**************************************************************************************************
*		动态任务创建步骤
*					1.创建任务函数
*					2.创建任务创建APPTaskCreate函数
*					3.在APPTaskCreate函数创建任务函数，创建成功后，删除APPTaskCreate函数
*				需要在FreeRTOSConfig.h文件中开启configSUPPORT_DYNAMIC_ALLOCATION宏
**************************************************************************************************/


/**************************************************************************************************
*									任务句柄
**************************************************************************************************/
/* 创建任务函数任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务函数任务句柄 */
static TaskHandle_t LED_Task_Handle;

/**************************************************************************************************
*									函数声明
**************************************************************************************************/
static void BSP_Init(void);
static void LED_Task(void *parameter);


static void AppTaskCreate(void *parameter);
static void LED_Task(void *parameter);



/**************************************************************************************************
*									变量定义
**************************************************************************************************/


/**************************************************************************************************/
int main(void)	
{
	BaseType_t xReturn = pdPASS;
	//开发板硬件初始化
	BSP_Init();
	printf("初始化完成\n");
	
	xReturn = xTaskCreate(AppTaskCreate,			//任务函数
									   "AppTaskCreate任务",			//任务名称
										128,				//任务堆栈大小
										NULL,				//传递给任务函数的参数
										3,					//任务优先级
										AppTaskCreate_Handle);	
	if(pdPASS == xReturn)//创建成功
	{
		//启动任务调度器
		vTaskStartScheduler();
	}
	while(1);	//正常执行不会到这里来
}

/********************************************************************************************
*	描	述:与开发板相关的板级外设
*	参	数:无
*	返回值:无
********************************************************************************************/
static void BSP_Init(void)
{
	//stm32的优先级分组,全部用于抢占优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* LED初始化 */
	Led_Init();
	
	/*串口初始化 */
	UART_Init();
}


/********************************************************************************************
*	描	述:任务创建任务函数，用于创建任务
*	参	数:无
*	返回值:无
********************************************************************************************/
static void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL(); //进入临界区
	/* 创建LED——Task任务 */
	xReturn = xTaskCreate(LED_Task,			//任务函数
				   "LED任务",			//任务名称
				   128,				//任务堆栈大小
				   NULL,				//传递给任务函数的参数
				   4,					//任务优先级
				   LED_Task_Handle);
	
	if(pdPASS == xReturn) /* LED任务创建成功 */
	{
		printf("任务创建成功\n");
	}
	else
	{
		printf("任务创建失败\n");
	}
	vTaskDelete(AppTaskCreate_Handle);	/* 删除AppTaskCreate任务 */
	taskEXIT_CRITICAL();		//退出临界区
}


/********************************************************************************************
*	描	述:LED1闪灯任务函数
*	参	数:无
*	返回值:无
********************************************************************************************/
static void LED_Task(void *parameter)
{
	while(1)
	{
		LED2(1);
		vTaskDelay(500);
		printf("LED Task Running,led1_on\n");
		
		LED2(0);
		vTaskDelay(500);
		printf("LED Task Running,led1_off\n");
	}
}
