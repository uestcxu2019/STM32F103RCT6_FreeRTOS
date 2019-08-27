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
#include "timers.h"


void AppTaskCreate(void *parameter);

void Period_CallBack(TimerHandle_t xTimer);
void oneTime_CallBack(TimerHandle_t xTimer);
void KEY_ControlTask(void *parameter);

#endif  /* __BSP_TASK_H */
