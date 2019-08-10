#include "bsp_task.h"


/**************************************************************************************************
*			��������������ͬ�Ĳ���ʱ������ͨ����������ı�,�Ӷ����ô����������
*		   ��ʹ�������ʱ����ʱ�������������̬�������������ʱ��ֻ�и����ȼ�����������
*		   ������vTaskDelay()����ʱ�����������̬��������������ִ��
**************************************************************************************************/

/**************************************************************************************************
*									��������
**************************************************************************************************/
#define Queue_Length    4		//���еĳ���
#define Queue_ItemSize		4		//������ÿ����Ϣ�Ĵ�С(�ֽ�)


/**************************************************************************************************
*									������
**************************************************************************************************/
TaskHandle_t AppTaskCreate_Handle = NULL;
TaskHandle_t LED1_Task_Handle = NULL;
TaskHandle_t LED2_Task_Handle = NULL;

TaskHandle_t KEY_SendTask_Handle = NULL;
TaskHandle_t Queue_ReceiveTask_Handle = NULL;

QueueHandle_t  Queue_Handle = NULL;

/********************************************************************************************
*	��	��:���񴴽�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = 3;
	taskENTER_CRITICAL();	//�����ٽ���
	
	//������Ϣ����
	Queue_Handle = xQueueCreate(Queue_Length,Queue_ItemSize);
	if(NULL != Queue_Handle)
	{
		printf("��Ϣ���д����ɹ�\n");
	}
		
	//��������������Ϣ����
	xReturn = xTaskCreate(KEY_SendTask,"KEY_SendTask",126,NULL,5,&KEY_SendTask_Handle);
	if(pdPASS == xReturn)
	{
		printf("KEY_SendTask���񴴽��ɹ�\n");
	}
	//����������Ϣ����
	xReturn = xTaskCreate(Queue_ReceiveTask,"Queue_ReceiveTask",126,NULL,3,&Queue_ReceiveTask_Handle);
	if(pdPASS == xReturn)
	{
		printf("Queue_ReceiveTask���񴴽��ɹ�\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//�˳��ٽ���
}


/********************************************************************************************
*	��	��:LED1����������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void LED1_Task(void *parameter)
{	uint32_t i = 0;
	TickType_t pxPreviousWakeTime;
	TickType_t xTimeIncrement = pdMS_TO_TICKS(500);
	pxPreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		//ʹ�þ�����ʱ
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
*	��	��:LED2����������
*	��	��:��
*	����ֵ:��
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
*	��	��:����ɨ��������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void KEY_Task(void *parameter)
{	
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("LED1�������\n");
			vTaskSuspend(LED1_Task_Handle);	//����LED1
			printf("LED1�������ɹ�\n");
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("LED1����ָ�\n");
			vTaskResume(LED1_Task_Handle);
			printf("LED1����ָ��ɹ�\n");
		}
		vTaskDelay(500);
	}
}



/********************************************************************************************
*	��	��:����������Ϣ����������
*	��	��:��
*	����ֵ:��
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
			printf("������Ϣ1\n");
			xReturn = xQueueSend(Queue_Handle,&data1,0);
			if(xReturn == pdPASS)
			{
				printf("������Ϣ1�ɹ�\n");
			}
		}
		
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			printf("������Ϣ2\n");
			xReturn = xQueueSend(Queue_Handle,&data2,0);
			if(xReturn == pdPASS)
			{
				printf("������Ϣ2�ɹ�\n");
			}
		}
		vTaskDelay(20);
	}
	
}


/********************************************************************************************
*	��	��:����������Ϣ����������
*	��	��:��
*	����ֵ:��
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
			printf("������Ϣ�ɹ�\n");
			printf("���յ���������:%d\n",rece_data);
		}
		vTaskDelay(20);
	}
}
