# Btree #

An Btree library in C based on Btree algorithm published in Introduction to Algorithms (2nd edition), Cormen et al. p434.

I wrote this library as I couldn't find any 'easy-to-learn' code on Btrees online. The idea isn't to make this code fast and efficient, but to have the basic Btree algorithms available that allows those who are interested to see how these can be implemented. I will be writing a blog post explaining the theory and walking through the key parts of the code - will post the link here :)

### How do I get set up? ###

To compile, run **make** in the 'tests' folder as follows:


```
#!bash

$ cd btree/src/tests
$ make
```

This will compile the 'btree_test.c' file that shows how to use the library. To run the test program do: 


```
#!bash

$ ./btree_test
```

If you haven't done any modifications you should see the following output:


```
#!bash

**test: Start

Print Sizes
-----------
MAX_KEYS = 1023
MAX_CHILDREN = 1024
off_t = 8 bytes
unsigned int = 4 bytes
void* = 8 bytes
BTree = 24 bytes
BTreeNode = 49128 bytes
BTreeKey = 32 bytes

**test: Printing Btree
Level 0: node = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,]

**test: Searching for keyValue = 19
**test: searchKey allocated...
btreeSearch: i = 18
key found!
keyValue = 19
dataOffset = 69
dataLength = 69

**test: Deleting keyValue = 19
btreeSearch: i = 18
btreeSearch: i = 18
key not found!

**test: Printing Btree
Level 0: node = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,*19,20,21,22,23,24,]
**test: Finshed

```

The test program shows how to:

- Create & populate a Btree
- Print the values in a Btree
- Search a Btree for a key
- Delete a key from a Btree


### Questions ###

If you have questions, feel free to email me at: jmsmistral@gmail.com
I don't guarantee a reply! ;)