/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Dave Hairapetian					*
* Date: 13.09.2023								*
* Description: Test file for Scheduler			*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "scheduler.hpp"
#include "utilities.hpp"

using namespace dev;

class DragonTask : public Scheduler::ITask
{
public:
	void Execute()
	{
		int result = system("cowsay -f dragon FEAR ME!!!!");
		if(result != 0)
		{
			std::cerr << "system() failed";
		}
	}
};

class TurtleTask : public Scheduler::ITask
{
public:
	void Execute()
	{
		int result = system("cowsay -f turtle Go 132SeaTurtles!!!!");
		if(result != 0)
		{
			std::cerr << "system() failed";
		}
	}
};

class GenericTask : public Scheduler::ITask
{
public:
	void Execute()
	{
		int result = system("cowsay -f elephant yay");
		if(result != 0)
		{
			std::cerr << "system() failed";
		}

		Scheduler *sched = Singleton<Scheduler>::GetInstance();

		sched->AddTask(std::make_shared<DragonTask>(), 0);
	}
};

static void TestSched(void);

int main(void)
{
	TestSched();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
static void TestSched(void)
{
	Scheduler *sched = Singleton<Scheduler>::GetInstance();

	HEADER(Scheduler Test);

	sched->AddTask(std::make_shared<GenericTask>(), 1);
	sched->AddTask(std::make_shared<DragonTask>(), 4);
	sched->AddTask(std::make_shared<TurtleTask>(), 2);
	sched->AddTask(std::make_shared<GenericTask>(), 0);

	sleep(7);
}