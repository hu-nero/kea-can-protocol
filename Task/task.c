/*
 * task_init.c
 *
 *  Created on: 2023年8月28日
 *      Author: xinlei.hu
 */
#include "task.h"
#include "Led/task_led.h"
#include "Can_Protocol/task_can_protocol.h"
#include "Timer/hal_timer.h"
#include "PE_Types.h"



static BOARD_TASK_Struct gTaskMain[MAX_TASKS_SIZE] = {};
static uint8_t gTaskCurId = 0;

static void task_timer_callback(void);

void
task_init(void)
{
	//task tick timer callback func set
    hal_timer0_callback_set(task_timer_callback);
    //timer init
    hal_timer_init(HAL_DEV_TIM0);
    //task init
    led_task_init();
    can_protocol_task_init();
    //task add
    task_add(led_task, 50, 500, TASK_MAX_TIMES);
    task_add(can_protocol_task, 10, 10, TASK_MAX_TIMES);
}

void
task_run(void)
{
    uint8_t u8TaskId;

    // Go through the task array
    for (u8TaskId = 0; u8TaskId < MAX_TASKS_SIZE; u8TaskId++)
    {
        // Check if there is a task at this location
        if (gTaskMain[u8TaskId].pTask != NULL)
        {
            //执行任务
            if (gTaskMain[u8TaskId].taskTick == 0)
            {
                gTaskCurId = u8TaskId;
                // All tasks are periodic: schedule task to run again
                gTaskMain[u8TaskId].taskTick = gTaskMain[u8TaskId].taskPeriod;
                (*gTaskMain[u8TaskId].pTask)(); // Run the task


                //更新执行次数
                if(gTaskMain[u8TaskId].taskTimes != TASK_MAX_TIMES) //任务定时器
                    gTaskMain[u8TaskId].taskTimes--;
                //删除任务
                if (gTaskMain[u8TaskId].taskTimes == 0)
                {
                    task_delete(gTaskMain[u8TaskId].pTask);
                }
            }
        }
    }
}

void
task_add(uint16_t (*PTask)(),
        const uint32_t Tick,
        const uint32_t Period,
        const uint32_t Times)
{
     uint32_t u32TaskId = 0;

   // Check pre-conditions (START)
   // First find a gap in the array (if there is one)
   while ((gTaskMain[u32TaskId].pTask != NULL) && (u32TaskId < MAX_TASKS_SIZE))
   {
      u32TaskId++;
   }

   // Have we reached the end of the list?
   if ((u32TaskId < MAX_TASKS_SIZE) || (Period > 0))
   {
      // If we're here, there is a space in the task array
      // and the task to be added is periodic
      gTaskMain[u32TaskId].pTask = PTask;
      gTaskMain[u32TaskId].taskTick = Tick + 1;
      gTaskMain[u32TaskId].taskPeriod = Period;
      gTaskMain[u32TaskId].taskTimes = Times;
   }
}

void
task_delete(uint16_t (*PTask)())
{

   uint32_t u32IdCounter;
   for (u32IdCounter = 0; u32IdCounter < MAX_TASKS_SIZE;)
   {
      if (gTaskMain[u32IdCounter].pTask != PTask)
         u32IdCounter++;

      else
      {
         __DI();

         gTaskMain[u32IdCounter].pTask = NULL;

         __EI();
         u32IdCounter = MAX_TASKS_SIZE + 1;
      }
   }
}

//任务运行过程中切换为其他任务运行。
//则当前任务返回后不再运行。
//为了安全应该关中断操作。
// 可以在task中增加一个参数，task运行到一定次数切换到其他的task;
//或者 事件触发 退出当前task,执行新的task
void
task_change(uint16_t (*PTask)(),
           const uint32_t Tick,
           const uint32_t Period)
{

   __DI();

   if ((gTaskCurId < MAX_TASKS_SIZE) || (Period > 0))
   {
      gTaskMain[gTaskCurId].pTask = PTask;
      gTaskMain[gTaskCurId].taskTick = Tick + 1;
      gTaskMain[gTaskCurId].taskPeriod = Period;
   }
   __EI();
}

void
task_timer_callback(void)
{
    uint8_t i;
    for(i = 0 ; i < MAX_TASKS_SIZE ; i ++)
    {
        if(gTaskMain[i].taskTick) //任务定时器
            gTaskMain[i].taskTick--;
    }
}


