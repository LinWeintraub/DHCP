#ifndef __TRIE__
#define __TRIE__

#include <stddef.h>

typedef struct trie trie_t;
typedef struct trie_node trie_node_t;

typedef enum
{
    TRIE_SUCCESS = 0,
    TRIE_FAILURE = 1,
    TRIE_MEMORY_ALLOCATION_ERROR = 2
} trie_status_t;

trie_t *TrieCreate(unsigned int amount_of_bits); 

void TrieDestroy(trie_t *trie);

trie_status_t TrieInsert(trie_t *trie,  unsigned int data);

trie_status_t TrieRemove(trie_t *trie, unsigned int data);

unsigned int TrieCountFree(trie_t *trie);

unsigned int TrieDepth(trie_t *trie);

size_t TrieSize(trie_t *trie);


#endif /* __TRIE__ */
