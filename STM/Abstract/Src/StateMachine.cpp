#include "StateMachine.h"

StateMachine& StateMachine::getInstance()
{
	static StateMachine singleton;
	return singleton;
}

void StateMachine::post(const Message * message)
{
	osStatus_t postStatus = osMessageQueuePut(queue,
			static_cast<const void*>(message),
				0/* Message priority is ignored */,
				0/* Don't block if queue is full */);
	if(postStatus != osOK)
	{
		debug("Can't post message.\r\n");
	}
}

void StateMachine::initialize()
{
	queue = osMessageQueueNew(QUEUE_LENGTH,
		sizeof(Message *), &queAttr );
	thrdId = osThreadNew(
			&StateMachine::threadFunc, this, &thrdAttr);
}

void StateMachine::threadFunc(void * argument)
{
	if(argument == nullptr)
	{
		printf("Null passed as the parameter.\r\n");
		HAL_NVIC_SystemReset();
	}
	StateMachine * instance = static_cast<StateMachine*>(argument);

	for(;;) /* Infinite loop */
	{
		struct Message * message = nullptr;
			osMessageQueueGet(instance->queue, message,
				0/* Message priority is ignored */,
					osWaitForever);
		/* Process message here */
#if DEBUG_QUEUE //Output to the UART
				extern UART_HandleTypeDef * debugUart;
				extern bool debugUart_isSending;
				if(!debugUart_isSending) //unblock from IRQ
				{
					debugUart_isSending = true;
					HAL_UART_Transmit_DMA(debugUart,
							(uint8_t*)&message->message[0],
								message->msgSize);
				}
#endif
		/* End message processing */
		FREE(message); //deallocate at receiver side

	} //end for
	//never achieved here
	HAL_NVIC_SystemReset();
}
