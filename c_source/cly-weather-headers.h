#include <stdio.h>
#include <stdbool.h>

//-----------------------------------------ABR------------------------------------
typedef struct ABRNode {
	long Value;
	char *Data;
	struct ABRNode *leftNode;
	struct ABRNode *rightNode;
} ABRNode;

typedef ABRNode *pABRNode;

// ABR Manipulation
void showABRPrefix(pABRNode tree);
void showABRData(pABRNode tree);

// ABR Modification
pABRNode insertInABR(pABRNode tree, long Value, char * Data);
void wipeABR(pABRNode tree);
void writeABRTreeDataToFile(pABRNode tree, FILE * outputFile, bool isReverse);







//------------------------------------ Tab
typedef struct chainedList {
	long Value;
	char *Data;
	struct chainedList *nextNode;
	struct chainedList *previousNode;
} chainedList;

typedef chainedList *pChainedList; 

pChainedList createChainedList(long Value, char * Data);
pChainedList insertInChainedList(pChainedList list, long Value, char * Data);
void writeChainedListData(pChainedList node, FILE * output, bool useReverseMode);





//------------------------------------- AVL ------------------------------------
typedef struct AVLNode {
	long Value;
	char *Data;
	struct AVLNode *leftNode;
	struct AVLNode *rightNode;
	int Balance;
} AVLNode;

typedef AVLNode *pAVLNode;

pAVLNode createAVLTree(long Value, char * Data);
pAVLNode insertInAVL(pAVLNode tree, long Value, int *h, char * Data);
void wipeAVL(pAVLNode tree);
void showAVLPrefix(pAVLNode tree);
void showAVLData(pAVLNode tree);
void writeAVLTreeDataToFile(pAVLNode tree, FILE * outputFile, bool isReverse);



//------------------------------------- Math
int max(int a, int b);
int min(int a, int b);