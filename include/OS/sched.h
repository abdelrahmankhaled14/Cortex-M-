#ifndef SCHED_H
#define SCHED_H
#include "std_types.h"

#define MAX 10
typedef struct 
{
    Runnable_Cbg_t callback;
    void *arg;
    uint32_t period_ms;
    uint8_t priority;
    uint8_t first_delay_ms;
} Runnable_t;


void Sched_Init(uint32_t tick_time);
status_t Sched_Start(void);
status_t Sched_RegisterRunnable(Runnable_t *runnable);
status_t Sched_execute(void);

#endif // SCHED_H