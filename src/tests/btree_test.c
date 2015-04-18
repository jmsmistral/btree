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

    // open test file for btree io tests
    /*    int fd;
    if((fd = open("/home/jonathan/dev/c/tree/src/tree_core/test.idx", ( O_WRONLY | O_CREAT | O_TRUNC ))) < 0) {
        if(errno == ENOENT) {
            printf("ERROR ENOENT\n");
            if((fd = creat("/home/jonathan/dev/c/tree/src/tree_core/test.idx", 0)) < 0) {
                printf("creat error!");
                exit(-1);
            }
        }
        else {
            printf("open error!");
            exit(-1);
        }
    } */

    /* Print data struct sizes */
    printf("\nPrint Sizes\n-----------\n");
    printf("MAX_KEYS = %d\n", MAX_KEYS);
    printf("MAX_CHILDREN = %d\n", MAX_CHILDREN);
    printf("off_t = %lu bytes\n", sizeof(off_t));
    printf("unsigned int = %lu bytes\n", sizeof(unsigned int));
    printf("void* = %lu bytes\n", sizeof(void*));
    printf("BTree = %lu bytes\n", sizeof(BTree));
    printf("BTreeNode = %lu bytes\n", sizeof(BTreeNode));
    printf("BTreeKey = %lu bytes\n\n", sizeof(BTreeKey));

    /* TEST 1: CREATE AND POPULATE BTREE */
    BTree* btree = btreeAlloc();
    BTreeKey* key = btreeKeyAlloc();

    off_t i;
    for(i=1; i<25; i++) {
      if(i==19)
         btreeSetKeyValue(key, i, 69, 69);
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

    if( (resultKey = (BTreeKey*) btreeSearch(btree, btree->root, 19)) != NULL) {
      printf("key found!\n");
      printf("keyValue = %lu\n", resultKey->keyValue);
      printf("dataOffset = %lu\n", resultKey->dataOffset);
      printf("dataLength = %lu\n", resultKey->dataLength);
    }

    /* TEST 4: DELETE KEY FROM BTREE  */
    printf("\n**test: Deleting keyValue = 19\n");
    btreeDelete(btree, 19);

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

    printf("**test: Finshed\n\n");


    return 0;
}






