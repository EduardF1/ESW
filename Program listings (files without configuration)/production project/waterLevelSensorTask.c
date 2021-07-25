#include "waterLevelSensor.h"

//	Water level sensor ADT definition
typedef struct waterLevelSensor {
	uint16_t _lastWaterLevel;
	EventGroupHandle_t _pvEventGroup;
	TaskHandle_t _taskHandle;
	SemaphoreHandle_t _mutexSemaphore;

}waterLevelSensor;

//	private instance variable
static waterLevelSensor_t instance;

//	Water level sensor task function declaration
void waterLevelSensor_task(void* pvParameters);

/*
	Function to create the water level sensor ADT object
	Parameters:
	1) EventGroupHandle_t pvEventGroup - event group used for synchronization with the app controller task (passed as parameter/argument)
	2) SemaphoreHandle_t mutexSemaphore - mutex given by the app controller (used to protect printf statements - they are not re-entrant)
	Return: the Water level sensor ADT object
*/
waterLevelSensor_t waterLevelSensor_create(EventGroupHandle_t pvEventGroup, SemaphoreHandle_t mutexSemaphore)
{
	//	Allocate memory for the object
	instance = malloc(sizeof(waterLevelSensor));

	//	Verify that memory was available
	if (instance == NULL)
	{
		return NULL;
	}

	//	Set the Water level sensor's inner variables
	instance->_lastWaterLevel = 100;
	instance->_pvEventGroup = pvEventGroup;
	instance->_taskHandle = NULL;
	instance->_mutexSemaphore = mutexSemaphore;

	//	Create the Water level sensor task
	xTaskCreate(waterLevelSensor_task,
		(const portCHAR*)"Water level task",
		TASKS_STACK,
		NULL,
		WATER_LEVEL_TASK_PRIORITY,
		&instance->_taskHandle);

	return instance;
}

//	Function to execute a measurement on the Water level sensor object
void waterLevelSensor_meassure(waterLevelSensor_t self)
{
	self->_lastWaterLevel++;
}
//	Water level sensor task function definition
void waterLevelSensor_task(void* pvParameters)
{

	xSemaphoreTake(instance->_mutexSemaphore, portMAX_DELAY);
	printf("Water level sensor task is running\n\n");
	xSemaphoreGive(instance->_mutexSemaphore);
	for (;;)
	{
		/*	Wait for the bits to be set;
			parameters:
			-	event group variable that is part of the waterLevelSensor,
			-   WATER_LEVEL_DATA_READY_BIT - bit that is waited for,
			-	clear bits before return (pdTRUE - true),
			-	wait for all bits (pdTRUE - true),
			-	wait indefinitely (portMAX_DELAY)
		*/
		xEventGroupWaitBits(instance->_pvEventGroup, WATER_LEVEL_MEASUREMENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

		//	execute a measurement on the Water level sensor object
		waterLevelSensor_meassure(instance);
		//	delay for 100 ms
		vTaskDelay(pdMS_TO_TICKS(100));

		xSemaphoreTake(instance->_mutexSemaphore, portMAX_DELAY);
		printf("Water level --- current (Liters): %d \n\n", instance->_lastWaterLevel);
		xSemaphoreGive(instance->_mutexSemaphore);

		//	set the bit in the event group 
		xEventGroupSetBits(instance->_pvEventGroup, WATER_LEVEL_DATA_READY_BIT);

	}
}
//	Function to return the latest water level meassured by the Water level sensor object
uint16_t waterLevelSensor_getLastWaterLevel(waterLevelSensor_t self)
{
	if (self->_lastWaterLevel == 0)
	{
		return 0;
	}
	return self->_lastWaterLevel;
}