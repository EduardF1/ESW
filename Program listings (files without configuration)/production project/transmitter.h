#pragma once

#include "configuration.h"

/*	Message payload struct variable
*	Contained variables:
	1) len (uint8_t) = length of payload (size)
*	2) bytes[4] = byte array (type uint8_t)
*/
typedef struct messagePayload {
	uint8_t len;
	uint8_t bytes[4];
}messagePayload;

//	Function to send a payload object, parameters: payload - to be sent (type: messagePayload)
void sendMessage(messagePayload payload);

/*
	Function to create the transmitter externally
	Parameters:
	1) MessageBufferHandle_t payloadMessageBuffer - message buffer from which to receive messages (sent payload)
	2) SemaphoreHandle_t mutexSemaphore - mutex required for standard I/O functions (i.e. printf) as they are not
	   re-entrant (they block)
*/
void transmitter_create(MessageBufferHandle_t payloadMessageBuffer, SemaphoreHandle_t mutexSemaphore);