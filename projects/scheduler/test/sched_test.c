/********************************************
* Developer: Avri Kehat						*
* Reviewed by:	Adir Ortal					*
* Date: 05.03.23							*
* Description: Test file for scheduler		*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/

#include "sched.h"
#include "utilities.h"

/************************************PROTOTYPES**************************************************/

static void TestScheduler(void);
static int TestSchRemoveTask(sch_t *scheduler, m_uid_t uid);
static int TestSchClear(sch_t *scheduler);
static int PrintMsg(void* msg);
static int FailFunc(void* dummy);
static int StopFunc(void* scheduler);

int main(void)
{
	TestScheduler();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestScheduler(void)
{
	sch_t *new_scheduler = SchCreate();
	char *msg1 = "Testing the scheduler with fail function";
	char *msg2 = "Hope this works";
	char *msg3 = "This should print every 3 seconds";
	char *msg4 = "Testing the scheduler with SchStop";
	m_uid_t to_remove = {0};

	TEST(1 == SchIsEmpty(new_scheduler), SchIsEmpty);
	TEST(0 == SchSize(new_scheduler), SchSize);

	to_remove = SchAddTask(new_scheduler, PrintMsg, msg1, time(NULL) + 2, 0);
	TEST(1 != UIDIsSame(to_remove, UIDGetBad()), SchAddTask);

	SchAddTask(new_scheduler, PrintMsg, msg2, time(NULL) + 3, 0);
	to_remove = SchAddTask(new_scheduler, PrintMsg, msg3, time(NULL) + 5, 3);

	TEST(3 == SchSize(new_scheduler), SchSize);

	SchAddTask(new_scheduler, FailFunc, msg3, time(NULL) + 9, 0);

	TEST(0 != SchRun(new_scheduler), SchRun);
	SchClear(new_scheduler);

	SchAddTask(new_scheduler, StopFunc, new_scheduler, time(NULL) + 4, 0);
	to_remove = SchAddTask(new_scheduler, PrintMsg, msg3, time(NULL) + 6, 0);
	SchAddTask(new_scheduler, PrintMsg, msg4, time(NULL) + 2, 0);
	SchAddTask(new_scheduler, FailFunc, msg3, time(NULL) + 15, 0);
	
	TEST(0 == SchRun(new_scheduler), SchRun);

	TEST(0 == TestSchRemoveTask(new_scheduler, to_remove), SchRemoveTask);

	TEST(0 == TestSchClear(new_scheduler), SchClear);

	SchDestroy(new_scheduler);
}

static int TestSchRemoveTask(sch_t *scheduler, m_uid_t uid)
{
	size_t size = SchSize(scheduler);

	SchRemoveTask(scheduler, uid);

	return !(size - 1 == SchSize(scheduler));
}

static int TestSchClear(sch_t *scheduler)
{
	SchClear(scheduler);
	return !(1 == SchIsEmpty(scheduler));
}
/************************************USER FUNCTIONS*************************************************/

static int PrintMsg(void *msg)
{
	printf("%s\n",(char *)msg);

	return 0;
}

static int FailFunc(void* dummy)
{
	(void)dummy;

	return 1;
}

static int StopFunc(void* scheduler)
{
	SchStop((sch_t *)scheduler);

	return 0;
}
