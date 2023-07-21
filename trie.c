/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.1                                           *
 ~ Date          6/12/22                                         ~
 * Description   Trie                                            *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <math.h> /*pow*/

#include "trie.h"

#define TWO 2
#define BIT 1

typedef enum
{
    LEFT,
    RIGHT,
    NUM_CHILDREN = 2
} direction_t;

struct trie_node
{
    int is_full;
    struct trie_node *parent;
    struct trie_node *children[NUM_CHILDREN];
};

struct trie
{
    trie_node_t *root;
    unsigned int trie_depth;
    size_t trie_size;
};

/*---------------------------Function Declarations----------------------------*/
static void TrieUpdateIsFullBranch(trie_node_t *node);
static void TrieDestroyRecursive(trie_node_t *node);
static trie_node_t *TrieNodeCreate(trie_node_t *node);
static void TrieRemoveBranch(trie_node_t *node);
static trie_node_t *GoToNode(trie_t *trie, unsigned int data);
static int IsLeftChild(trie_node_t *node);

/*---------------------------------API-Functions------------------------------*/
trie_t *TrieCreate(unsigned int amount_of_bits)
{
    trie_t *trie = malloc(sizeof(trie_t));
    trie_node_t *root = malloc(sizeof(trie_node_t));
    
    if (NULL == trie)
	{   
		return NULL;
	}

    if (NULL == root)
	{   
        free(trie); trie = NULL;
		return NULL;
	}

    root->is_full = 0;
    root->parent = NULL;
	root->children[LEFT] = NULL;
	root->children[RIGHT] = NULL;

    trie->root = root;
    trie->trie_depth = amount_of_bits;
    trie->trie_size = 0;

    return trie;
}
/*----------------------------------------------------------------------------*/
void TrieDestroy(trie_t *trie)
{
    trie_node_t *node = NULL;

    assert(trie);

    node = trie->root;
    TrieDestroyRecursive(node);

    free(trie); trie = NULL;
}
/*----------------------------------------------------------------------------*/
trie_status_t TrieInsert(trie_t *trie, unsigned int data)
{
    int i = 0;
    trie_node_t *curr = NULL;
    trie_node_t *new_node = NULL;
    direction_t next_bit = LEFT;
    int is_node_created = 0;

    assert(trie);
    
    curr = trie->root;

    if (1 == trie->root->is_full)
    {
        return TRIE_FAILURE;
    }

    for (i = TrieDepth(trie) - 1; i >= 0; --i)
    {
        next_bit = (data >> i) & BIT;

        if (curr->children[next_bit] == NULL)
        { 
            new_node = TrieNodeCreate(curr);

            if (new_node == NULL)
            {
                TrieRemoveBranch(curr);

                return TRIE_MEMORY_ALLOCATION_ERROR;
            }      

            is_node_created = 1;
            curr->children[next_bit] = new_node;
        }

        curr = curr->children[next_bit];
    }
    
    if (is_node_created == 1)
    {   
        curr->is_full = 1;
        TrieUpdateIsFullBranch(curr);
        ++(trie->trie_size);
    }

    else
    {
        return TRIE_FAILURE;
    }

    return TRIE_SUCCESS;
}
/*----------------------------------------------------------------------------*/
trie_status_t TrieRemove(trie_t *trie, unsigned int data)
{
    trie_node_t *curr = NULL;
    trie_node_t *parent = NULL;
    unsigned int i = 0;

    assert(trie);

    curr = GoToNode(trie, data);

    if (curr == NULL)
    {
        return TRIE_FAILURE;
    }
        
    for (i = TrieDepth(trie) - 1; i != 0; i--)
    {
        parent = curr->parent;    

        if ((NULL == curr->children[LEFT]) && (NULL == curr->children[RIGHT]))
        {
            if (IsLeftChild(curr))
            {
                parent->children[LEFT] = NULL;
            }

            else
            {
                parent->children[RIGHT] = NULL;
            }
            
            free(curr); curr = NULL;
        }

        else
        {
            curr->is_full = 0;
        }

        curr = parent;
    }

    --(trie->trie_size);

    return TRIE_SUCCESS;
}
/*----------------------------------------------------------------------------*/
unsigned int TrieCountFree(trie_t *trie)
{
    assert(trie);

    return (pow(TWO, TrieDepth(trie)) - TrieSize(trie));
}
/*----------------------------------------------------------------------------*/
unsigned int TrieDepth(trie_t *trie)
{
    assert(trie);

    return (trie->trie_depth);
}
/*----------------------------------------------------------------------------*/
size_t TrieSize(trie_t *trie)
{
    assert(trie);

    return (trie->trie_size);
}

/*--------------------------------Static-Functions----------------------------*/
static void TrieDestroyRecursive(trie_node_t *node)
{
    if (NULL == node)
    {
        return;
    }

    TrieDestroyRecursive(node->children[LEFT]);
    TrieDestroyRecursive(node->children[RIGHT]);

    free(node); node = NULL;
}
/*----------------------------------------------------------------------------*/
static trie_node_t *TrieNodeCreate(trie_node_t *node)
{
    trie_node_t *new_node = malloc(sizeof(trie_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }

    assert(node);

    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->parent = node;
    new_node->is_full = 0;

    return new_node;
}
/*----------------------------------------------------------------------------*/
static int IsLeftChild(trie_node_t *node)
{
    assert(node);

    if (NULL == node->parent)
    {
        return 0;
    }

    if (node == node->parent->children[LEFT])
    {
        return 1;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
static void TrieRemoveBranch(trie_node_t *node)
{
    trie_node_t *parent = NULL;

    assert(node);

    while (NULL != node->parent)
    {
        if ((NULL == node->children[LEFT]) && (NULL == node->children[RIGHT]))
        {
            parent = node->parent; 

            if (IsLeftChild(node))
            {
                parent->children[LEFT] = NULL;
            }

            else
            {
                parent->children[RIGHT] = NULL;
            }
            
            free(node); node = NULL;
            node = parent;
        }

        else
        {
            break;
        }
    }
}
/*----------------------------------------------------------------------------*/
static trie_node_t *GoToNode(trie_t *trie, unsigned int data)
{
    unsigned int i = 0;
    unsigned int next_bit = LEFT;
    trie_node_t *curr = trie->root;

    assert(trie);

    curr = trie->root;

    for (i = TrieDepth(trie) - 1; i != 0; --i)
    {
        next_bit = (data >> i) & BIT;

        if (NULL == curr->children[next_bit])
        {
            return NULL;
        }

        curr = curr->children[next_bit];
    }

    return curr;   
}
/*----------------------------------------------------------------------------*/
static void TrieUpdateIsFullBranch(trie_node_t *node)
{
    while (node != NULL)
    {
        if (NULL == node->children[LEFT] || NULL == node->children[RIGHT])
        {
            node = node->parent;
            continue;
        }

        if ((1 == node->children[LEFT]->is_full) && (1 == node->children[RIGHT]->is_full))
        {
            node->is_full = 1;
        }

        node = node->parent;
    }
}
