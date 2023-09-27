/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Harel Salhuv					*
* Date: 27.04.2023							*
* Description: Test file for trie			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include "trie.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/
typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
}child_pos_t;

struct trie_node
{
    trie_node_t *children[NUM_OF_CHILDREN];
    int is_full;
};

struct trie
{
    trie_node_t *root; 
};

/********************************FORWARD DECLERATIONS*******************************************/
static void TestTrie(void);

static void TestFullTrie(void);

int main(void)
{
	TestTrie();
	TestFullTrie();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestTrie(void)
{
	trie_t *trie = TrieCreate();
	unsigned int buffer = 0;

	HEADER(TrieCreate);
	TEST(NULL != trie, TrieCreate);

	HEADER(TrieCount);
	TEST(0 == TrieCount(trie, 8), TrieCount);

	HEADER(TrieInsert);
	TEST(SUCCESS == TrieInsert(trie, 15, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);

	TEST(SUCCESS == TrieInsert(trie, 15, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);

	TEST(SUCCESS == TrieInsert(trie, 0, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);
		
	TEST(SUCCESS == TrieInsert(trie, 0, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);
	
	TEST(SUCCESS == TrieInsert(trie, 255, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);
	TEST(SUCCESS == TrieInsert(trie, 255, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);
	

	TEST(SUCCESS == TrieInsert(trie, 14, 8, &buffer), TrieInsert);
	printf("received address: %u\n", buffer);
	
	
	HEADER(TrieCount);
	TEST(7 == TrieCount(trie, 8), TrieCount);
	TrieInsert(trie, 132, 8, &buffer);
	TrieInsert(trie, 100, 8, &buffer);
	TrieInsert(trie, 87, 8, &buffer);
	TEST(10 == TrieCount(trie, 8), TrieCount);

	HEADER(TrieRemove);
	TEST(SUCCESS == TrieRemove(trie, 15, 8), TrieRemove);
	TEST(9 == TrieCount(trie, 8), TrieRemove);
	TEST(TRIE_INVALID_FREE == TrieRemove(trie, 37, 8), TrieRemove - Invalid);
	TEST(TRIE_DOUBLE_FREE == TrieRemove(trie, 15, 8), TrieRemove - Double);
	
	TrieDestroy(trie);
}

static void TestFullTrie(void)
{
	size_t i = 0;
	trie_t *trie = TrieCreate();
	unsigned int received = 0;

	HEADER(TestFullTrie);

	for (i = 0; i < 256; ++i)
	{
		TrieInsert(trie, i, 8, &received);
	}

	TEST(TRIE_FULL == TrieInsert(trie, 255, 8, &received), TrieInsert - Full);

	TrieDestroy(trie);
}