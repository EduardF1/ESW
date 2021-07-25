#include "transmitter.h"

//	Private variables
static MessageBufferHandle_t _payloadMessageBuffer;
static TaskHandle_t transmitter_task_handle = NULL;
static SemaphoreHandle_t _mutexSemaphore;
static messagePayload receivedPayload;

//	Transmitter task function declaration 
void transmitter_task(void* pvParameters);



/*
	Function to create the transmitter
	Parameters:
	1) MessageBufferHandle_t payloadMessageBuffer - the message buffer from which data is received
	2) SemaphoreHandle_t mutexSemaphore - mutex given by the app controller (used to protect printf statements - they are not re-entrant)
*/
void transmitter_create(MessageBufferHandle_t payloadMessageBuffer, SemaphoreHandle_t mutexSemaphore)
{
	//	Verify that the buffer and mutex are not null
	if (payloadMessageBuffer != NULL && mutexSemaphore != NULL)
	{
		//	Assign the passed message buffer and mutex variables to the local ones
		_payloadMessageBuffer = payloadMessageBuffer;
		_mutexSemaphore = mutexSemaphore;


		//	Create the app controller task
		xTaskCreate(transmitter_task,		//	Function that implements the task
			(const portCHAR*)"Transmitter task",	//	Task name
			TASKS_STACK,						//	Allocated Stack size (words, not bytes)
			NULL,								//	Parameters
			TRANSMITTER_PRIORITY,			//	Priority of the task
			&transmitter_task_handle);		//	Task handle
	}
}

//	Transmitter task body function definition
void transmitter_task(void* pvParameters) {

	xSemaphoreTake(_mutexSemaphore, portMAX_DELAY);
	printf("Transmitter started !\n\n");
	xSemaphoreGive(_mutexSemaphore);

	for (;;)
	{
		//	Receive the message (received payload - the contents at the memory address of the variable) in the buffer as long as there are any
		xMessageBufferReceive(_payloadMessageBuffer, &receivedPayload, sizeof(messagePayload), portMAX_DELAY);

		if (receivedPayload.len > 0)
		{
			//	Send the received message (receivedPayload, print the contents) 
			sendMessage(receivedPayload);
		}

	}

}

/*
	Function to send the received payload
	Parameters:
	1) messagePayload payload - payload to be sent (in our case, just printed to the terminal)
*/
void sendMessage(messagePayload payload)
{
	//	Verify that the mutex is not null
	if (_mutexSemaphore != NULL)
	{
		//	Take the mutex
		xSemaphoreTake(_mutexSemaphore, portMAX_DELAY);
		printf("Payload was received --- content:\n\n");
		//	Print payload contents as long as there are any (byte by byte)
		for (int i = 0; i < receivedPayload.len; i++)
		{
			//printf("Byte[%d]: %X\n", i, receivedPayload.bytes[i]);
			if (i < 2)
			{
				printf("Byte[%d] -- Water level -- : 0x%X\n", i, receivedPayload.bytes[i]);
			}
			else
			{
				printf("Byte[%d] -- Water temperature -- : 0x%X\n", i, receivedPayload.bytes[i]);
			}
		}
		printf("\n\n");
		//	Give the mutex (proceed)
		xSemaphoreGive(_mutexSemaphore);
	}
}