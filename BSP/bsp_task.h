#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "FreeRTOS.h"			  
#include "task.h"
#include "queue.h"
#include "bsp_uart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "semphr.h"
#include "event_groups.h"


void AppTaskCreate(void *parameter);

void KEY_Task(void *parameter);
void Event_Task(void *parameter);
void EventAll_task(void *parameter);



#endif  /* __BSP_TASK_H */
