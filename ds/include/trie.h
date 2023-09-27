/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Harel Salhuv					*
* Date: 27.04.2023							*
* Description: Header file for trie			*
*********************************************/

#ifndef MY_PROJECT_TRIE_H__
#define MY_PROJECT_TRIE_H__

#include <stddef.h> /*size_t*/

typedef struct trie trie_t;
typedef struct trie_node trie_node_t;

typedef enum trie_status
{
    TRIE_FULL = 1,   
    TRIE_DOUBLE_FREE = 2,
    TRIE_DS_FAILURE = 3,
    TRIE_INVALID_FREE = 4 /*free network, server or broadcast addresses*/
}trie_status_t;

trie_t *TrieCreate(void);

void TrieDestroy(trie_t *trie);

trie_status_t TrieInsert(trie_t *trie, unsigned int address, size_t num_of_bits, unsigned int *received_address);

trie_status_t TrieRemove(trie_t *trie, unsigned int address, size_t num_of_bits);

size_t TrieCount(const trie_t *trie, size_t num_of_bits);

#endif