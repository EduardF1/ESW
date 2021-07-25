#include "appController.h"
/*
	Necessary private variable for task synchronization, protection of resources and size allocation (_payloadMessageBuffer)
	-	app controller task handle
	-	event group for data ready (measurement finished) synchronization with the water level sensor task
	-	mutex semaphore for protection of resources (printf statements - as they are not reentrant)
	-	message buffer through which the payload is sent to the transmitter task
	-	payload size (maximum possible, as per the documentation, 4 bytes are needed as extra space within the buffer)
*/
static TaskHandle_t appController_task_handle = NULL;
static EventGroupHandle_t pvEventGroup;
static SemaphoreHandle_t mutexSemaphore;
static MessageBufferHandle_t payloadMessageBuffer;
static const size_t payloadMessageBufferSizeBytes = 10;

/*
	Sensor variables - initialized with NULL (will be created within the app controller task)
*/
static waterLevelSensor_t waterLevelSensor = NULL;
static waterTemperatureSensor_t waterTemperatureSensor = NULL;

//	App controler task function declaration
void appController_task(void* pvParameters);

//	initialize all the necessary FreeRTOS elements
void appController_initialize()
{
	pvEventGroup = xEventGroupCreate();	//	Create the event group 
	mutexSemaphore = xSemaphoreCreateMutex();	//	Create the mutex
	payloadMessageBuffer = xMessageBufferCreate(payloadMessageBufferSizeBytes);	//	Create the message buffer

	//	verify if the initializations were successful
	if (pvEventGroup != NULL && mutexSemaphore != NULL && payloadMessageBuffer != NULL)
	{
		waterLevelSensor = waterLevelSensor_create(pvEventGroup, mutexSemaphore);	//	create water level sensor
		waterTemperatureSensor = waterTemperatureSensor_create(mutexSemaphore);	//	create water temperature sensor
		transmitter_create(payloadMessageBuffer, mutexSemaphore); // create transmitter driver

		//	Create the app controller task
		xTaskCreate(appController_task,		//	Function that implements the task
			(const portCHAR*)"App Controller",	//	Task name
			TASKS_STACK,						//	Allocated Stack size (words, not bytes)
			NULL,								//	Parameters
			APP_CONTROLLER_TASK_PRIORITY,			//	Priority of the task
			&appController_task_handle);		//	Task handle
	}
}

//	App controler task function body definition
void appController_task(void* pvParameters)
{
	for (;;)
	{
		//	Set the measurement bit (bit 0) in the data ready event group
		xEventGroupSetBits(pvEventGroup, WATER_LEVEL_MEASUREMENT_BIT);
		/*	xEventGroupWaitBits function parameters:
			event group name,
			the bits that are waited for (bit 1),
			clear bits before return (pdTRUE - true),
			wait for the specified bit(s) (pdTRUE - true),
			wait indefinitely (portMAX_DELAY)
		*/
		xEventGroupWaitBits(pvEventGroup, WATER_LEVEL_DATA_READY_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
		/*
			Retrieve latest measurements of the water level and water temperature from the
			water level sensor and water temperature sensor
		*/
		uint16_t waterLevel = waterLevelSensor_getLastWaterLevel(waterLevelSensor);
		int8_t waterTemperature = waterTemperatureSensor_getLastWaterTemperature(waterTemperatureSensor);
		/*
			Declare and initialize the payload that will be sent
			Payload initialization:
			- .len = 4 (4 bytes to send)
			- .bytes = {0} (all bytes set to 0)
		*/
		messagePayload payloadMessage = { .len = 4, .bytes = {0} };
		//	Assemble the payload
		payloadMessage.bytes[0] = waterLevel >> 8;	//	bit-wise shift right 8 positions
		payloadMessage.bytes[1] = waterLevel & 0xFF;	//	bit-wise AND with 0b11111111
		payloadMessage.bytes[2] = waterTemperature >> 8;  //	bit-wise shift right 8 positions
		payloadMessage.bytes[3] = waterTemperature & 0xFF;	//	bit-wise AND with 0b11111111
		/*
			Send the payload to the Message buffer
			Parameters:
			1)	payloadMessageBuffer - message buffer to which to send the data
			2)	&message - the message's memory location contents (point to the data content)
			3)	sizeof(messagePayload) - message size (data length)
			4)	portMAX_DELAY - wait time (indefinite) for the buffer to block the receiving task (if there are no contents written in the buffer by the App controller task - writer)
		*/
		xMessageBufferSend(payloadMessageBuffer, &payloadMessage, sizeof(messagePayload), portMAX_DELAY);
		xSemaphoreTake(mutexSemaphore, portMAX_DELAY);
		printf("Payload sent to the transmitter !\n\n");
		xSemaphoreGive(mutexSemaphore);
		vTaskDelay(pdMS_TO_TICKS(1000));	//	delay for 1 S

	}
}

