/**
 ** 
 ** btree interface tests
 **
 **/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>

#include "../btree.h"

int main (int argc, char* argv[]) {

    printf("**test: Start\n");

    /* Print data struct sizes */
    printf("\nPrint Sizes\n-----------\n");
    printf("MAX_KEYS = %d\n", MAX_KEYS);
    printf("MAX_CHILDREN = %d\n", MAX_CHILDREN);
    printf("BTree = %lu bytes\n", sizeof(BTree));
    printf("BTreeNode = %lu bytes\n", sizeof(BTreeNode));
    printf("BTreeKey = %lu bytes\n\n", sizeof(BTreeKey));

    /* TEST 1: CREATE AND POPULATE BTREE */
    BTree* btree = btreeAlloc();
    BTreeKey* key = btreeKeyAlloc();

    off_t i;
    for(i=1; i<75; i++) { // Insert 74 keys into Btree
      if(i==19) // Used to test search below 
         btreeSetKeyValue(key, i, 69, 69); // 69's here are offsets that can be used to index data in a file, for example
      else
         btreeSetKeyValue(key, i, 0, 0);

      debug_print("\n**test: Inserting value = %lu\n", i);
      if(btreeInsert(btree, key) == -1) {
          printf("test: btreeInsert error\n");
          exit(-1);
      };
    }

    btreeKeyFree(&key);


    /* TEST 2: PRINT BTREE */
    printf("**test: Printing Btree\n");
    btreeForEach(btree, btreeCBPrintNode); /* Print btree nodes */


    /* TEST 3: SEARCH BTREE FOR KEY */
    printf("\n**test: Searching for keyValue = 19\n");
    BTreeKey* resultKey;
    printf("**test: searchKey allocated...\n");

    if( (resultKey = (BTreeKey*) btreeSearch(btree, btree->root, 19)) != NULL) { // To search for a key, pass the btree, ptr to root node, and key value
      printf("key found!\n");
      printf("keyValue = %lu\n", resultKey->keyValue);
      printf("dataOffset = %lu\n", resultKey->dataOffset);
      printf("dataLength = %lu\n", resultKey->dataLength);
    }
    else
      printf("key not found!\n");

    /* TEST 4: DELETE KEY FROM BTREE  */
    printf("\n**test: Deleting keyValue = 19\n");
    btreeDelete(btree, 19);

    printf("**test: Searching for keyValue = 19\n");
    if( (resultKey = (BTreeKey*) btreeSearch(btree, btree->root, 19)) != NULL) {
      printf("key found!\n");
      printf("keyValue = %lu\n", resultKey->keyValue);
      printf("dataOffset = %lu\n", resultKey->dataOffset);
      printf("dataLength = %lu\n", resultKey->dataLength);
    }
    else
      printf("key not found!\n");

    printf("\n**test: Printing Btree\n");
    btreeForEach(btree, btreeCBPrintNode); /* deleted key should be marked */

    btreeFree(&btree);

    printf("\n**test: Finished\n\n");


    return 0;
}






