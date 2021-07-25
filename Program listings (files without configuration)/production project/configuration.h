#pragma once

//	FreeRTOS dependencies
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <message_buffer.h>
#include <semphr.h>

#include <stdint.h>

//	 Task priorities setup macros
#define APP_CONTROLLER_TASK_PRIORITY	 ( tskIDLE_PRIORITY + 2 )
#define WATER_LEVEL_TASK_PRIORITY		 ( tskIDLE_PRIORITY + 2 )
#define WATER_TEMPERATURE_PRIORITY		 ( tskIDLE_PRIORITY + 1 )
#define TRANSMITTER_PRIORITY		     ( tskIDLE_PRIORITY + 1 )

//	Task Stack size (words, not bytes) 
#define TASKS_STACK					( configMINIMAL_STACK_SIZE )

//	WATER LEVEL event group bits
#define WATER_LEVEL_MEASUREMENT_BIT (1 << 0) // bit 0
#define WATER_LEVEL_DATA_READY_BIT (1 << 1) // bit 1
