#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cmsis_os.h"

struct Node
{
	uint8_t * buf;		//!< Buffer itself
	uint32_t bufNum; 	//!< Sequential buffer number
	size_t bufSize;		//!< Size of allocated buffer
	uint32_t bufIdx; 	//prefer indexes to pointer movement

	struct Node * next;
	struct Node * prev;
};

#define MEM_PROT_SYMBOL		0xAA

struct Node * initList(size_t bufSize);
struct Node * addNode(struct Node * head);
void checkProtection(struct Node * head);
