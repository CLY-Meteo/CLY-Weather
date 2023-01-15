//ABRNode
typedef struct ABRNode {
	int Value;
	struct ABRNode *LeftNode;
	struct ABRNode *RightNode;
} ABRNode;

typedef ABRNode *pABRNode;

//Chained lists
typedef struct ChainedListNode {
	int Value;
	struct ChainedListNode *NextNode;
} ChainedListNode;

typedef ChainedListNode *pChainedListNode;

//AVLNode
typedef struct AVLNode {
	int Value;
	struct AVLNode *leftNode;
	struct AVLNode *rightNode;
	int Balance;
} AVLNode;