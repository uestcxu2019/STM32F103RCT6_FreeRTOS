#include "bsp_task.h"


/**************************************************************************************************
*					��������:
*							�����ź���ʵ��(���ȼ���ת)
*							�����������񣬸ߡ��С������ȼ�
*
*
*							�����ź�����һ��ӵ�����ȼ��̳еĶ�ֵ�ź����������ڻ������Ӧ����
*							�����ź����൱��һ��Կ�ף���������Ҫʹ����Դʱ������Կ�ף�ʹ�������Ҫ�黹
*							(��ظ�����ȼ��̳У����ȼ���ת)
**************************************************************************************************/

//			���ȼ�:����Խ�����ȼ�Խ��

/**************************************************************************************************
*		   ��������������ͬ�Ĳ���ʱ������ͨ����������ı�,�Ӷ����ô����������
*		   ��ʹ�������ʱ����ʱ�������������̬�������������ʱ��ֻ�и����ȼ�����������
*		   ������vTaskDelay()����ʱ�����������̬��������������ִ�У�
*		   ���ÿ������ִ�ж��б�Ҫ����ʱ���������ָ����ȼ�����һֱռ��CPU
**************************************************************************************************/

/**************************************************************************************************
*									��������
**************************************************************************************************/


/**************************************************************************************************
*									������
**************************************************************************************************/
TaskHandle_t AppTaskCreate_Handle = NULL;		//��������������

SemaphoreHandle_t MutexSem_Handle =NULL;		//�ź������



/********************************************************************************************
*	��	��:���񴴽�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn = pdPASS;
	taskENTER_CRITICAL();	//�����ٽ���
		
	//���������ź���(��Ҫ��configUSE_MUTEXES��)
	MutexSem_Handle = xSemaphoreCreateMutex();		
	if(NULL != MutexSem_Handle)
	{
		printf("�����ź��������ɹ�\n");
	}
//	xSemaphoreGive(MutexSem_Handle);	//�ͷŻ����ź���
	
	//���������ȼ�����
	xReturn = xTaskCreate(LowPriority_Task,"LowPriority_Task",126,NULL,2,NULL);
	if(pdPASS == xReturn)
	{
		printf("LowPriority_Task���񴴽��ɹ�\n");
	}
	
	//�������������ȼ�����
	xReturn = xTaskCreate(MidPriority_Task,"MidPriority_Task",126,NULL,3,NULL);
	if(pdPASS == xReturn)
	{
		printf("MidPriority_Task���񴴽��ɹ�\n");
	}
	
	//�������������ȼ�����
	xReturn = xTaskCreate(HighPriority_Task,"HighPriority_Task",126,NULL,4,NULL);
	if(pdPASS == xReturn)
	{
		printf("HighPriority_Task���񴴽��ɹ�\n");
	}
	
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();	//�˳��ٽ���
}


/********************************************************************************************
*	��	��:�����ȼ�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void LowPriority_Task(void *parameter)
{	
	uint32_t i;
	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("LowPriority_Task��ȡ�ź���\n");
		xReturn = xSemaphoreTake(MutexSem_Handle,portMAX_DELAY);
		if(pdPASS == xReturn)
			printf("LowPriority_Task������\n");
			for(i = 0;i< 2000000;i++)		//��δ���
			{
				taskYIELD();
			}
		printf("LowPriority_Task�ͷ��ź���\n");
			xSemaphoreGive(MutexSem_Handle);
		vTaskDelay(500);
	}
}


/********************************************************************************************
*	��	��:�����ȼ�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void MidPriority_Task(void *parameter)
{	
//	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("MidPriority_Task������\n");
		vTaskDelay(500);
	}
}


/********************************************************************************************
*	��	��:�����ȼ�������
*	��	��:��
*	����ֵ:��
********************************************************************************************/
void HighPriority_Task(void *parameter)
{	
	UBaseType_t xReturn = pdPASS;
	while(1)
	{
		printf("HighPriority_Task��ȡ�ź���\n");
		xReturn = xSemaphoreTake(MutexSem_Handle,portMAX_DELAY);
		if(pdPASS == xReturn)
			printf("HighPriority_Task������\n");
			taskYIELD();
		printf("HighPriority_Task�ͷ��ź���\n");
			xSemaphoreGive(MutexSem_Handle);
		vTaskDelay(500);
	}
}
