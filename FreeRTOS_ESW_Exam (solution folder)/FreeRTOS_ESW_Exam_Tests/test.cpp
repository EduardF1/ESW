#include "gtest/gtest.h"
#include "../fff-master/fff.h"
DEFINE_FFF_GLOBALS	// Initialize the FFF framework

//	External C-code files (necessary for testing)
extern "C"
{
#include "waterTemperatureSensor_ADT.h"
#include "payload.h"
}


/*
	Water temperature sensor ADT Test suite
*/

//	Fake function to create a Water temperature sensor ADT object
FAKE_VALUE_FUNC(waterTemperatureSensor_t, waterTemperatureSensor_create);

//	Fake function to retrieve the latest value of a measurement from a Water temperature sensor ADT object
FAKE_VALUE_FUNC(int8_t, waterTemperatureSensor_getLastWaterTemperature, waterTemperatureSensor_t);


class Water_temperature_sensor_ADT_Test : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(waterTemperatureSensor_create);
		RESET_FAKE(waterTemperatureSensor_getLastWaterTemperature);
		FFF_RESET_HISTORY();
	}

	void TearDown() override
	{

	}
};

TEST_F(Water_temperature_sensor_ADT_Test, Test_is_temperature_sensor_ADT_create_called)
{
	//	Arrange
	waterTemperatureSensor_t waterTemperatureSensor;
	//	Act
	waterTemperatureSensor = waterTemperatureSensor_create();
	//	Assert
	ASSERT_TRUE(waterTemperatureSensor_create_fake.call_count, 1);
	EXPECT_TRUE(waterTemperatureSensor == NULL);
}

TEST_F(Water_temperature_sensor_ADT_Test, Test_is_temperature_sensor_ADT_returning_a_value)
{
	//	Arrange
	int8_t value;
	waterTemperatureSensor_t waterTemperatureSensor;
	//	Act
	waterTemperatureSensor = waterTemperatureSensor_create();
	value = waterTemperatureSensor_getLastWaterTemperature(waterTemperatureSensor);
	//	Assert
	ASSERT_TRUE(waterTemperatureSensor_create_fake.call_count, 1);
	ASSERT_TRUE(waterTemperatureSensor_getLastWaterTemperature_fake.call_count, 1);
	EXPECT_TRUE(waterTemperatureSensor_getLastWaterTemperature_fake.return_val == 0);
	EXPECT_EQ(value, 0);
}


/*
	Message payload Test suite
*/

//	Fake function to send a payload message
FAKE_VOID_FUNC(sendMessage, messagePayload);

class Payload_Test : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(sendMessage);
		FFF_RESET_HISTORY();
	}

	void TearDown() override
	{

	}
};

TEST_F(Payload_Test, Test_create_payload_and_send)
{
	//	Arrange
	messagePayload messageToSend;
	messageToSend = { messageToSend.len = 4 };

	int8_t waterTemperature = 25;
	uint16_t waterLevel = 2000;
	//	Act
	messageToSend.bytes[0] = waterLevel >> 8;
	messageToSend.bytes[1] = waterLevel & 0xFF;
	messageToSend.bytes[2] = waterTemperature >> 8;
	messageToSend.bytes[3] = waterTemperature & 0xFF;



	sendMessage(messageToSend);
	//	Assert
	ASSERT_EQ(messageToSend.len, 4);
	for (int i = 0; i < messageToSend.len; i++)
	{
		if (i % 2 == 1) // bytes 2 - [1] and 4 - [3], low bytes
		{
			ASSERT_TRUE(messageToSend.bytes[1] == 0xD0);	// hex - low byte water level
			ASSERT_TRUE(messageToSend.bytes[1] == 208);	// decimal - low byte water level
			ASSERT_TRUE(messageToSend.bytes[3] == 0x19); // hex - low byte water temperature
			ASSERT_TRUE(messageToSend.bytes[3] == 25); // decimal - low byte water temperature 
		}

		if (i == 0)
		{
			ASSERT_TRUE(messageToSend.bytes[i] == 0x07); // hex - high byte water level
			ASSERT_TRUE(messageToSend.bytes[0] == 7); // decimal - high byte water level
		}

		if (i == 2)
		{
			ASSERT_TRUE(messageToSend.bytes[i] == 0x00); // hex - high byte water temperature
			ASSERT_TRUE(messageToSend.bytes[i] == 0);	// decimal - high byte water temperature
		}

	}
	EXPECT_TRUE(sendMessage_fake.call_count == 1);
}
