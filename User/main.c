#include "stm32f10x.h"
#include "bsp_led.h"
#include "FreeRTOS.h"			  
#include "task.h" 

/**************************************************************************************************
*		使用静态方法创建进行任务的创建和删除时需要实现需要用户
*		实现空闲任务和定时器任务的任务堆栈，任务控制块
**************************************************************************************************/

/**************************************************************************************************
*		静态任务创建步骤
*					1.创建任务函数
*					2.创建任务创建APPTaskCreate函数
*					3.在APPTaskCreate函数创建任务函数，创建成功后，删除APPTaskCreate函数
*				需要在FreeRTOSConfig.h中开启configSUPPORT_STATIC_ALLOCATION宏
*				需要用户实现空闲任务函数和定时器任务函数
**************************************************************************************************/


/* 任务创建任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED任务堆栈 */
static StackType_t LED_Task_Stack[128];

/* 任务创建任务控制块 */
static StaticTask_t AppTaskCreate_TCB;
/* LED任务控制块 */
static StaticTask_t LED_Task_TCB;


/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;


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

/**
	* 使用了静态分配内存，以下这两个函数是由用户实现，函数在task.c文件中有引用
	*	当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
	*/

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize);

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
								   StackType_t **ppxTimerTaskStackBuffer, 
								   uint32_t *pulTimerTaskStackSize);


/**************************************************************************************************
*									变量定义
**************************************************************************************************/


/**************************************************************************************************/
int main(void)	
{
	//开发板硬件初始化
	BSP_Init();
	LED1(1);
	printf("初始化完成\n");
	
	AppTaskCreate_Handle = xTaskCreateStatic(AppTaskCreate,			//任务函数
											"AppTaskCreate任务",			//任务名称
											128,				//任务堆栈大小
											NULL,				//传递给任务函数的参数
											3,					//任务优先级
											AppTaskCreate_Stack,		//任务堆栈
											&AppTaskCreate_TCB );	//任务控制块
	if(NULL != AppTaskCreate_Handle)//创建成功
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
	taskENTER_CRITICAL(); //进入临界区
	/* 创建LED——Task任务 */
	LED_Task_Handle = xTaskCreateStatic(LED_Task,			//任务函数
										"LED任务",			//任务名称
								        128,				//任务堆栈大小
										NULL,				//传递给任务函数的参数
										4,					//任务优先级
										LED_Task_Stack,		//任务堆栈
										&LED_Task_TCB );	//任务控制块
	
	if(NULL != LED_Task_Handle) /* LED任务创建成功 */
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
		LED1(1);
		vTaskDelay(500);
		printf("LED Task Running,led1_on\n");
		
		LED1(0);
		vTaskDelay(500);
		printf("LED Task Running,led1_off\n");
	}
}


/********************************************************************************************
*	描	述:空闲任务
*	参	数: ppxIdleTaskTCBBuffer 任务控制块内存
*			ppxIdleTaskStackBuffer 任务堆栈内存
*			pulIdleTaskStackSize 任务堆栈大小
*	返回值:无
********************************************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &Idle_Task_TCB;				/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer = Idle_Task_Stack;			/* 任务堆栈内存 */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;	/* 任务堆栈大小 */
}


/********************************************************************************************
*	描	述:定时器任务
*	参	数: ppxIdleTaskTCBBuffer 任务控制块内存
*			ppxIdleTaskStackBuffer 任务堆栈内存
*			pulIdleTaskStackSize 任务堆栈大小
*	返回值:无
********************************************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
								   StackType_t **ppxTimerTaskStackBuffer, 
								   uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &Timer_Task_TCB;				/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer = Timer_Task_Stack;			/* 任务堆栈内存 */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;	/* 任务堆栈大小 */
}
