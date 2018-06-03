#include "Code130_Queue.h"

#define QUEUE_SIZE 256

typedef struct {
	uint8_t data[QUEUE_SIZE];
	uint32_t head;
	uint32_t tail;
} Queue;

void queueInit(Queue * q)
{
	q->head = q->tail = -1;
}

void queueClear(Queue * q)
{
	q->head = q->tail = -1;
}

uint8_t queueIsEmpty(Queue * q)
{
	return (q->head == q->tail);
}

uint8_t queueIsFull(Queue * q)
{
	return ((q->tail - QUEUE_SIZE) == q->head);
}

void queueAdd(Queue * q, uint8_t c)
{
	if (queueIsFull(q))
		return FALSE;

	q->tail++;
	q->data[q->tail % QUEUE_SIZE] = c;
	
	return TRUE;
}

uint8_t queueDelete(Queue * q)
{
	if (queueIsEmpty(q))
		return '\0';
	else
	{
		q->head++;
		return q->data[q->head % QUEUE_SIZE];
	}
}
