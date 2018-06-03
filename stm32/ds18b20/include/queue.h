#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>

#define QUEUE_SIZE 256

typedef struct {
	volatile uint8_t data[QUEUE_SIZE];
	volatile uint32_t head;
	volatile uint32_t tail;
} Queue;

void queueInit(Queue * q);
void queueClear(Queue * q);
uint8_t queueIsEmpty(Queue * q);
uint8_t queueIsFull(Queue * q);
uint8_t queueAdd(Queue * q, uint8_t c);
uint8_t queueDelete(Queue * q);

#endif
