#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>

#include "cmsis_os.h"

#include "AbstractMain.h"

struct Node
{
	char * buf;			//!< Buffer itself
	uint32_t bufNum; 	//!< Sequential buffer number
	size_t bufSize;		//!< Size of allocated buffer
	uint32_t bufIdx; 	//prefer indexes to pointer movement

	struct Node * next;
	struct Node * prev;
};



/*--- Function prototypes ---*/
struct Node * initList(size_t bufSize);
struct Node * addNode(struct Node * head);
void checkProtection(struct Node * head);

#ifdef __cplusplus
}
#endif
