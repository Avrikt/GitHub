/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Yotam Sasson   				*
* Date: 15.08.2023							*
* Description: Test file for Thread Pool	*
* Version: 2.1                              *
* Status: Approved                          *
*********************************************/

/************************************LIBRARIES**************************************************/

#include <thread>

#include "thread_pool.hpp"
#include "utilities.hpp"

const int THREADS = 5;
const int MAX_THREADS = 7;
const int MIN_THREADS = 1;


void TestThreadPool(void);

void TestFutureFunc(void);

using namespace dev;

class Task1: public ITask
{
public:
	void Execute() {std::cout << "Task 1" << std::endl; }
};

class Task2: public ITask
{
public:
	void Execute() {std::cout << "Task 2" << std::endl; }
};

class Task3: public ITask
{
public:
	void Execute() {std::cout << "Task 3" << std::endl; }
};

class TimedFunc : public ITask
{
public:
	void Execute() {sleep(3);}
};

/************************************ Future Functions *******************************************/
char CharFunc()
{
	return 'A';
}

int IntFunc()
{
	return 42;
}

std::string StringFunc()
{
	return std::string("Future Object");
}

/************************************ Main Function *******************************************/
int main(void)
{
	TestThreadPool();

	TestFutureFunc();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

void TestThreadPool(void)
{
	HEADER(Test Thread Pool);
	ThreadPool tp(THREADS, 0, SCHED_IDLE);
	auto task1 = std::make_shared<Task1>();
	auto task2 = std::make_shared<Task2>();
	auto task3 = std::make_shared<Task3>();
	auto task4 = std::make_shared<TimedFunc>();

	tp.AddTask(task3);
	tp.AddTask(task2, ThreadPool::LOW);
	tp.AddTask(task3, ThreadPool::LOW);
	tp.AddTask(task2, ThreadPool::MEDIUM);
	tp.AddTask(task2, ThreadPool::MEDIUM);
	sleep(2);
	std::cout << "Pause\n";
	tp.Pause();
	tp.SetNumThreads(MAX_THREADS);
	tp.AddTask(task1, ThreadPool::HIGH);
	tp.AddTask(task1, ThreadPool::HIGH);
	tp.AddTask(task3, ThreadPool::HIGH);
	tp.AddTask(task1, ThreadPool::LOW);
	tp.AddTask(task2, ThreadPool::MEDIUM);
	sleep(2);
	std::cout << "Resume\n";
	tp.Resume();

	tp.SetNumThreads(MIN_THREADS);
	tp.AddTask(task1, ThreadPool::HIGH);
	tp.AddTask(task1, ThreadPool::HIGH);
	tp.AddTask(task4, ThreadPool::HIGH);
	tp.AddTask(task3, ThreadPool::HIGH);
	tp.AddTask(task1, ThreadPool::LOW);
	tp.AddTask(task2, ThreadPool::MEDIUM);

}

void TestFutureFunc(void)
{
	HEADER(FutureTask);
	ThreadPool tp(THREADS);

	int test1 = 42;
	char test2 = 'A';
	std::string test3("Future Object");

	auto task1 = std::make_shared<FutureTask<int>>(IntFunc);
	auto task2 = std::make_shared<FutureTask<char>>(CharFunc);
	auto task3 = std::make_shared<FutureTask<std::string>>(StringFunc);
	
	tp.AddTask(task1);
	tp.AddTask(task2);
	tp.AddTask(task3);

	TEST(test1 == task1->GetValue(), Int Function);
	TEST(test2 == task2->GetValue(), Char Function);
	TEST(test3 == task3->GetValue(), String Function);
}