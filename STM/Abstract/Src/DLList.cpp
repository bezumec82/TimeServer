#include "DLList.h"

struct Node * initList(size_t bufSize)
{
	struct Node * head = NULL;
	head = (struct Node *)MALLOC(sizeof(struct Node));
	if(!head)
	{
		printf("Out of memory.\r\n");
		return head;
	}

	//Initialize fields
	head->buf = (char *)MALLOC(bufSize + 1/* protection byte */);
	if(!head->buf)
	{
		printf("Out of memory.\r\n");
		free(head);
		head = NULL;
		return head;
	}
	head->bufNum = 0;
	head->bufSize = bufSize;
	head->buf[head->bufSize] = MEM_PROT_SYMBOL;

	head->next = head;
	head->prev = head;
	return head;
}

struct Node * addNode(struct Node * head)
{
	struct Node * last = head;

	//find last member
	for( ; last->next != head; last = last->next) {}

	struct Node * newNode = (struct Node *)MALLOC(sizeof(struct Node));
	if(!newNode)
	{
		printf("Out of memory.\r\n");
		return newNode;
	}

	newNode->buf = (char *)MALLOC(head->bufSize + 1);
	if(!head->buf)
	{
		printf("Out of memory.\r\n");
		free(newNode);
		newNode = NULL;
		return newNode;
	}
	newNode->bufNum = last->bufNum + 1;
	newNode->bufSize = head->bufSize;
	newNode->buf[newNode->bufSize] = MEM_PROT_SYMBOL;

	newNode->next = head;
	head->prev = newNode;
	newNode->prev = last;
	last->next = newNode;
	return newNode;
}

void checkProtection(struct Node * head)
{
	struct Node * curr = head;
	for( ; curr->next != head; curr = curr->next)
	{
		assert( (curr->buf[curr->bufSize] == MEM_PROT_SYMBOL) && \
				"Buffer overflow");
	}
}
