/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 01.03.23							*
* Description: Test file for UID			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/

#include "uid.h"

/************************************TEST MACRO*************************************************/
#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-20s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-20s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) ((0) == (x)) ?\
printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)

/************************************PROTOTYPES**************************************************/

void TestUID(void);
void PrintUid(m_uid_t uid);

int main(void)
{
	TestUID();
	return 0;
}

/****************************************TEST FUNCTIONS*****************************************/

void TestUID(void)
{
	m_uid_t new_uid = UIDCreate();
	m_uid_t second_uid = UIDGetBad();

	TEST(0 == UIDIsSame(new_uid, second_uid), UIDIsSame);
	TEST(1 == UIDIsSame(new_uid, new_uid), UIDIsSame);

	PrintUid(new_uid);
	PrintUid(second_uid);
}

void PrintUid(m_uid_t uid)
{
	printf("\ntime stamp: %ld\n", uid.time_stamp);
	printf("count: %ld\n", uid.counter);
	printf("PID: %d\n", uid.pid);
	printf("IP: %s\n", uid.ip);
}
