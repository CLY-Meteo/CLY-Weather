#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "cly-weather-headers.h"

// ------------------------------- Chained List Modification -------------------------------
pChainedList createChainedList(long Value, char * Data){
	pChainedList newChainedList = NULL;
	newChainedList = malloc(sizeof(chainedList));
	if (newChainedList == NULL){
		printf("\nMemory allocation error.\n");
		exit(4);
	}

	newChainedList->Value = Value;
	newChainedList->Data = Data;
	newChainedList->nextNode = NULL;
	return newChainedList;
}

//We insert in the chained list according to the order of the values.
pChainedList insertInChainedList(pChainedList list, long Value, char * Data){
	pChainedList newChainedList = NULL;
	newChainedList = createChainedList(Value, Data);

	if (list == NULL){
		return newChainedList;
	}

	pChainedList currentNode = list;
	pChainedList previousNode = NULL;

	while (currentNode != NULL && currentNode->Value <= Value){
		previousNode = currentNode;
		currentNode = currentNode->nextNode;
	}

	if (previousNode == NULL){
		newChainedList->nextNode = list;
		return newChainedList;
	}

	previousNode->nextNode = newChainedList;
	newChainedList->nextNode = currentNode;
	return list;
}

// ------------------------------------- Chained List Manipulation -------------------------------------
// Used for debugging, this function will show the chained list's values used for sorting.
void showChainedList(pChainedList list){
	if(list != NULL){
		printf("%ld - [%s]", list->Value, list->Data);
		showChainedList(list->nextNode);
	}
}
void showChainedListData(pChainedList list){
	if(list != NULL){
		printf("%s", list->Data);
		showChainedListData(list->nextNode);
	}
}

void writeChainedListData(pChainedList node, FILE * output, bool useReverseMode){
	if(node != NULL){
		//Workaround
		if(isalpha(node->Data[strlen(node->Data)-1]) || isdigit(node->Data[strlen(node->Data)-1])){
			node->Data[strlen(node->Data)-1] = '\0';
		}

		if(useReverseMode){
			writeChainedListData(node->nextNode, output, useReverseMode);
			fprintf(output, "%s", node->Data);
		} else {
			fprintf(output, "%s", node->Data);
			writeChainedListData(node->nextNode, output, useReverseMode);
		}
	}
}

// This function will complete wipe the chained list.
void wipeChainedList(pChainedList list) {
	if(list != NULL) {
		if(list->Data != NULL){
			free(list->Data);
		}
		wipeChainedList(list->nextNode);
		free(list);
	}
}