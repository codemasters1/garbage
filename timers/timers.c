#include "timers.h"

#define DEBUG 0

#define _dbg if (DEBUG)

void clearTimers(void)
{
    int i;

    for (i = 0; i < TIMERS_COUNT; i++)
    {
        timers[i] = NULL;
    }
}

int setTimer(int interval, _timer_cb callback)
{
    int i;

    for (i = 0; i < TIMERS_COUNT; i++)
    {
        if (timers[i] == NULL) // znalezione niezaalokowane miejsce w tablicy timerów
        {
            _dbg printf("Setting timer at index %d with values: callback_address=%x interval=%d\n",i, (int)callback, interval);

            _timer_t *timer = malloc(sizeof(_timer_t));

            timer->callback     = callback;
            timer->interval     = interval;
            timer->left         = interval - 1;

            timers[i] = timer;

            return i;
        }
    }

    printf("Error: Not enough space to allocate timer!\n");

    return -1;
}
#if DEBUG

#warning In debug Mode
void listTimers(void)
{
    int i;

    for (i = 0; i < TIMERS_COUNT; i++)
    {
        if (timers[i] != NULL)
        {
            printf("timer=%d (%x), left=%d, interval=%d\n", i, (unsigned int)timers[i], timers[i]->left, timers[i]->interval);
        }
    }
}

#endif

void clearTimer(int id)
{
    free(timers[id]);
    timers[id] = NULL;

    _dbg printf("Cleared timer with index %d\n", id);
}

void runTimers(void)
{
    int i;

    for (i = 0; i < TIMERS_COUNT; i++) // iteracja po elementach tablcy
    {
        if (timers[i] == NULL) // jesli timer nie jest przypisany
        {
			continue;
		}
		
		_timer_t *timer = timers[i];
		if (timer->left == 0) // jeœli zostaje wykonany takt timera
		{
			timer->callback(); // wykonywanie eventu
			timer->left = timer->interval - 1; // czyszczenie licznika pozostalych taktów do eventu
		}
		else
		{
			timer->left--;
		}
    }
}
