#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#include "cly-meteo-headers.h"

pAVLNode createAVLTree(int Value) {
	pAVLNode tree = malloc(sizeof(AVLNode));
	if(tree == NULL){
		printf("\nMemory allocation error.\n");
		exit(4);
	}
	tree->Value = Value;
	tree->leftNode = NULL;
	tree->rightNode = NULL;
	tree->Balance = 0;
	return tree;
}

// IMPORTANT
pAVLNode leftRotation(pAVLNode tree) {
	pAVLNode pivot = tree->rightNode;
	tree->rightNode = pivot->leftNode;
	pivot->leftNode = tree;

	int balanceT = tree->Balance;
	int balanceP = pivot->Balance;

	balanceT = balanceT - max(balanceP, 0) - 1;
	balanceP = min( min(balanceT - 2, balanceT + balanceP - 2) , balanceP - 1);

	return pivot;
}

pAVLNode rightRotation(pAVLNode tree) {
	pAVLNode pivot = tree->leftNode;
	tree->leftNode = pivot->rightNode;
	pivot->rightNode = tree;

	int balanceT = tree->Balance;
	int balanceP = pivot->Balance;

	balanceT = balanceT - min(balanceP, 0) + 1;
	balanceP = max( max( balanceT + 2 , balanceT + balanceP + 2) , balanceP + 1);

	return pivot;
}

pAVLNode doubleLeftRotation(pAVLNode tree) {
	tree->rightNode = rightRotation(tree->rightNode);
	return leftRotation(tree);
}

pAVLNode doubleRightRotation(pAVLNode tree) {
	tree->leftNode = leftRotation(tree->leftNode);
	return rightRotation(tree);
}

pAVLNode balanceAVL(pAVLNode tree) {
	if(tree->Balance >= 2) {
		if(tree->rightNode->Balance >= 0) {
			return leftRotation(tree);
		}
		else {
			return doubleLeftRotation(tree);
		}
	}
	else if(tree->Balance <= -2) {
		if(tree->leftNode->Balance <= 0) {
			return rightRotation(tree);
		}
		else {
			return doubleRightRotation(tree);
		}
	}
}

pAVLNode insertInAVL(pAVLNode tree, int Value, int *h) {
	if(tree == NULL) {
		*h = 1;
		return createAVLTree(Value);
	}
	else if(Value < tree->Value) {
		tree->leftNode = insertInAVL(tree->leftNode, Value, h);
		*h = -*h;
	}
	else if(Value > tree->Value) {
		tree->rightNode = insertInAVL(tree->rightNode, Value, h);
	}
	else {
		*h = 0;
		return tree;
	}

	if(*h != 0){
		tree->Balance += *h;
		tree = balanceAVL(tree);
		if(tree->Balance == 0) {
			*h = 0;
		}
		else {
			*h = 1;
		}
	}

	return tree;
}

pAVLNode delMinAVL(pAVLNode tree, int *h, int *element) {
	if(tree->leftNode == NULL) {
		*element = tree->Value;
		*h = -1;
		pAVLNode treeTemp = tree;
		tree = tree->rightNode;
		free(treeTemp);
		return(tree);
	}
	else {
		tree->leftNode = delMinAVL(tree->leftNode, h, element);
		*h = -*h;
	}
	if(*h != 0) {
		tree->Balance += *h;
		tree = balanceAVL(tree);
		if(tree->Balance == 0) {
			*h = -1;
		}
		else {
			*h = 0;
		}
	}
	return tree;
}

pAVLNode deleteValueFromAVL(pAVLNode tree, int Value, int *h) {
	if(tree == NULL) {
		*h = 0;
		return NULL;
	}
	else if(Value < tree->Value) {
		tree->leftNode = deleteValueFromAVL(tree->leftNode, Value, h);
		*h = -*h;
	}
	else if(Value > tree->Value) {
		tree->rightNode = deleteValueFromAVL(tree->rightNode, Value, h);
	}
	else if(tree->rightNode != NULL) {

	}
	else {
		pAVLNode treeTemp = tree;
		tree = tree->leftNode;
		free(treeTemp);
		*h = -1;
	}

	if(*h != 0) {
		tree->Balance += *h;
		tree = balanceAVL(tree);
		if(tree->Balance == 0) {
			*h = 0;
		}
		else {
			*h = 1;
		}
	}

	return tree;
}

void wipeAVL(pAVLNode tree) {
	if(tree != NULL) {
		wipeAVL(tree->leftNode);
		wipeAVL(tree->rightNode);
		free(tree);
	}
}