/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 05.03.23							*
* Description: Source file for scheduler	*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <unistd.h> /*sleep*/

#include "sched.h"
#include "task.h"
#include "pq.h"
#include "utilities.h"

/***************************PROTOTYPES AND GLOBAL VARIABLES**************************************/

static int CmpExecTime(const void *task1, const void *task2);
static int IsSameTask(const void *task, const void *uid);
static unsigned int IsTimeToExecute(time_t time_to);

typedef enum run_status{STOP = 0, RUN = 1} run_status_t;

struct scheduler
{
    pq_t *task_queue;
    run_status_t mode;

};

/************************************Functions***************************************************/

sch_t *SchCreate(void)
{
    sch_t *new_scheduler = (sch_t *)malloc(sizeof(sch_t));

    if (NULL == new_scheduler)
    {
        return NULL;
    }

    new_scheduler->task_queue = PQCreate(CmpExecTime);

    if (NULL == new_scheduler->task_queue)
    {
        free(new_scheduler);
        return NULL;
    }

    new_scheduler->mode = STOP;

    return new_scheduler;
}

void SchDestroy(sch_t *scheduler)
{
    assert(NULL != scheduler);

    SchClear(scheduler);
    PQDestroy(scheduler->task_queue);
    free(scheduler);
}

m_uid_t SchAddTask(sch_t *scheduler ,int (*op_func)(void *), void *params, time_t exec_time , time_t interval)
{
    task_t *new_task = NULL;

    assert(NULL != scheduler);
    assert(NULL != op_func);

    new_task = TaskConstruct(op_func, params, exec_time, interval);
    if (NULL == new_task)
    {
        return UIDGetBad();
    }

    if (SUCCESS != PQEnqueue(scheduler->task_queue, new_task))
    {
        TaskDestruct(new_task);
        return UIDGetBad();
    }

    return TaskGetUID(new_task);
}

void SchRemoveTask(sch_t *scheduler ,m_uid_t task_uid)
{
    task_t *to_remove = NULL;
    assert(NULL != scheduler);
    assert(NULL != scheduler->task_queue);
    assert(!UIDIsSame(task_uid, UIDGetBad()));

    to_remove = (task_t *) PQErase(scheduler->task_queue, IsSameTask, &task_uid);
    TaskDestruct(to_remove);
}

int SchIsEmpty(const sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->task_queue);

    return PQIsEmpty(scheduler->task_queue);
}

size_t SchSize(const sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->task_queue);

    return PQSize(scheduler->task_queue);
}

int SchRun(sch_t *scheduler)
{
    task_t *to_exec = NULL;
    time_t interval = 0;
    time_t exec_time = 0;
    int run_stat = 0;

    assert(NULL != scheduler);
    assert (NULL != scheduler->task_queue);

    scheduler->mode = RUN;
    while (!SchIsEmpty(scheduler) && RUN == scheduler->mode && 0 == run_stat)
    {
        to_exec = (task_t *)PQPeek(scheduler->task_queue);
        exec_time = TaskGetExecTime(to_exec);
        interval = TaskGetInterval(to_exec);
        PQDequeue(scheduler->task_queue);

        sleep(IsTimeToExecute(exec_time));

        run_stat = TaskExecute(to_exec);
        if ((0 == run_stat) && (0 < interval))
        {
            TaskSetExecTime(to_exec, exec_time + interval);
            if (0 != PQEnqueue(scheduler->task_queue, (void *) to_exec))
            {
                TaskDestruct(to_exec);
                scheduler->mode = STOP;
            };
        }
        else
        {
            TaskDestruct(to_exec);
        }
    }
    scheduler->mode = STOP;

    return run_stat;
}

void SchStop(sch_t *scheduler)
{
    assert(NULL != scheduler);

    scheduler->mode = STOP;
}

void SchClear(sch_t *scheduler)
{
    assert(NULL != scheduler);
    assert(NULL != scheduler->task_queue);

    while (!SchIsEmpty(scheduler))
    {
        TaskDestruct((task_t *)PQPeek(scheduler->task_queue));
        PQDequeue(scheduler->task_queue);
    }
}

static int CmpExecTime(const void *task1, const void *task2)
{
    return (int)difftime(TaskGetExecTime(task2), TaskGetExecTime(task1));
}

static int IsSameTask(const void *task, const void *uid)
{
	return UIDIsSame(TaskGetUID((task_t *)task), *(m_uid_t *)uid);
}

static unsigned int IsTimeToExecute(time_t time_to)
{
    int time_to_execute = difftime(time_to, time(NULL));

    if (0 > time_to_execute)
    {
        time_to_execute = 0;
    }

    return (unsigned int)time_to_execute;
}
