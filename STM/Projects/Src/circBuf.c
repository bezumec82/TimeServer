#include "circBuf.h"

int32_t initCircBuf(struct CircBuf * cBuf, size_t size)
{
#if DYNAMIC_BUF
	if(size == 0) return -1;
	cBuf->buf = pvPortMalloc(size + 1); //One byte for the protection symbol
	if(cBuf->buf == NULL)
	{
		printf("Can't allocate space for circular buffer.\r\n");
		return -1;
	}
	cBuf->size = size;
#elif STATIC_BUF
	cBuf->size = CIRC_BUF_SIZE;
#else
# error
#endif
	cBuf->buf[cBuf->size] = 0xAA; //protection symbol
	cBuf->head = &cBuf->buf[0];
	cBuf->tail = &cBuf->buf[0];

	cBuf->lock = osMutexNew(NULL);

	return 0;
}

/*!
 * @brief Copies data to the circular buffer.
 * @param cBuf Destination circular buffer.
 * @param data Source data.
 * @param size Target data size.
 * @retval Head position
 */
uint8_t * circBufPut(struct CircBuf * cBuf, uint8_t * data, size_t size)
{
	if(size >= cBuf->size)
	{
		printf("Data to put bigger than buffer.\r\n");
		return NULL; //no op
	}

	// Simple copy
	if(cBuf->head + size < &cBuf->buf[cBuf->size])
	{
		memcpy(cBuf->head, data, size);
		cBuf->head += size;
	}
	// Simple copy with head carry out
	else if(cBuf->head + size == &cBuf->buf[cBuf->size])
	{
		memcpy(cBuf->head, data, size);
		cBuf->head = &cBuf->buf[0];
	}
	// Copy in two steps
	else if(cBuf->head + size > &cBuf->buf[cBuf->size])
	{
		//Copy data until the end of buffer
		uint32_t first_part = &cBuf->buf[cBuf->size] - cBuf->head;
		memcpy(cBuf->head, data, first_part);

		//Copy data from the start of buffer
		uint32_t second_part = size - first_part;
		memcpy(&cBuf->buf[0], data + first_part, second_part);
		cBuf->head = &cBuf->buf[second_part];
	}
	assert((cBuf->buf[cBuf->size] == 0xAA) && \
			"Circ buf overflow");
	return cBuf->head;
}

/*!
 * @brief Shows how much data available in the
 * circular buffer.
 * @note Uses lock inside.
 */
uint32_t availableData(struct CircBuf * cBuf)
{
	osMutexAcquire(cBuf->lock, osWaitForever);
	uint32_t retVal = 0;
	if(cBuf->tail < cBuf->head)
	{
		retVal = cBuf->head - cBuf->tail;
	}
	if(cBuf->tail > cBuf->head)
	{
		retVal = cBuf->size - (cBuf->tail - cBuf->head);
	}
	osMutexRelease(cBuf->lock);
	return retVal;
}

/*!
 * @brief Copies data from the circular buffer.
 * @param cBuf Source circular buffer.
 * @param data Destination data.
 * @param size Source data size (copy request).
 * @retval Tail position
 */
uint8_t * circBufGet(struct CircBuf * cBuf, uint8_t * data, size_t size)
{
	if( (!data)||(!size) )
	{
		printf("Wrong arguments.\r\n");
		return NULL;
	}
	//TODO : Repeated check possible to optimize
	if(size > availableData(cBuf))
	{
		printf("Data to get bigger than available.\r\n");
		return NULL;
	}

	// Simple copy
	if(cBuf->tail + size < &cBuf->buf[cBuf->size])
	{
		memcpy(data, cBuf->tail, size);
		cBuf->tail += size;
	}

	// Simple copy with tail carry out
	else if(cBuf->tail + size == &cBuf->buf[cBuf->size])
	{
		memcpy(data, cBuf->tail, size);
		cBuf->tail = &cBuf->buf[0];
	}

	// Copy in two steps
	else if(cBuf->tail + size > &cBuf->buf[cBuf->size])
	{
		//Copy data until the end of buffer
		uint32_t first_part = &cBuf->buf[cBuf->size] - cBuf->tail;
		memcpy(data, cBuf->tail, first_part);

		//Copy data to the start of buffer
		uint32_t second_part = size - first_part;
		memcpy(data + first_part, &cBuf->buf[0], second_part);
		cBuf->tail = &cBuf->buf[second_part];
	}
	return cBuf->tail;
}
