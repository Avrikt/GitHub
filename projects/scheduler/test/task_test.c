/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 01.03.23							*
* Description: Test file for Task			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*system*/

#include "task.h"

/************************************TEST MACRO*************************************************/
#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-20s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-20s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) ((0) == (x)) ?\
printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)

/************************************PROTOTYPES**************************************************/

static void TestTask(void);

static int UTubeFunc(void *link);
static int PrintFunc(void *msg);

int main(void)
{
	TestTask();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestTask(void)
{
	char *link = "https://www.youtube.com/watch?v=HlBYdiXdUa8";
	char *msg = "Enjoy a small laugh";
	task_t *new_task = {0};
	task_t *task2 = {0};
	time_t exec_time = 12;
	time_t interval = 30;

	new_task = TaskConstruct(UTubeFunc, link, exec_time, 0);
	task2 = TaskConstruct(PrintFunc, msg, exec_time, interval);

	TEST(0 == TaskExecute(new_task), TaskExecute);
	TEST(0 == TaskExecute(task2), TaskExecute);

	TEST(exec_time == TaskGetExecTime(new_task), TaskGetExecTime);
	TEST(interval == TaskGetInterval(task2), TaskGetInterval);

	TaskDestruct(new_task);
	TaskDestruct(task2);
}

/************************************USER FUNCTIONS**********************************************/

static int UTubeFunc(void *link)
{
    char command[1024];
	printf("Opening youtube\n");
    sprintf(command, "xdg-open %s > /dev/null 2>&1 &", (char *)link);
    if (-1 == system(command))
	{
		return 1;
	}

	return 0;
}

static int PrintFunc(void *msg)
{
	printf("%s\n", (char *)msg);

	return 0;
}
