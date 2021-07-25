#pragma once

#include "configuration.h"

//	Pointer to a waterTemperatureSensor ADT object (declaration)
typedef struct waterTemperatureSensor* waterTemperatureSensor_t;

/*
	Function to create the water temperature sensor ADT externally
	Parameters:
	1) SemaphoreHandle_t mutexSemaphore - mutex required for standard I/O functions (i.e. printf) as they are not
	   re-entrant (they block)
	Return: an water temperature sensor object (type: waterTemperatureSensor_t)
*/
waterTemperatureSensor_t waterTemperatureSensor_create(SemaphoreHandle_t mutexSemaphore);

/*
	Function to retrieve the last water level from the water temperature sensor
	Parameters:
	1)	waterTemperatureSensor_t self - reference to the water temperature sensor ADT from
		which the latest water temperature is to be retrieved.
	Return: the last water temperature (type: int8_t)
*/
int8_t waterTemperatureSensor_getLastWaterTemperature(waterTemperatureSensor_t self);