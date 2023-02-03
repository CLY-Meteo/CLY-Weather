#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cly-weather-headers.h"

// ------------------------------- AVL Modification -------------------------------
// This function will insert a new node into an AVL tree pointer.
pAVLNode createAVLTree(long Value, char * Data) {
	pAVLNode tree = NULL;
	tree = malloc((int)sizeof(AVLNode) + 20);
	if(tree == NULL){
		printf("\nMemory allocation error.\n");
		exit(4);
	}
	tree->Value = Value;
	tree->Data = Data;
	tree->leftNode = NULL;
	tree->rightNode = NULL;
	tree->Balance = 0;
	return tree;
}

// ------------------------------- AVL Tree Balancing Algorithm -------------------------------
pAVLNode leftRotation(pAVLNode tree) {
	if(tree->rightNode == NULL) {
		return tree;
	}

	pAVLNode pivot = tree->rightNode;
	tree->rightNode = pivot->leftNode;
	pivot->leftNode = tree;

	int balanceT = tree->Balance;
	int balanceP = pivot->Balance;

	balanceT = balanceT - max(balanceP, 0) - 1;
	balanceP = min( min(balanceT - 2, balanceT + balanceP - 2) , balanceP - 1);

	return pivot;
}

// This function is part of the AVL tree's balancing algorithm.
pAVLNode rightRotation(pAVLNode tree) {
	if(tree->leftNode == NULL) {
		return tree;
	}

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
		if(tree->rightNode == NULL) {
			return tree;
		}

		if(tree->rightNode->Balance >= 0) {
			return leftRotation(tree);
		}
		else {
			return doubleLeftRotation(tree);
		}
	}
	else if(tree->Balance <= -2) {
		if(tree->leftNode == NULL) {
			return tree;
		}

		if(tree->leftNode->Balance <= 0) {
			return rightRotation(tree);
		}
		else {
			return doubleRightRotation(tree);
		}
	} else {
		return tree;
	}
}

// This function will add a new value into the AVL tree, along with its associated data.
pAVLNode insertInAVL(pAVLNode tree, long Value, int *h, char * Data) {
	if(tree == NULL) {
		*h = 1;
		return createAVLTree(Value, Data);
	}
	else if(Value < tree->Value) {
		tree->leftNode = insertInAVL(tree->leftNode, Value, h, Data);
		*h = -*h;
	}
	else if(Value >= tree->Value) {
		tree->rightNode = insertInAVL(tree->rightNode, Value, h, Data);
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

// This function is supposed to completely wipe the AVL tree from memory. Unused.
void wipeAVL(pAVLNode tree) {
	if(tree != NULL) {
		if(tree->Data != NULL){
			free(tree->Data);
		}

		if(tree->leftNode != NULL){
			wipeAVL(tree->leftNode);
		}
		if(tree->rightNode != NULL){
			wipeAVL(tree->rightNode);
		}
		free(tree);
	}
}

// Debug functions used to show the contents of the AVL tree.
void showAVLPrefix(pAVLNode tree) {
	if(tree != NULL) {
		printf("%f ", tree->Value);
		showAVLPrefix(tree->leftNode);
		showAVLPrefix(tree->rightNode);
	} else {
		printf("| ");
	}
}
void showAVLData(pAVLNode tree){
	if(tree != NULL){
		showAVLData(tree->leftNode);
		printf("%ld - [%s]", tree->Value,tree->Data);
		showAVLData(tree->rightNode);
	}
}

// This function will write the contents of the AVL tree to a file.
void writeAVLTreeDataToFile(pAVLNode tree, FILE * outputFile, bool useReverse){
	if(tree != NULL){
		if(!useReverse){
			writeAVLTreeDataToFile(tree->leftNode, outputFile, useReverse);

			//Workaround for an unknown bug where the last character of random strings is 1 instead of \0.
			if(tree->Data[strlen(tree->Data)-1] == '1'){
				tree->Data[strlen(tree->Data)-1] = '\0';
			}
			fprintf(outputFile, "%s", tree->Data);

			writeAVLTreeDataToFile(tree->rightNode, outputFile, useReverse);
		} else {
			writeAVLTreeDataToFile(tree->rightNode, outputFile, useReverse);

			//Workaround
			if(tree->Data[strlen(tree->Data)-1] == '1'){
				tree->Data[strlen(tree->Data)-1] = '\0';
			}

			fprintf(outputFile, "%s", tree->Data);
			writeAVLTreeDataToFile(tree->leftNode, outputFile, useReverse);
		}
	}
}