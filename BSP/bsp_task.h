#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "FreeRTOS.h"			  
#include "task.h" 
#include "bsp_uart.h"
#include "bsp_led.h"
#include "bsp_key.h"

void AppTaskCreate(void *parameter);
void LED1_Task(void *parameter);
void LED2_Task(void *parameter);
void KEY_Task(void *parameter);


#endif  /* __BSP_TASK_H */
