#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define TIMERS_COUNT 16

typedef void (*_timer_cb)(void);

typedef struct
{
    _timer_cb callback;
    int interval;
    int left;
} _timer_t;

_timer_t *timers[TIMERS_COUNT];

void clearTimers(void);
void runTimers(void);
void clearTimer(int id);
int setTimer(int interval, _timer_cb callback);

#endif // TIMERS_H_INCLUDED
