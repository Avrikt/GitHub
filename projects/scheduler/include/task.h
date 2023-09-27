#ifndef MY_PROJECT_TASK_H__
#define MY_PROJECT_TASK_H__

#include "uid.h"

typedef struct task task_t;
typedef int (*func_t)(void*);

/*
* TaskConstruct Description:
*	Creates a new task.
*
* @params:
*   op_func - pointer to function to store in task
*	params - pointer to parameters to store in task
*	exec_time - execution time of task
*	interval - frequency of execution
*
* @returns:
*	Pointer to struct that was created. If a failure occured while creating task - will return NULL
*
* @Complexity
*	O(1)
*/
task_t *TaskConstruct(int (*op_func)(void *), void *params, time_t exec_time , time_t interval);

/*
* TaskDestruct Description:
*	Releases task from memory
*
* @params:
*   task - The task to to release
*
* @returns:
*	None
*
* @Complexity
*	O(1)
*/
void TaskDestruct(task_t *task);

/*
* TaskGetExecTime Description:
*	Function to get the time stamp in a task
*
* @params:
*   task - The task to retrieve the time stamp from
*
* @returns:
*	The time stamp from given task
*
* @Complexity
*	O(1)
*/
time_t TaskGetExecTime(const task_t *task);

/*
* TaskGetInterval Description:
*	Function to gets the interval of execution of given task
*
* @params:
*   task - The task to retrieve the interval from
*
* @returns:
*	The frequency from given task
*
* @Complexity
*	O(1)
*/
time_t TaskGetInterval(const task_t *task);

/*
* TaskGetUID Description:
*	Function to retrieve the UID stored in the task
*
* @params:
*   task - The task to retrieve the UID from
*
* @returns:
*	UID of the given task
*
* @Complexity
*	O(1)
*/
m_uid_t TaskGetUID(const task_t *task);

/*
* TaskSetTime Description:
*	Function to set time of execution for task
*
* @params:
*   task - The task to set execution time
*	time - time to execute the task
*
* @returns:
*	None
*
* @Complexity
*	O(1)
*/
void TaskSetExecTime(task_t *task, time_t time);

/*
* TaskExecute Description:
*	Executes function in given task
*
* @params:
*   task - The task to execute
*
* @returns:
*	0 in succes and non-zero value if fail.
*
* @Complexity
*	O(1)
*/
int TaskExecute(task_t *task);

#endif
