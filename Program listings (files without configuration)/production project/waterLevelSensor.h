#pragma once

#include "configuration.h"

//	Pointer to the waterLevelSensor ADT object (declaration)
typedef struct waterLevelSensor* waterLevelSensor_t;

/*
	Function to create the water level sensor ADT externally
	Parameters:
	1)	EventGroupHandle_t pvEventGroup - event group used for synchronization with the Application controller task
		(app_controller task). Will be the event group for which a bit is waited for and then set in the waterLevelSensor task
	2) SemaphoreHandle_t mutexSemaphore - mutex required for standard I/O functions (i.e. printf) as they are not
	   re-entrant (they block)
	Return:	an water level sensor object (type: waterLevelSensor_t)
*/
waterLevelSensor_t waterLevelSensor_create(EventGroupHandle_t pvEventGroup, SemaphoreHandle_t mutexSemaphore);

/*
	Function to execute a sensor measurement for the water level sensor ADT
	Parameters:
	1)	waterLevelSensor_t self - reference to the water level sensor ADT that is to execute a measurement
*/
void waterLevelSensor_meassure(waterLevelSensor_t self);

/*
	Function to retrieve the last water level from the water level sensor
	Parameters:
	1)	waterLevelSensor_t self - reference to the water level sensor ADT from
		which the latest water level is to be retrieved.
	Return: the last water level (type: uint16_t)
*/
uint16_t waterLevelSensor_getLastWaterLevel(waterLevelSensor_t self);

