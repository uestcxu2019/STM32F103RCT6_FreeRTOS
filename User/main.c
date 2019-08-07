#include "stm32f10x.h"
#include "bsp_led.h"
#include "FreeRTOS.h"			  
#include "task.h" 

/**************************************************************************************************
*		ʹ�þ�̬����������������Ĵ�����ɾ��ʱ��Ҫʵ����Ҫ�û�
*		ʵ�ֿ�������Ͷ�ʱ������������ջ��������ƿ�
**************************************************************************************************/

/**************************************************************************************************
*		��̬���񴴽�����
*					1.����������
*					2.�������񴴽�APPTaskCreate����
*					3.��APPTaskCreate���������������������ɹ���ɾ��APPTaskCreate����
*				��Ҫ��FreeRTOSConfig.h�п���configSUPPORT_STATIC_ALLOCATION��
*				��Ҫ�û�ʵ�ֿ����������Ͷ�ʱ��������
**************************************************************************************************/


/* ���񴴽������ջ */
static StackType_t AppTaskCreate_Stack[128];
/* LED�����ջ */
static StackType_t LED_Task_Stack[128];

/* ���񴴽�������ƿ� */
static StaticTask_t AppTaskCreate_TCB;
/* LED������ƿ� */
static StaticTask_t LED_Task_TCB;


/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;


/**************************************************************************************************
*									������
**************************************************************************************************/
/* ���������������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������������ */
static TaskHandle_t LED_Task_Handle;

/**************************************************************************************************
*									��������
**************************************************************************************************/
static void BSP_Init(void);
static void LED_Task(void *parameter);


static void AppTaskCreate(void *parameter);
static void LED_Task(void *parameter);

/**
	* ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
	*	���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
	*/

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize);

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
								   StackType_t **ppxTimerTaskStackBuffer, 
								   uint32_t *pulTimerTaskStackSize);


/**************************************************************************************************
*									��������
**************************************************************************************************/


/**************************************************************************************************/
int main(void)	
{
	//������Ӳ����ʼ��
	BSP_Init();
	LED1(1);
	printf("��ʼ�����\n");
	
	AppTaskCreate_Handle = xTaskCreateStatic(AppTaskCreate,			//������
											"AppTaskCreate����",			//��������
											128,				//�����ջ��С
											NULL,				//���ݸ��������Ĳ���
											3,					//�������ȼ�
											AppTaskCreate_Stack,		//�����ջ
											&AppTaskCreate_TCB );	//������ƿ�
	if(NULL != AppTaskCreate_Handle)//�����ɹ�
	{
		//�������������
		vTaskStartScheduler();
	}
	while(1);	//����ִ�в��ᵽ������
}

/********************************************************************************************
*	��	��:�뿪������صİ弶����
*	��	��:��
*	����ֵ:��
********************************************************************************************/
static void BSP_Init(void)
{
	//stm32�����ȼ�����,ȫ��������ռ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* LED��ʼ�� */
	Led_Init();
	
	/*���ڳ�ʼ�� */
	UART_Init();
}


/********************************************************************************************
*	��	��:���񴴽������������ڴ�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
static void AppTaskCreate(void *parameter)
{
	taskENTER_CRITICAL(); //�����ٽ���
	/* ����LED����Task���� */
	LED_Task_Handle = xTaskCreateStatic(LED_Task,			//������
										"LED����",			//��������
								        128,				//�����ջ��С
										NULL,				//���ݸ��������Ĳ���
										4,					//�������ȼ�
										LED_Task_Stack,		//�����ջ
										&LED_Task_TCB );	//������ƿ�
	
	if(NULL != LED_Task_Handle) /* LED���񴴽��ɹ� */
	{
		printf("���񴴽��ɹ�\n");
	}
	else
	{
		printf("���񴴽�ʧ��\n");
	}
	vTaskDelete(AppTaskCreate_Handle);	/* ɾ��AppTaskCreate���� */
	taskEXIT_CRITICAL();		//�˳��ٽ���
}


/********************************************************************************************
*	��	��:LED1����������
*	��	��:��
*	����ֵ:��
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
*	��	��:��������
*	��	��: ppxIdleTaskTCBBuffer ������ƿ��ڴ�
*			ppxIdleTaskStackBuffer �����ջ�ڴ�
*			pulIdleTaskStackSize �����ջ��С
*	����ֵ:��
********************************************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &Idle_Task_TCB;				/* ������ƿ��ڴ� */
	*ppxIdleTaskStackBuffer = Idle_Task_Stack;			/* �����ջ�ڴ� */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;	/* �����ջ��С */
}


/********************************************************************************************
*	��	��:��ʱ������
*	��	��: ppxIdleTaskTCBBuffer ������ƿ��ڴ�
*			ppxIdleTaskStackBuffer �����ջ�ڴ�
*			pulIdleTaskStackSize �����ջ��С
*	����ֵ:��
********************************************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
								   StackType_t **ppxTimerTaskStackBuffer, 
								   uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &Timer_Task_TCB;				/* ������ƿ��ڴ� */
	*ppxTimerTaskStackBuffer = Timer_Task_Stack;			/* �����ջ�ڴ� */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;	/* �����ջ��С */
}
