#include "mcal/systic/systick.h"
#include "OS/sched.h"
#include "macros.h"
#include "hal/switch/switch.h"
#include "hal/led/led.h"

static Runnable_t* SavedRunnable[MAX];
static signed int volatile Sched_trigger = 0;
uint64_t volatile tick = 0;
static void Sched_exec(void);

static void Sched_exec(void)
{
    Sched_trigger = 1;
    tick++;
}

void Sched_Init(uint32_t tick_time)
{
    SysTick_Init(tick_time);
    SysTick_SetCallBack(Sched_exec);
}






status_t Sched_Start(void)
{
    SysTick_Start();
    for(;;)
    {

        if(Sched_trigger == 1)
        {
            Sched_trigger = 0;
            for (int i = 0; i < MAX; i++)
            {
                if (NULL != SavedRunnable[i])
                {
                    if (NULL != SavedRunnable[i]->callback)
                    {
                        if (tick % SavedRunnable[i]->period_ms == 0)
                        {
                            SavedRunnable[i]->callback(SavedRunnable[i]->arg);
                        }
                    }
                }
                else
                {
                }
            }
        }

    }
}
status_t Sched_RegisterRunnable(Runnable_t *runnable)
{
    status_t ret = STATUS_OK;
    if(runnable  == NULL)
    {
        ret = STATUS_ERROR;
    }
    else if(runnable->callback == NULL)
    {
        ret = STATUS_ERROR;
    }
    else if(SavedRunnable[runnable->priority] == NULL)
    {
        SavedRunnable[runnable->priority] = runnable;
    }
    return ret;
}
