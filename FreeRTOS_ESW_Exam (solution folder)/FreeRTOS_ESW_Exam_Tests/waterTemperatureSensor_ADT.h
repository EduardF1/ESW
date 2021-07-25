
#pragma once
#include <stdint.h>

///
///		Interface used for testing - FreeRTOS objects excluded
/// 

//	Pointer to a waterTemperatureSensor ADT object (declaration)
typedef struct waterTemperatureSensor* waterTemperatureSensor_t;

/*
	Function to create the water temperature sensor ADT externally
	Return: an water temperature sensor object (type: waterTemperatureSensor_t)
*/
waterTemperatureSensor_t waterTemperatureSensor_create();

/*
	Function to retrieve the last water level from the water temperature sensor
	Parameters:
	1)	waterTemperatureSensor_t self - reference to the water temperature sensor ADT from
		which the latest water temperature is to be retrieved.
	Return: the last water temperature (type: int8_t)
*/
int8_t waterTemperatureSensor_getLastWaterTemperature(waterTemperatureSensor_t self);

