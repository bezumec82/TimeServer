#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include "cmsis_os.h"

#include "AbstractMain.h"
#include "debug.h"

#define DEBUG_QUEUE		true

class StateMachine
{
public : /*--- Structs, classes, enums ---*/
	enum class SenderId : uint8_t
	{
		PC_UART,
		PC_I2C,
		PC_USB,
		GPS_UART,
		STATE_MACHINE
	};
	struct Message
	{
		char * message;
		uint32_t msgSize;
		enum SenderId senderId;
	};

private :
	StateMachine() = default;

public :
	~StateMachine() = default;

public :
	void initialize();
	static StateMachine& getInstance();
	void post(const Message * message);

private :
	static void threadFunc(void * argument);

private : /*--- Properties ---*/
	osMessageQueueId_t queue = nullptr;
	osMessageQueueAttr_t queAttr = {
		.name = "State machine queue"
	};
	osThreadId_t thrdId = nullptr;
	const osThreadAttr_t thrdAttr =
	{
		.name = "State machine thread"
	};
	/*--- Constants ---*/
	const UBaseType_t QUEUE_LENGTH = 16;
	const uint32_t POST_TIMEOUT_MS = 1;
};
