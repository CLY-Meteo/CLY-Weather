#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cly-weather-headers.h"

pAVLNode createAVLTree(long Value, char * Data) {
	pAVLNode tree = malloc(sizeof(AVLNode));
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

// IMPORTANT
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

// Debug
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

void writeAVLTreeDataToFile(pAVLNode tree, FILE * outputFile, bool useReverse){
	if(tree != NULL){
		if(!useReverse){
			writeAVLTreeDataToFile(tree->leftNode, outputFile, useReverse);
			//Workaround
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