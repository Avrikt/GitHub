/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 10.04.23							*
* Description: Test file for Calculator		*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h>

#include "calc.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/

#define NUM_TESTS 82
#define BUFF_SIZE 35

typedef struct calc_test
{
	char str[BUFF_SIZE];
	double result;
}calc_test_t;

calc_test_t test_arr[NUM_TESTS] = {
			{"1 + 1", 2},
			{"2 * 3", 6},
			{"7 - 10", -3},
			{"1 / 4", 0.25},
			{"2 ^ 3", 8},
			{"2 + 5 +  3", 10},
			{"2 * 3 * 4", 24},
			{"-1 + 10", 9},
			{"+4 + 3", 7},
			{"1 + 3 - 7", -3},
			{"2 * 3 + 1", 7},
			{"1 + 2 * 3", 7},
			{"-1 + 8", 7},
			{"-1 - -8", 7},
			{"+9 - -8", 17},
			{"2 * (3 + 4)", 14},
			{"(3 + 4)*  2", 14},
			{"1 / 2 + 2", 2.5},
			{"2 + 1 / 2", 2.5},
			{"3*(2+1)", 9},
			{"10 - -5", 15},
			{"-5 + 10 + 15", 20},
			{"3 ^ 2", 9},
			{"9 / 3", 3},
			{"(5 + 3) * 2", 16},
			{"2 + 3 * 4", 14},
			{"(2 + 3) * 4", 20},
			{"2 ^ 0", 1},
			{"(2 + 3) ^ 2", 25},
			{"1 + 2 * 3 + 4 * 5 + 6", 33},
			{"(1 + 2) * (3 + 4)", 21},
			{"(1 + 2) * 3 + 4", 13},
			{"1 * 2 + 3 * 4", 14},
			{"3 ^ 2 + 4 ^ 2", 25},
			{"2 ^ 2 + 3 ^ 2 + 4 ^ 2", 29},
			{"(1 + 2) * 3", 9},
			{"(2 * 3) + 4", 10},
			{"(5 - 2) * (4 + 1)", 15},
			{"(10 - 5) / 5", 1},
			{"(3 + 5) / (2 * 2)", 2},
			{"(2 * 3) ^ 2", 36},
			{"(3 + 4) ^ 2", 49},
			{"(4 - 2) ^ 3", 8},
			{"2 * (3 + 4)", 14},
			{"4 / (2 + 2)", 1},
			{"(2 ^ 3) / 4", 2},
			{"3 * (4 + 5) / 2", 13.5},
			{"(3 + 4 * 5) / (2 - 3)", -23},
			{"(2 * 3 + 4) * 5", 50},
			{"(2 + 3) * (4 + 5)", 45},
			{"(1 + 2 + 3 + 4) * 5", 50},
			{"5 * (1 + 2 + 3 + 4)", 50},
			{"(2 + 3) * 4 - 5", 15},
			{"2 + (3 * 4 - 5)", 9},
			{"(1 + 2) ^ (3 - 1)", 9},
			{"2 ^ (1 + 3)", 16},
			{"(3 - 1) ^ (1 + 2)", 8},
			{"2 + 3 * 4 ^ 2", 50},
			{"(2 + 3) * 4 ^ 2", 80},
			{"(2 ^ 3) * (4 / 2)", 16},
			{"2 ^ (3 * 4) / 4", 1024},
			{"(2 * 3) + (4 * 5) - (6 / 3)", 24},
			{"(1 + 2) * (3 + 4) * (5 + 6)", 231},
			{"(1 + 1) ^ (0 + 2) ^ 3", 256},
			{"2 + (3 * 4) ^ 2 + 5", 151},
			{"(2 + 3 * 4) ^ 2 + 5", 201},
			{"(2 ^ 3) * (4 / 2) + 5", 21},
			{"2 ^ (3 * 4) / 4 - 5", 1019},
			{"(2 * 3) + (4 * 5) - (6 / 3) ^ 2", 22},
			{"(1 + 2) * (3 + 4) * (5 + 6) / 7", 33},
			{"3 ^ 2 ^ 2 * 3", 243},
			{"(((1 + 1)))", 2},
			{"  (  (((( 2 + 2 ) * 2)  ) ))", 8},
			{"-(1 + 2)", -3},
			{"+(1 + 2)", 3},
			{"((+3 - -2) * 4 - 2) / (5 - 1)", 4.5},
			{"(2 + (3 * 4)) / ((-5 + +9) * 2)", 1.75},
			{"((2 + 3) * (4 - 1)) / (5 - (2 + 1))", 7.5},
			{"((10 / 2) - 3) * ((+6 - 2)^2)", 32},
			{"((2 * 3) + 4) / ((5 - 1) * (2 - 1))", 2.5},
			{"((10 - (3 * 2)) / 4) + (6 - 2)^2", 17},
			{"((5 - 2) * ((3 + 4) / 2)) / (2 + 1)", 3.5}
};

/********************************FORWARD DECLERATIONS*******************************************/
static void TestCalculator(void);

int main(void)
{
	TestCalculator();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestCalculator(void)
{
	calc_t *calc = CalcCreate(10);
	size_t i = 0;
	double result = 0;
	char *exp1 = "2 * (3 + 4)";
	char *exp2 = "5 + -9";
	char *exp3 = "2^3-5*-2.5";
	char *exp4 = "1 + 3 - 7";
	char *exp5 = "8^2";
	char exp6[100] = "7+8";                 /* Result = 15 status = SUCCESS       			*/
    char exp7[100] = "8+8*3+-2^5";         	/* Result =  0 status = SUCCESS      			*/
    char exp8[100] = "8+8*3-2^";         	/* Result =  0 status = SYNTAX_ERROR 			*/
    char exp9[100] = "2/0";                 /* Result =  0 status = DIVISION_BY_ZERO   		*/
    char exp10[100] = "8++8*((3-2)*5)";     /* Result = 48 status = SUCCESS       			*/
    char exp11[100] = "3-2)*5";             /* Result =  0 status = UNBALANCED_PARANTHESIS 	*/
    char exp12[100] = "(3-2)*5+ 5*(4+4+4";	/* Result =  0 status = UNBALANCED_PARANTHESIS 	*/


	HEADER(Calculator);
	TEST(SUCCESS == Calculator(calc, exp1, &result), Calc Status 1);
	TEST(14 == result, Result 1);
	TEST(SUCCESS == Calculator(calc, exp2, &result), Calc Status 2);
	TEST(-4 == result, Result 2);
	TEST(SUCCESS == Calculator(calc, exp3, &result), Calc Status 3);
	TEST(20.5 == result, Result 3);
	TEST(SUCCESS == Calculator(calc, exp4, &result), Calc Status 4);
	TEST(-3 == result, Result 4);
	TEST(SUCCESS == Calculator(calc, exp5, &result), Calc Status 5);
	TEST(64 == result, Result 5);
	TEST(SUCCESS == Calculator(calc, exp6, &result), Calc Status 6);
	TEST(15 == result, Result 6);
	TEST(SUCCESS == Calculator(calc, exp7, &result), Calc Status 7);
	TEST(0 == result, Result 7);
	TEST(SYNTAX_ERROR == Calculator(calc, exp8, &result), Calc Status 8);
	TEST(0 == result, Result 8);
	TEST(DIVISION_BY_ZERO == Calculator(calc, exp9, &result), Calc Status 9);
	TEST(0 == result, Result 9);
	TEST(SUCCESS == Calculator(calc, exp10, &result), Calc Status 10);
	TEST(48 == result, Result 10);
	TEST(UNBALANCED_PARANTHESIS == Calculator(calc, exp11, &result), Calc Status 11);
	TEST(0 == result, Result 11);
	TEST(UNBALANCED_PARANTHESIS == Calculator(calc, exp12, &result), Calc Status 12);
	TEST(0 == result, Result 12);


	for (i = 0; i < NUM_TESTS; ++i)
	{
		Calculator(calc, test_arr[i].str, &result);
		printf("test number %ld:", i + 1);
		TEST(test_arr[i].result == result, );
	}
	
	CalcDestroy(calc);
}
