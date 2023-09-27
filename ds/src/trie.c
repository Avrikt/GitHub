/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Harel Salhuv					*
* Date: 27.04.2023							*
* Description: Source file for trie			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/
#include <limits.h> /*CHAR_BIT*/

#include "trie.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/
#define EMPTY (0)
#define FULL  (1)
#define BIT_ON (1)
#define LEFT_CHILD(node) (node->children[LEFT])
#define RIGHT_CHILD(node) (node->children[RIGHT])
#define CHILD_POSITION (address >> (num_of_bits - 1)) & BIT_ON
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

trie_node_t *CreateTrieNode(void);

static void FreeNode(trie_node_t *node);

static trie_status_t TrieTraverse(trie_node_t *node, unsigned int address, size_t num_of_bits, unsigned int *received_address);

static trie_status_t RemoveNode(trie_node_t *node, unsigned int address, size_t num_of_bits);

static size_t CountNodes(trie_node_t *node, size_t num_of_bits);

static int SubTrieIsFull(trie_node_t *root);

/************************************Functions***************************************************/

trie_t *TrieCreate(void)
{
    trie_t *new_trie = (trie_t *)malloc(sizeof(trie_t));
    if (NULL == new_trie)
    {
        return NULL;
    }

    new_trie->root = CreateTrieNode();
    if (NULL == new_trie->root)
    {
        free(new_trie);
        return NULL;
    }

    return new_trie;
}

void TrieDestroy(trie_t *trie)
{
    FreeNode(trie->root);
    free(trie);
}

trie_status_t TrieInsert(trie_t *trie, unsigned int address, size_t num_of_bits, unsigned int *recieved_address)
{
    assert(NULL != trie);

    if(TRUE == SubTrieIsFull(trie->root))
    {
        return TRIE_FULL;
    }

    return TrieTraverse(trie->root, address, num_of_bits, recieved_address);
}

trie_status_t TrieRemove(trie_t *trie, unsigned int address, size_t num_of_bits)
{
    assert(NULL != trie);

    return RemoveNode(trie->root, address, num_of_bits);
}

size_t TrieCount(const trie_t *trie, size_t num_of_bits)
{
    assert(NULL != trie);
    
    return CountNodes(trie->root, num_of_bits);
}

/************************************ Static Functions *******************************************/

trie_node_t *CreateTrieNode(void)
{
    trie_node_t *node = (trie_node_t *) malloc(sizeof(trie_node_t));

    if (NULL == node)
    {
        return NULL;
    }
        
    node->children[LEFT] = NULL;
    node->children[RIGHT] = NULL;
    node->is_full = EMPTY;

    return node;
}

static void FreeNode(trie_node_t *node)
{
    if(NULL == node)
    {
        return;
    }
    
    FreeNode(LEFT_CHILD(node));
    FreeNode(RIGHT_CHILD(node));
    free(node);
}

static trie_status_t TrieTraverse(trie_node_t *node, unsigned int address, size_t num_of_bits, unsigned int *received_address)
{
    child_pos_t child_pos = CHILD_POSITION;
    trie_node_t *next_node = NULL;;

    if(0 == num_of_bits)
    {
        node->is_full = FULL;
        *received_address = address;
        return SUCCESS;
    }

    next_node = node->children[child_pos];
    if (NULL == next_node)
    {
        next_node = CreateTrieNode();
        if (NULL == next_node)
        {
            return TRIE_DS_FAILURE;
        }
        node->children[child_pos] = next_node;
    }

    if(NULL != next_node && FULL == next_node->is_full)
    {
        address ^= (BIT_ON << (num_of_bits - 1));
        ++num_of_bits;
        *received_address = address;
        next_node->is_full = FULL;
        next_node = node;
    }
    
    if (SUCCESS == TrieTraverse(next_node, address, num_of_bits - 1, received_address))
    {
        if(TRUE == SubTrieIsFull(next_node))
        {
            next_node->is_full = FULL;
        }
        return SUCCESS;
    }

    return TRIE_DS_FAILURE;
}

static trie_status_t RemoveNode(trie_node_t *node, unsigned int address, size_t num_of_bits)
{
    child_pos_t child_pos = CHILD_POSITION;
    trie_node_t *next_node = node->children[child_pos];
    trie_status_t status = SUCCESS;

    if(0 == num_of_bits)
    {
        if (EMPTY == node->is_full)
        {
            return TRIE_DOUBLE_FREE;
        }

        node->is_full = EMPTY;
        return SUCCESS;
    }

    if (NULL == next_node)
    {
        return TRIE_INVALID_FREE;
    }
    
    status = RemoveNode(next_node, address, num_of_bits - 1);
    
    node->is_full = EMPTY;

    return status;
}

static size_t CountNodes(trie_node_t *node, size_t num_of_bits)
{
    size_t count = 0;
    
    if (0 == num_of_bits || NULL == node)
    {
        if (NULL != node && FULL == node->is_full)
        {
            ++count;
            return count;
        }
        return 0;
    }

    count += CountNodes(LEFT_CHILD(node), num_of_bits - 1); 
    count += CountNodes(RIGHT_CHILD(node), num_of_bits - 1); 

    return count;
}

static int SubTrieIsFull(trie_node_t *root)
{
    return(NULL != RIGHT_CHILD(root) && NULL != LEFT_CHILD(root) && root->children[LEFT]->is_full == FULL && root->children[RIGHT]->is_full == FULL);
}