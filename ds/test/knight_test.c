/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Lihi Cohen					*
* Date: 20.04.2023							*
* Description: Test file for knight			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <time.h> /*clock()*/

#include "knight.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/


/********************************FORWARD DECLERATIONS*******************************************/

static void TestKnightsTour(void);

int main(void)
{
	TestKnightsTour();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestKnightsTour(void)
{
	int matrix[SIZE_ROW][SIZE_COL] = {0};
	clock_t start_time, end_time;

	HEADER(KightsTour);

	start_time = clock();
	TEST(SUCCESS == KnightTour(matrix, 0, 0), KnightTour 0:0);
	end_time = clock();

	printf("Total time taken by CPU: %f\n", (double)(end_time - start_time)/ CLOCKS_PER_SEC);
	PrintBoard(matrix);

	start_time = clock();
	TEST(SUCCESS == KnightTour(matrix, 6, 1), KnightTour 6:1);
	end_time = clock();

	printf("Total time taken by CPU: %f\n", (double)(end_time - start_time)/ CLOCKS_PER_SEC);
	PrintBoard(matrix);
	
	start_time = clock();
	TEST(SUCCESS == KnightTour(matrix, 2, 1), KnightTour 2:1);
	end_time = clock();

	printf("Total time taken by CPU: %f\n", (double)(end_time - start_time)/ CLOCKS_PER_SEC);
	PrintBoard(matrix);
	
	start_time = clock();
	TEST(SUCCESS == KnightTour(matrix, 6, 0), KnightTour 6:0);
	end_time = clock();

	printf("Total time taken by CPU: %f\n", (double)(end_time - start_time)/ CLOCKS_PER_SEC);
	
	PrintBoard(matrix);
}