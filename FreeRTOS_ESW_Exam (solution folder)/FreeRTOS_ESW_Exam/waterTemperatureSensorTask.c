#include "waterTemperatureSensor.h"

//	Water temperature sensor ADT definition
typedef struct waterTemperatureSensor {
	int8_t _lastWaterTemperature;
	TaskHandle_t _taskHandle;
	SemaphoreHandle_t _mutexSemaphore;
}waterTemperatureSensor;

//	private instance variable
static waterTemperatureSensor_t instance;

//	Water temperature sensor task function declaration
void waterTemperatureSensor_task(void* pvParameters);

/*
	Function to create the water temperature sensor ADT object
	Parameters:
	1) SemaphoreHandle_t mutexSemaphore - mutex given by the app controller (used to protect printf statements - they are not re-entrant)
	Return: the Water temperature sensor ADT object
*/
waterTemperatureSensor_t waterTemperatureSensor_create(SemaphoreHandle_t mutexSemaphore)
{
	//	Allocate memory for the object
	instance = malloc(sizeof(waterTemperatureSensor));

	//	Verify that memory was available
	if (instance == NULL)
	{
		return NULL;
	}

	//	Set the Water temperature sensor's inner variables
	instance->_lastWaterTemperature = 30;
	instance->_taskHandle = NULL;
	instance->_mutexSemaphore = mutexSemaphore;

	//	Create the Water temperature sensor task
	xTaskCreate(waterTemperatureSensor_task,
		(const portCHAR*)"Water temperature task",
		TASKS_STACK,
		NULL,
		WATER_TEMPERATURE_PRIORITY,
		&instance->_taskHandle);

	return instance;
}

//	Function to execute a measurement on the Water temperature sensor object
static void waterTemperatureSensor_measure(waterTemperatureSensor_t self) {
	self->_lastWaterTemperature++;
}

//	Water temperature sensor task function definition
void waterTemperatureSensor_task(void* pvParameters) {

	xSemaphoreTake(instance->_mutexSemaphore, portMAX_DELAY);
	printf("Water temperature sensor task is running\n\n");
	xSemaphoreGive(instance->_mutexSemaphore);

	for (;;)
	{

		//	execute a measurement on the Water temperature sensor object
		waterTemperatureSensor_measure(instance);

		//	Reset: set _lastWaterTemperature to 1 if _lastWaterTemperature > 50
		if (instance->_lastWaterTemperature > 50)
		{
			xSemaphoreTake(instance->_mutexSemaphore, portMAX_DELAY);
			printf("Water temperature exceeds 50%c --- cooldown initiated\n\n", 248);
			xSemaphoreGive(instance->_mutexSemaphore);
			if (instance->_lastWaterTemperature > 50)
			{
				instance->_lastWaterTemperature = 1;
			}
		}

		xSemaphoreTake(instance->_mutexSemaphore, portMAX_DELAY);
		printf("Water temperature --- current: %d%c\n", instance->_lastWaterTemperature, 248);
		xSemaphoreGive(instance->_mutexSemaphore);
		//	delay for 75 ms
		vTaskDelay(pdMS_TO_TICKS(75));
	}
}


//	Function to return the latest water temperature meassured by the Water temperature sensor object
int8_t waterTemperatureSensor_getLastWaterTemperature(waterTemperatureSensor_t self) {
	if (self->_lastWaterTemperature == 0)
	{
		return 0;
	}
	return self->_lastWaterTemperature;
}