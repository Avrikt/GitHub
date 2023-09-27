/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 01.03.23							*
* Description: Source file for Task  		*
*********************************************/

#include <stdlib.h> /*malloc, freee*/
#include <assert.h> /*assert*/

#include "task.h"

struct task
{
	m_uid_t uid;
	func_t func_ptr;
	void *param;
	time_t exec_time;
	time_t interval;
};

task_t *TaskConstruct(int (*op_func)(void *), void *params, time_t exec_time, time_t interval)
{
    task_t *new_task = (task_t *) malloc(sizeof(task_t));

    assert(NULL != op_func);

    if (NULL == new_task)
    {
        return NULL;
    }

	new_task->uid = UIDCreate();

	if(UIDIsSame(new_task->uid, UIDGetBad()))
	{
		free(new_task);
		return NULL;
	}

	new_task->func_ptr = op_func;
    new_task->exec_time = exec_time;
    new_task->interval = interval;
    new_task->param = params;

	return new_task;
}

void TaskDestruct(task_t *task)
{
	free(task);
}

m_uid_t TaskGetUID(const task_t *task)
{
	assert (NULL != task);

	return task->uid;
}

time_t TaskGetExecTime(const task_t *task)
{
    assert (NULL != task);

    return task->exec_time;
}

time_t TaskGetInterval(const task_t *task)
{
	assert(NULL != task);

	return task->interval;
}

void TaskSetExecTime(task_t *task, time_t time)
{
    assert (NULL != task);

    task->exec_time = time;
}

int TaskExecute(task_t *task)
{
    assert (NULL != task);
	assert (NULL != task->func_ptr);

    return task->func_ptr(task->param);
}
