#include "include/queue.h"

void queueInit(Queue * q)
{
	q->head = q->tail = 0;
}

uint8_t queueIsEmpty(Queue * q)
{
	return (q->head == q->tail);
}

uint8_t queueIsFull(Queue * q)
{
	return ((q->tail - QUEUE_SIZE) == q->head);
}

uint8_t queueAdd(Queue * q, uint8_t c)
{
	if (queueIsFull(q))
		return 0;

	q->tail++;
	q->data[q->tail % QUEUE_SIZE] = c;

	return 1;
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
