/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Tomer Savorai				*
* Date: 16.04.23							*
* Description: Test file for Hash Table		*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h>
#include <string.h> /*strcmp, strcpy, strcspn*/

#include "hash.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/
#define MAX_WORDS (104334)
#define MAX_WORD_LENTGH (50)

/********************************FORWARD DECLERATIONS*******************************************/

static void TestHashTable(void);

static void TestHashDict(void);

static size_t HashFunc(const void *value);

static int DoesStringMatch(const void *lhd, const void *rhd);

int main(void)
{
	TestHashTable();
	TestHashDict();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestHashTable(void)
{
	hash_table_t *table = HashTableCreate(6, HashFunc, DoesStringMatch);
	char *test1 = "Avri";
	char *test2 = "Bernard";
	char *test3 = "Kylian";
	char *test4 = "Gianluigi";
	char *test5 = "Erlin";
	char *test6 = "Michael";
	char *test7 = "Jordan";
	char *find = "";


	HEADER(HashTableCreate);
	TEST(NULL != table, HashTableCreate);
	TEST(TRUE == HashTableIsEmpty(table), HashTableIsEmpty);
	TEST(0 == HashTableSize(table), HashTableSize);
	
	HEADER(HashTableInsert);
	TEST(SUCCESS == HashTableInsert(table, test1), HashTableInsert);
	TEST(SUCCESS == HashTableInsert(table, test2), HashTableInsert);
	TEST(SUCCESS == HashTableInsert(table, test3), HashTableInsert);
	TEST(SUCCESS == HashTableInsert(table, test4), HashTableInsert);
	TEST(SUCCESS == HashTableInsert(table, test5), HashTableInsert);

	HEADER(HashTableSize);
	TEST(5 == HashTableSize(table), HashTableSize);
	HashTableInsert(table, test6);
	TEST(6 == HashTableSize(table), HashTableSize);


	HEADER(HashTableFind);
	find = (char *)HashTableFind(table, test2);
	TEST(find == test2, HashTableFind);
	find = (char *)HashTableFind(table, test4);
	TEST(find == test4, HashTableFind);
	find = (char *)HashTableFind(table, test1);
	TEST(find == test1, HashTableFind);
	find = (char *)HashTableFind(table, test7);
	TEST(NULL == find, HashTableFind- Fail);

	HEADER(HashLoad);
	TEST(1 == HashLoad(table), HashLoad);

	HEADER(HashSTDev);
	printf("The standard deviation is: %f\n", HashSTDev(table));

	HEADER(HashTableRemove);
	HashTableRemove(table, test4);
	find = (char *)HashTableFind(table, test4);
	TEST(NULL == find, HashTableRemove);
	TEST(5 == HashTableSize(table), HashTableSize);


	HashTableDestroy(table);
}

static void TestHashDict(void)
{
	hash_table_t *table = HashTableCreate(20, HashFunc, DoesStringMatch);
	char dict_array[MAX_WORDS][MAX_WORD_LENTGH] = {0};
	char buffer [MAX_WORD_LENTGH] = {0};
	char user_input[MAX_WORD_LENTGH] = {0};
	char *test = NULL;
	size_t i = 0;
	FILE *fp = NULL;

	HEADER(Hash Dictionary);
	fp = fopen("/usr/share/dict/american-english", "r");
	
	while (fgets(buffer, MAX_WORD_LENTGH, fp))
	{
		buffer[strcspn(buffer, "\n")] = '\0';
		strcpy(dict_array[i], buffer);
		HashTableInsert(table, dict_array[i++]);
	}

	fclose(fp);
	TEST(104334 == HashTableSize(table), HashTableSize);
	HEADER(HashSTDev);
	printf("The standard deviation is: %f\n", HashSTDev(table));

	HEADER(HashTableFind);
	printf("Input word to search for: \n");
	scanf("%s", user_input);
	test = (char *)HashTableFind(table, user_input);
	
	if(NULL != test)
	{
		TEST(*user_input == *test, HashTableFind);
	}
	else
	{
		printf("String not found\n");
	}

	HashTableDestroy(table);
}

/************************************ User Functions *******************************************/
static size_t HashFunc(const void *value)
{
	size_t hash_value = 5381;
	char *runner = (char *)value;

	while ('\0' != *runner)
	{
		hash_value = ((hash_value << 5) + hash_value) + (*runner++);
	}
	
	return hash_value;
}

static int DoesStringMatch(const void *lhd, const void *rhd)
{
	return (0 == strcmp((char *)lhd, (char *)rhd));
}