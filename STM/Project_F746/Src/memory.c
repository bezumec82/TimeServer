#include "memory.h"

struct Node * initList(size_t bufSize)
{
	struct Node * head = NULL;
	head = pvPortMalloc(sizeof(struct Node));
	if(!head)
	{
		printf("Out of memory.\r\n");
		return head;
	}

	//Initialize fields
	head->buf = pvPortMalloc(bufSize + 1/* protection byte */);
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

	struct Node * new = pvPortMalloc(sizeof(struct Node));
	if(!new)
	{
		printf("Out of memory.\r\n");
		return new;
	}

	new->buf = pvPortMalloc(head->bufSize + 1);
	if(!head->buf)
	{
		printf("Out of memory.\r\n");
		free(new);
		new = NULL;
		return new;
	}
	new->bufNum = last->bufNum + 1;
	new->bufSize = head->bufSize;
	new->buf[new->bufSize] = MEM_PROT_SYMBOL;

	new->next = head;
	head->prev = new;
	new->prev = last;
	last->next = new;
	return new;
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
