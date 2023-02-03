#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cly-weather-headers.h"

// ------------------------------- ABR Modification -------------------------------
// This function will insert a new node into an ABR tree pointer.
pABRNode insertInABR(pABRNode tree, long Value, char * Data){
	if(tree == NULL){
		pABRNode NewNode = NULL;
		NewNode = malloc(sizeof(ABRNode));
		if (NewNode == NULL) {
			printf("\nMemory allocation error.\n");
			exit(4);
		}
		NewNode->Value = Value;
		NewNode->Data = Data;
		NewNode->leftNode = NULL;
		NewNode->rightNode = NULL;
		return NewNode;
	} else {
		if(Value < tree->Value){
			tree->leftNode = insertInABR(tree->leftNode, Value, Data);
		} else {
			tree->rightNode = insertInABR(tree->rightNode, Value, Data);
		}
		return tree;
	}
}

// ------------------------------------- ABR Manipulation -------------------------------------
// Used for debugging, this function will show the ABR tree's values used for sorting in prefix order.
void showABRPrefix(pABRNode tree){
	if(tree != NULL){
		printf("%d ", tree->Value);
		showABRPrefix(tree->leftNode);
		showABRPrefix(tree->rightNode);
	} else {
		printf("| ");
	}
}

// Used for debugging, this function will show the ABR tree's values used for sorting in infix order.
// This is the order in which the data will be written in the output file.
void showABRData(pABRNode tree){
	if(tree != NULL){
		showABRData(tree->leftNode);
		printf("%ld - [%s]", tree->Value,tree->Data);
		showABRData(tree->rightNode);
	}
}

// This function will complete wipe the ABR tree. Unused.
void wipeABR(pABRNode tree) {
	if(tree != NULL) {
		if(tree->Data != NULL){
			free(tree->Data);
		}

		if(tree->leftNode != NULL){
			wipeABR(tree->leftNode);
		}
		if(tree->rightNode != NULL){
			wipeABR(tree->rightNode);
		}
		free(tree);
	}
}

// This function will write the ABR tree's data in the output file.
// The data will be written in infix order. This is similar to the printABRData function.
void writeABRTreeDataToFile(pABRNode tree, FILE * outputFile, bool useReverse){
	if(tree != NULL){
		if(!useReverse){
			writeABRTreeDataToFile(tree->leftNode, outputFile, useReverse);

			//Workaround for an unknown bug where the last character of random strings is 1 instead of \0.
			if(tree->Data[strlen(tree->Data)-1] == '1'){
				tree->Data[strlen(tree->Data)-1] = '\0';
			}

			fprintf(outputFile, "%s", tree->Data);
			writeABRTreeDataToFile(tree->rightNode, outputFile, useReverse);
		} else {
			writeABRTreeDataToFile(tree->rightNode, outputFile, useReverse);
			
			//Workaround
			if(tree->Data[strlen(tree->Data)-1] == '1'){
				tree->Data[strlen(tree->Data)-1] = '\0';
			}

			fprintf(outputFile, "%s", tree->Data);
			writeABRTreeDataToFile(tree->leftNode, outputFile, useReverse);
		}
	}
}