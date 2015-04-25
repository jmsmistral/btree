/**
 ** 
 ** Implementation of btree interface
 **
 **/

#include "btree.h"

/************************************************
 ************************************************
 **                                           **
 **                BTree Memory               **
 **                                           **
 ************************************************
 ************************************************/



/** 
 ** Allocates memory for a BTree
 **/
BTree* btreeAlloc(void) {

    BTree* btree;

    /* Allocate memory for the btree */
    debug_print("btreeAlloc: Allocating memory for btree...\n");
    if((btree = calloc(1, sizeof(BTree))) == NULL ) {
        printf("btreeAlloc: malloc error\n");
        return NULL;
    }

    /* When a btree is allocated, so is the root node */
    btree->root = btreeNodeAlloc();
    assert(btree->root != NULL);

    btree->numNodes = 1;
    btree->numKeys = 0;
    btree->rootNodeOffset = 0;
    //btree->nextFreeNodeOffset = 0;
    
    return btree;

}



/** 
 ** Allocates memory for a BTreeNode
 **/
BTreeNode* btreeNodeAlloc(void) {
    
    BTreeNode* node;   
 
    /* Allocate memory for the btree root node */
    debug_print("btreeNodeAlloc: Allocating memory for btree node...\n");
    if((node = malloc(sizeof(BTreeNode))) == NULL ) {
        printf("btreeNodeAlloc: malloc error\n");
        return NULL;
    }

    node->numKeys = 0;
    node->isLeaf = LEAF_TRUE;
    
    return node;

}



/** 
 ** Allocates memory for a BTreeKey
 **/
BTreeKey* btreeKeyAlloc(void) {
    
    BTreeKey* key;   
 
    /* Allocate memory for the btree root node */
    debug_print("btreeKeyAlloc: Allocating memory for a btree key...\n");
    if((key = malloc(sizeof(BTreeKey))) == NULL ) {
        printf("btreeKeyAlloc: malloc error\n");
        return NULL;
    }

    key->keyValue = 0;
    key->dataOffset = 0;
    key->dataLength = 0;
    
    return key;

}



/** 
 ** Allocates memory for a BTreeQueue
 **/
BTreeQueue* btreeQueueAlloc(BTree* btree) {

    BTreeQueue* btreeQueue;

    /* Allocate memory for the btreeQueue */
    debug_print("btreeQueueAlloc: Allocating memory for queue...\n");
    if((btreeQueue = calloc(1, sizeof(BTreeQueue))) == NULL ) {
        printf("btreeQueueAlloc: malloc error\n");
        return NULL;
    }

    /* When a BTreeQueue is allocated, so are the first 100 BTreeQueueNodes */
    btreeQueue->queue = btreeQueueNodeAlloc(btree);
    assert(btreeQueue->queue != NULL);

    btreeQueue->btree = btree;
    btreeQueue->size = 0;
    
    return btreeQueue;

}



/** 
 ** Initial memory allocation for a BTreeQueueNode array
 **/
BTreeQueueNode* btreeQueueNodeAlloc(BTree* btree) {
    
    BTreeQueueNode* qnode;   
    unsigned int qnodeArraySize = btree->numNodes;
 
    debug_print("btreeQueueNodeAlloc: Allocating memory (%lu bytes) for %u BTreeQueueNodes...\n", qnodeArraySize*sizeof(BTreeQueueNode)
                                                                                                , qnodeArraySize);
    if((qnode = malloc(qnodeArraySize*sizeof(BTreeQueueNode))) == NULL ) {
        printf("btreeQueueNodeAlloc: malloc error\n");
        return NULL;
    }

    return qnode;

}



/** 
 ** Free memory for a given BTree
 **/
void btreeFree(
    BTree** btreePtr
) {

    debug_print("btreeFree: Freeing btree memory...\n");

    if((*btreePtr)->root != NULL) {
        btreeForEach(*btreePtr, btreeCBFreeNode); /* Free all btree nodes */
        (*btreePtr)->root = NULL;
    }
    
    assert((*btreePtr)->root == NULL);
    debug_print("btreeFree: btree nodes have been freed!\n");

    if(*btreePtr != NULL) {
        free(*btreePtr);
        *btreePtr = NULL;
    }

    assert(*btreePtr == NULL);
    debug_print("btreeFree: btree has been freed!\n");
    
}



/** 
 ** Free memory for a BTreeNode
 **/
void btreeNodeFree(
    BTreeNode** node
) {

    if(*node != NULL) {
        free(*node);
        *node = NULL;
    }

    assert(*node == NULL);
    debug_print("btreeNodeFree: node has been freed!\n");

}



/** 
 ** Free memory for a BTreeKey
 **/
void btreeKeyFree(
    BTreeKey** key
) {

    if(*key != NULL) {
        free(*key);
        *key = NULL;
    }

    assert(*key == NULL);

}



/** 
 ** Free memory for a given BTreeQueue
 **/
void btreeQueueFree(
    BTreeQueue** btreeQueuePtr
) {

    debug_print("btreeQueueFree: Freeing BTreeQueue memory...\n");

    /* Free root node */
    if((*btreeQueuePtr)->queue != NULL)
        btreeQueueNodeFree(&(*btreeQueuePtr)->queue);
    assert((*btreeQueuePtr)->queue == NULL);
    //debug_print("btreeQueueFree: BTreeQueue queue has been freed!\n");

    if(*btreeQueuePtr != NULL) {
        free(*btreeQueuePtr);
        *btreeQueuePtr = NULL;
    }

    assert(*btreeQueuePtr == NULL);
    debug_print("btreeQueueFree: BTreeQueue has been freed!\n");
    
}



/** 
 ** Free memory for a BTreeQueueNode array
 **/
void btreeQueueNodeFree(
    BTreeQueueNode** qnode
) {

    if(*qnode != NULL) {
        free(*qnode);
        *qnode = NULL;
    }

    assert(*qnode == NULL);
    debug_print("btreeQueueNodeFree: BTreeQueueNodes have been freed!\n");

}



/************************************************
 ************************************************
 **                                           **
 **             BTree Operations              **
 **                                           **
 ************************************************
 ************************************************/



/** 
 ** Search for a BTreeKey in a BTree (returns values via passed BTreeKey)
 **/
BTreeKey* btreeSearch(
   BTree* btree
   ,BTreeNode* node
   ,off_t keyValue
) {

   int i;
   debug_print("btreeSearch: node->numkeys = %u\n", node->numKeys);
   for(i = 0; (i <= node->numKeys - 1) && (keyValue > node->key[i].keyValue ); i++);
   debug_print("btreeSearch: i = %d\n", i);
   if((i <= node->numKeys - 1) && (keyValue == node->key[i].keyValue) && (!node->key[i].isUnlinked)) {
      debug_print("btreeSearch: key found\n");
      return (BTreeKey*) &node->key[i];
   }

   debug_print("btreeSearch: key not found this iteration\n");
   if(node->isLeaf)
      return NULL;
   else 
      return btreeSearch(btree, (BTreeNode*) node->childPtr[i], keyValue); /* TODO: Add function to read corresponding child node from disk */

}



/** 
 ** Insert a BTreeKey into a BTree
 **/
int btreeInsert(
    BTree* btree
    ,BTreeKey* key
) {

    /*TODO DEBUG
    printf("btreeInsert: key = %lu\n", key->key);
    printf("btreeInsert: dataOffset = %lu\n", key->dataOffset);
    printf("btreeInsert: dataLength = %lu\n", key->dataLength);
    printf("btreeInsert: btree->root->numKeys = %u\n", btree->root->numKeys);
    printf("btreeInsert: nodeIsFull(btree->root) = %u\n", nodeIsFull(btree->root));*/
    
    if(nodeIsFull(btree->root)) {
        debug_print("btreeInsert: root node full!\n");
        BTreeNode* x;
        x = btreeNodeAlloc();

        BTreeNode* y;
        y = btreeNodeAlloc();

        y = btree->root;
        btree->root = x;
        x->isLeaf = LEAF_FALSE;
        x->numKeys = 0;
        x->child[0] = btree->rootNodeOffset;
        x->childPtr[0] = (BTreeNode*)y;
    
        btreeSplitChild(btree, x, 0, y);
        btree->numNodes += 2; // if root is split we always add two new nodes
        btreeInsertNonFull(btree, x, key);
        //printf("btreeInsert: number of nodes after insert = %u\n", btree->numNodes);
        
    }
    else {
        debug_print("btreeInsert: node not full... \n");
        btreeInsertNonFull(btree, btree->root, key);
        //printf("btreeInsert: number of nodes after insert = %u\n", btree->numNodes);
    }

    return 0;

}



/** 
 ** Insert key into non-full node
 **/
int btreeInsertNonFull(
    BTree* btree
    ,BTreeNode* x
    ,BTreeKey* key
) {
    
    int i = x->numKeys - 1;
    if(x->isLeaf) {
        debug_print("btreeInsertNonFull: node is a leaf!\n");
        
        while (i>=0 && key->keyValue < x->key[i].keyValue) {
            x->key[i+1] = x->key[i];
            i--;
        }
        
        x->key[i+1] = *key;
        x->numKeys++, btree->numKeys++;
        
        return 0;
    }
    else {
        debug_print("btreeInsertNonFull: node is not a leaf...inserting into child!\n");
        /* Find correct index to insert key */
        while (i>=0 && key->keyValue < x->key[i].keyValue)
            i--;
        i++;
        
        
        BTreeNode* child = (BTreeNode*) x->childPtr[i];

        if(child->numKeys == MAX_KEYS) {
            debug_print("btreeInsertNonFull: child is full, splitting child...\n");
            btreeSplitChild(btree, x, i, child);
            btree->numNodes++; // only increment one in this case

            if(key->keyValue > x->key[i].keyValue) {
                i++;
                child = x->childPtr[i];
            }
        }

        return btreeInsertNonFull(btree, child, key);
    }
}



/** 
 ** Split the ith child (y), of BTreeNode x
 **/
int btreeSplitChild(
    BTree* btree
    ,BTreeNode* x
    ,const unsigned int i
    ,BTreeNode* y
) {

    debug_print("btreeSplitChild: Splitting node!\n");
    BTreeNode* z;
    z = btreeNodeAlloc();
    
    z->isLeaf = y->isLeaf;
    z->numKeys = MIN_KEYS;

    /* copy keys from node z to node z after the median key */
    int ctr = 0;
    for(ctr = 0; ctr <= MIN_KEYS-1; ctr++) {
        z->key[ctr] = y->key[ctr + MIN_DEGREE];
        /* Clear the keys being moved to node z */
        y->key[ctr + MIN_DEGREE].keyValue = 0, y->key[ctr + MIN_DEGREE].dataOffset = 0, y->key[ctr + MIN_DEGREE].dataLength = 0;
    }
    /* if node y is not a leaf, copy children from y to node z */
    if(!y->isLeaf) {
        for(ctr = 0; ctr <= MIN_DEGREE-1; ctr++) {
            z->child[ctr] = y->child[ctr + MIN_DEGREE];
            z->childPtr[ctr] = y->childPtr[ctr + MIN_DEGREE];
        }
    }

    /* update y node's key count to what it will be after the split */
    y->numKeys = MIN_DEGREE - 1; 
    /* move up each child of node x by one position, to make room for new child node */
    for(ctr = x->numKeys; ctr >= i+1; ctr--) {
        x->child[ctr + 1] = x->child[ctr];
        x->childPtr[ctr + 1] = x->childPtr[ctr];
    }
    /* set new child value to next free offset in disk (where z will be stored) */
    //x->child[i+1] = btree->nextFreeNodeOffset; //TODO: set to btree->nextFreeNodeOffset as z will be written at that postiion
    x->childPtr[i+1] = (BTreeNode*)z; 
    
    /* move up each key of node x by one position, to make room for new key */
    for(ctr = x->numKeys; ctr >= i+1; ctr--) {
        x->key[ctr] = x->key[ctr-1];
    }
    /* set new key to middle key in node y */
    x->key[i] = y->key[MIN_DEGREE - 1];
    /* increment number of keys in node x */
    x->numKeys++;




        /* Uncomment to print child splitting steps */
        /*printf("btreeSplitChild: x = [");
        int lo;
        for(lo=0; lo<x->numKeys; lo++) {
            printf("%lu,", x->key[lo].keyValue);
        }
        printf("]\n");

        printf("btreeSplitChild: y = [");
        for(lo=0; lo<y->numKeys; lo++) {
            printf("%lu,", y->key[lo].keyValue);
        }
        printf("]\n");

        printf("btreeSplitChild: z = [");
        for(lo=0; lo<z->numKeys; lo++) {
            printf("%lu,", z->key[lo].keyValue);
        }
        printf("]\n"); */

    //btreeWriteNode(y);
    //btreeWriteNode(z);
    //btreeWriteNode(x);

    return 0;
}



/** 
 ** Deletes key from BTree
 **/
int btreeDelete(
    BTree* btree
    ,off_t key
) {

   BTreeKey* deleteKey;
   if((deleteKey = btreeSearch(btree, btree->root, key)) != NULL) {
      deleteKey->isUnlinked = 1; /* Mark key as unlinked */
      return 1;
   }
   else
      return 0;

}



/************************************************
 ************************************************
 **                                           **
 **               BTree Utilities             **
 **                                           **
 ************************************************
 ************************************************/



/** 
 ** Traverses btree nodes and executes callback function on each in turn
 **/
void btreeForEach(
    BTree* btree
    ,queueNodeCallback callbackPtr
) {
    
    BTreeQueue* btreeQueue = populateBTreeQueueBFS(btree); 

    /* Iterate BTreeQueue and call callback function for each node */
    unsigned int j;
    for(j=0; j<btreeQueue->size; j++) {
        callbackPtr((btreeQueue->queue)+j);
    }

    btreeQueueFree(&btreeQueue);

}



/** 
 ** Returns a populated BTreeQueue* using BFS (Breadth-First Search)
 **/
BTreeQueue* populateBTreeQueueBFS(
    BTree* btree
) {

    unsigned int curLevel = 0; /* Distance from root node in btree */
    unsigned int i, j;

    BTreeQueue* btreeQueue = btreeQueueAlloc(btree);

    BTreeQueueNode curNode;
    setQueueNode(&curNode, btree->root, NULL, curLevel, 0);
    curNode.visited = 1;
    addQueueNode(btreeQueue, &curNode); /* Set root as first item in queue */

    BTreeQueueNode childNode;
restart:
    if( btreeQueue->size < btreeQueue->btree->numNodes) {

        /* Add children of current node to queue */
        for(i=0; i<curNode.nodePtr->numKeys+1; i++) {

            if(curNode.nodePtr->childPtr[i]) {
                setQueueNode(&childNode, curNode.nodePtr->childPtr[i], curNode.nodePtr, curLevel + 1, 0);
                addQueueNode(btreeQueue, &childNode);
                debug_print("i=%u\n", i);
            }

        }
        
        if( btreeQueue->size < btreeQueue->btree->numNodes) { /* If all nodes are under the root, queueSize = numNodes after above loop */
            for(j=0; j<btreeQueue->size; j++) { /* Iterate queue and set curNode to next unvisited node in current level */
                debug_print("Determining next unvisited node...\n");
                btreePrintNode(btreeQueue->queue[j].nodePtr, "check_queueNode", btreeQueue->queue[j].visited);
                
                if(btreeQueue->queue[j].visited == 0) { /* If QueueNode not visited, set as curNode */
                    debug_print("Found next unvisited node at queue index %u\n", j);
                    btreePrintNode(btreeQueue->queue[j].nodePtr, "found_queueNode", btreeQueue->queue[j].visited);
                    btreeQueue->queue[j].visited = 1; /* Mark node as visited so it's not selected next time */
                    setQueueNode(&curNode, btreeQueue->queue[j].nodePtr
                                        ,btreeQueue->queue[j].parentPtr
                                        ,btreeQueue->queue[j].nodeLevel
                                        ,btreeQueue->queue[j].visited);

                    if(curNode.nodeLevel == curLevel + 1)
                        curLevel++;

                    goto restart;
                }
            }
        }
        
    }

    return btreeQueue;

}



/** 
 ** Set BTreeQueueNode variables
 **/
void setQueueNode(
    BTreeQueueNode* qNode
    ,BTreeNode* nodePtr
    ,BTreeNode* nodeParentPtr
    ,unsigned int nodeLevel
    ,unsigned int visited
) {
    
    qNode->nodePtr = nodePtr;
    qNode->parentPtr = nodeParentPtr;
    qNode->nodeLevel = nodeLevel;
    qNode->visited = visited;

}



/** 
 ** Add a BTreeQueueNode to the BTreeQueue
 **/
void addQueueNode(
    BTreeQueue* btreeQueue
    ,BTreeQueueNode* qnode
) {

    btreeQueue->queue[btreeQueue->size] = *qnode;
    btreeQueue->size++;
    debug_print("\nBTreeQueueNode added at index %u! New btreeQueue->size = %u\n", btreeQueue->size-1, btreeQueue->size);
    debug_print("BTreeQueue->size = %u and BTree->numNodes = %u\n", btreeQueue->size, btreeQueue->btree->numNodes);
    btreePrintNode(qnode->nodePtr, "node", qnode->visited);

}



/** 
 ** Set the value of a BTreeKey object
 **/
void btreeSetKeyValue(
    BTreeKey* key
    ,off_t keyValue
    ,off_t dataOffset
    ,off_t dataLength
) {
    
    key->keyValue = keyValue;
    assert(key->keyValue == keyValue);

    key->dataOffset = dataOffset;
    assert(key->dataOffset == dataOffset);

    key->dataLength = dataLength;
    assert(key->dataLength == dataLength);

}



/** 
 ** Print key values of a btree node
 **/
void btreePrintNode(
    BTreeNode* node
    ,const char* label
    ,const unsigned int visited
) {

    debug_print("%s = [", label);
    int i;
    for(i=0; i<node->numKeys; i++) {
        debug_print("%lu,", node->key[i].keyValue);
    }
    if(visited){
        debug_print("] | visited = %u\n", visited);
    }
    else {
        debug_print("]\n");
    }

}



/** 
 ** Print key values of a BTreeQueueNode
 ** + callback to btreeForEach
 **/
void btreeCBPrintNode(
    BTreeQueueNode* qnode
) {

    unsigned int k;

    printf("Level %u: node = [",qnode->nodeLevel);
    for(k=0; k<qnode->nodePtr->numKeys; k++) {
        if(!qnode->nodePtr->key[k].isUnlinked)
            printf("%lu,", qnode->nodePtr->key[k].keyValue);
        else
            printf("*%lu,", qnode->nodePtr->key[k].keyValue); /* mark unlinked nodes */
    }
    printf("]\n");

}



/** 
 ** Free BTreeNode 
 ** + callback to btreeForEach
 **/
void btreeCBFreeNode(
    BTreeQueueNode* qnode
) {

    btreeNodeFree(&qnode->nodePtr);
    assert(qnode->nodePtr == NULL);

}





