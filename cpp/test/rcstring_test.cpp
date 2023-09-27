/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Markov   				    *
* Date: 26.06.2023						    	*
* Description: Test file for RCString        	*
* Version: 1.3                                  *
* Status: Approved                              *
*************************************************/

#include <cstring>

#include "rcstring.hpp"
#include "utilities.hpp"

using namespace dev;

static void TestCtor(void);

static void TestAssignOperator(void);

static void TestIsShared(void);

static void TestLength(void);

static void TestBoolOperators(void);

static void TestIndexOperators(void);

int main(void)
{
    TestCtor();

    TestAssignOperator();

    TestIsShared();

    TestLength();

    TestBoolOperators();

    TestIndexOperators();

    return 0;
}

static void TestCtor(void)
{
    RCString rc1("Avri");
    RCString rc2(rc1);
    RCString rc3;

    HEADER(Constructors);
    TEST(0 == strcmp(rc1.ToCStr(), "Avri"), Ctor);
    TEST(0 == strcmp(rc2.ToCStr(), rc1.ToCStr()), CCtor);
    TEST(0 == strcmp(rc3.ToCStr(), ""), Default Ctor);
}

static void TestAssignOperator(void)
{
    RCString r1("Max Verstappen");
    RCString r2;

    HEADER(Operator=);
    TEST(false == (r2.IsShared()), !IsShared);
    TEST(false == (r1.IsShared()), !IsShared);
    TEST(true == (r2 != r1), operator!=);
    r2 = r1;
    TEST(true == r1.IsShared(), IsShared);
    TEST(true == r2.IsShared(), IsShared);
    TEST(true == (r1 == r2), operator=);
}

static void TestIsShared(void)
{
    RCString r1("IsShared");

    HEADER(IsShared);
    TEST(false == r1.IsShared(), IsShared());
    RCString r2(r1);
    TEST(true == r1.IsShared(), IsShared());
    TEST(true == r2.IsShared(), IsShared());
}

static void TestLength(void)
{
    RCString r1("Lewis Hamilton");
    RCString r2(r1);
    RCString r3;

    HEADER(Length);
    TEST(14 == r1.Length(), Length());
    TEST(14 == r2.Length(), Length());
    TEST(0 == r3.Length(), Length());
}

static void TestBoolOperators(void)
{
    RCString r1("Michael Schumacher");
    RCString r2(r1);
    RCString r3("Lewis Hamilton");

    HEADER(Bool Operators);
    TEST(true == (r2 == r1), operator==);
    TEST(true == (r1 != r3), operator!=);
    TEST(true == (r2 > r3), operator>);
    TEST(true == (r3 < r1), operator<);
}

static void TestIndexOperators(void)
{
    const RCString r1("Daniel Ricciardo");
    RCString r2("Checo Perez");
    RCString r3(r2);

    HEADER(Index Operators);
    HEADER(const operator[]);
    TEST('D' == r1[0], const operator[]);
    TEST(' ' == r1[6], const operator[]);
    TEST('R' == r1[7], const operator[]);
    TEST('\0' == r1[16], const operator[]);

    HEADER(operator[]);
    TEST(true == r2.IsShared(), IsShared);
    TEST(true == r3.IsShared(), IsShared);
    TEST(true == (r3 == r2), r3 == r2);
    r3[6] = 'B';
    TEST("Checo Perez" == r2, operator[]);
    TEST("Checo Berez" == r3, operator[]);
    TEST(false == r2.IsShared(), !IsShared);
    TEST(false == r3.IsShared(), !IsShared);
    TEST(true == (r3 != r2), r3 != r2);
}