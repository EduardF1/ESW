#pragma once

#include <stdint.h>

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