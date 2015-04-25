# Btree #

An Btree library in C based on Btree algorithm published in Introduction to Algorithms (2nd edition), Cormen et al. p434.

I wrote this library as I couldn't find any 'easy-to-learn' code on Btrees online. The idea isn't to make this code fast and efficient, but to have the basic Btree algorithms available that allows those interested to see how these can be implemented. I will be writing a blog post explaining the theory and walking through the key parts of the code - will post the link here :)

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
MAX_KEYS = 11
MAX_CHILDREN = 12
BTree = 24 bytes
BTreeNode = 552 bytes
BTreeKey = 32 bytes

**test: Printing Btree
Level 0: node = [36,]
Level 1: node = [6,12,18,24,30,]
Level 1: node = [42,48,54,60,66,]
Level 2: node = [1,2,3,4,5,]
Level 2: node = [7,8,9,10,11,]
Level 2: node = [13,14,15,16,17,]
Level 2: node = [19,20,21,22,23,]
Level 2: node = [25,26,27,28,29,]
Level 2: node = [31,32,33,34,35,]
Level 2: node = [37,38,39,40,41,]
Level 2: node = [43,44,45,46,47,]
Level 2: node = [49,50,51,52,53,]
Level 2: node = [55,56,57,58,59,]
Level 2: node = [61,62,63,64,65,]
Level 2: node = [67,68,69,70,71,72,73,74,]

**test: Searching for keyValue = 19
**test: searchKey allocated...
btreeSearch: i = 0
btreeSearch: i = 3
btreeSearch: i = 0
key found!
keyValue = 19
dataOffset = 69
dataLength = 69

**test: Deleting keyValue = 19
btreeSearch: i = 0
btreeSearch: i = 3
btreeSearch: i = 0
btreeSearch: i = 0
btreeSearch: i = 3
btreeSearch: i = 0
key not found!

**test: Printing Btree
Level 0: node = [36,]
Level 1: node = [6,12,18,24,30,]
Level 1: node = [42,48,54,60,66,]
Level 2: node = [1,2,3,4,5,]
Level 2: node = [7,8,9,10,11,]
Level 2: node = [13,14,15,16,17,]
Level 2: node = [*19,20,21,22,23,]
Level 2: node = [25,26,27,28,29,]
Level 2: node = [31,32,33,34,35,]
Level 2: node = [37,38,39,40,41,]
Level 2: node = [43,44,45,46,47,]
Level 2: node = [49,50,51,52,53,]
Level 2: node = [55,56,57,58,59,]
Level 2: node = [61,62,63,64,65,]
Level 2: node = [67,68,69,70,71,72,73,74,]

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