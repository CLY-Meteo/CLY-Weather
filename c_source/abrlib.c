#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cly-meteo-headers.h"

// ------------------------------- ABR Modification -------------------------------
// Some optimisation could be done.
pABRNode insertInABR(pABRNode tree, long Value, char * Data){
	if(tree == NULL){
		pABRNode NewNode = malloc(sizeof(ABRNode));
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
void showABRPrefix(pABRNode tree){
	if(tree != NULL){
		printf("%d ", tree->Value);
		showABRPrefix(tree->leftNode);
		showABRPrefix(tree->rightNode);
	} else {
		printf("| ");
	}
}
void showABRData(pABRNode tree){
	if(tree != NULL){
		showABRData(tree->leftNode);
		printf("%ld - [%s]", tree->Value,tree->Data);
		showABRData(tree->rightNode);
	}
}

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

void writeABRTreeDataToFile(pABRNode tree, FILE * outputFile, bool useReverse){
	if(tree != NULL){
		if(!useReverse){
			writeABRTreeDataToFile(tree->leftNode, outputFile, useReverse);
			//Workaround
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