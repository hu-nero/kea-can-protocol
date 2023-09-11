/*
 * task_init.h
 *
 *  Created on: 2023Äê8ÔÂ28ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_TASK_H_
#define TASK_TASK_H_
#include <stdint.h>

#define MAX_TASKS_SIZE    10
#define TASK_MAX_TIMES    0xFFFFFFFFUL

typedef struct _task
{
    uint16_t      (*pTask) (void);
    uint32_t      taskTick;
    uint32_t      taskPeriod;
    uint32_t      taskTimes;
}BOARD_TASK_Struct;

extern void task_init(void);
extern void task_run(void);
extern void task_add(uint16_t (*PTask)(), const uint32_t Tick, const uint32_t Period, const uint32_t Times);
extern void task_delete(uint16_t (*PTask)());
extern void task_change(uint16_t (*PTask)(), const uint32_t Tick, const uint32_t Period);


#endif /* TASK_TASK_H_ */

