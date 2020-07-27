#pragma once

#define CIRC_BUF_SIZE	512

struct CircBuf
{
	uint8_t buf[CIRC_BUF_SIZE];
	uint8_t * head;
	uint8_t * tail;
};

#define INIT_CIRC_BUF(cBuf) \
	cBuf.head = &cBuf.buf[0]; 	\
	cBuf.tail = &cBuf.buf[0];	\
	memset(cBuf.buf, 0x0, CIRC_BUF_SIZE);
