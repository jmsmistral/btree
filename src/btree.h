/**
 ** 
 ** btree interface
 **
 **/

#ifndef _BTREE__H_
#define _BTREE__H_

#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<errno.h>
#include<string.h>
#include<sys/uio.h> /* struct iovec */
#include<assert.h>



#ifdef BTREE_DEBUG
#define debug_print(...) \
     do { fprintf(stderr, __VA_ARGS__); } while (0)
#else
#define debug_print(...)
#endif

/**
 ** Implementation limits
 **/
#define MIN_DEGREE 6 /* defines the upper and lower bounds of a btree (see p434 Intro to Algorithms, Cormen et al.) */
#define MIN_KEYS MIN_DEGREE-1
#define MAX_KEYS (2*MIN_DEGREE)-1
#define MIN_CHILDREN MIN_DEGREE
#define MAX_CHILDREN 2*MIN_DEGREE

#define BTREE_NODE_SIZE 456 /* Given MIN_DEGREE=6: (BTreeKey * 11) + (off_t * 12) + (unsigned int * 2) */

#define LEAF_TRUE 1
#define LEAF_FALSE 0



/**
 ** Btree key 
 **/
typedef struct {
    unsigned int isUnlinked; /* marks key as deleted in data set */
    off_t keyValue;
    off_t dataOffset; /* offset of data record in a given data file */
    off_t dataLength; /* length of data record in bytes */
} BTreeKey;



/**
 ** Btree node
 **/
typedef struct {
    BTreeKey key[MAX_KEYS];
    off_t child[MAX_CHILDREN];
    void* childPtr[MAX_CHILDREN];

    unsigned int numKeys;
    unsigned int isLeaf;
} BTreeNode;



/**
 ** Btree
 **/
typedef struct {
    BTreeNode* root;

    off_t rootNodeOffset;
    unsigned int numNodes; 
    unsigned int numKeys; 
} BTree;



/**
 ** BTreeQueueNode
 **/
typedef struct {
    BTreeNode* nodePtr;
    BTreeNode* parentPtr;
    unsigned int nodeLevel;
    unsigned int visited;
} BTreeQueueNode;



/**
 ** BTreeQueue
 **/
typedef struct {
    BTree* btree;
    BTreeQueueNode* queue;
    unsigned int size;
} BTreeQueue;



/* btree callbacks  */
typedef void (*queueNodeCallback) (BTreeQueueNode* qnode);
    void btreeCBPrintNode(BTreeQueueNode* qnode);
    void btreeCBFreeNode(BTreeQueueNode* qnode);

/* btree memory */
BTree* btreeAlloc(void);
BTreeNode* btreeNodeAlloc(void);
BTreeKey* btreeKeyAlloc(void);
BTreeQueue* btreeQueueAlloc(BTree* btree);
BTreeQueueNode* btreeQueueNodeAlloc(BTree* btree);
void btreeFree(BTree** btree);
void btreeNodeFree(BTreeNode** node);
void btreeKeyFree(BTreeKey** key);
void btreeQueueFree(BTreeQueue** queue);
void btreeQueueNodeFree(BTreeQueueNode** queueNode);

/* btree operations */
BTreeKey* btreeSearch(BTree* btree, BTreeNode* node, off_t key);
int btreeInsert(BTree* btree, BTreeKey* key);
int btreeInsertNonFull(BTree* btree, BTreeNode* node, BTreeKey* key);
int btreeSplitChild(BTree* btree, BTreeNode* x, const unsigned int i, BTreeNode* y);
int btreeDelete(BTree* btree, off_t key);

/* btree utils */
void btreeForEach(BTree* btree, queueNodeCallback callbackPtr);
BTreeQueue* populateBTreeQueueBFS(BTree* btree);
void setQueueNode(BTreeQueueNode*, BTreeNode*, BTreeNode*, unsigned int, unsigned int);
void addQueueNode(BTreeQueue*, BTreeQueueNode*);
void btreeSetKey(BTreeKey* key, off_t keyValue, off_t dataOffset, off_t dataLength);
void btreePrintNode(BTreeNode* node, const char* label, const unsigned int visited);

/* btree macros */
#define nodeIsFull(node) ((node)->numKeys == MAX_KEYS)
#define isRoot(tree) ((tree)->numNodes == 1) /* jms: wrong - rethink or remove */
#define btreegetSize(tree) ((tree)->size)
#define btreeGetRoot(tree) ((tree)->root)
#define btreeIsEob(tree) ((node)==NULL)
#define btreeIsLeaf(tree) ((node)->left == NULL && (node)->right == NULL)
#define btreeGetKey(node) ((node)->key)

#endif
