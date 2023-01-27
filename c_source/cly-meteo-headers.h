//-----------------------------------------ABR------------------------------------
typedef struct ABRNode {
	int Value;
	struct ABRNode *leftNode;
	struct ABRNode *rightNode;
} ABRNode;

typedef ABRNode *pABRNode;

// ABR Manipulation
int searchForValueInABR(pABRNode tree, int e);
void showABRPrefix(pABRNode tree);
int IsABRProperlyFormatted(pABRNode tree);

// ABR Modification
pABRNode insertABRiterative(pABRNode tree, int e);
pABRNode deleteElementFromABR(pABRNode ABRNode, int ValueToRemoveFromABR);
void wipeABR(pABRNode tree);







//------------------------------------ Chained lists
typedef struct ChainedListNode {
	int Value;
	struct ChainedListNode *NextNode;
} ChainedListNode;

typedef ChainedListNode *pChainedListNode;

//UNFINISHED
ChainedListNode * insertAtTop(ChainedListNode* pChainedListNode, int Value);
void showList(ChainedListNode * pChainedListNode);






//------------------------------------- AVL ------------------------------------
typedef struct AVLNode {
	int Value;
	struct AVLNode *leftNode;
	struct AVLNode *rightNode;
	int Balance;
} AVLNode;

typedef AVLNode *pAVLNode;


pAVLNode createAVLTree(int Value);
pAVLNode insertInAVL(pAVLNode tree, int Value, int *h);
pAVLNode deleteValueFromAVL(pAVLNode tree, int Value, int *h);
void wipeAVL(pAVLNode tree);





//------------------------------------- Math
int max(int a, int b);
int min(int a, int b);