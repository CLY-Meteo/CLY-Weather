#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#include "cly-meteo-headers.h"

pChainedListNode insertAtTop(pChainedListNode pList, int Value){
	pChainedListNode NewNode = (ChainedListNode*)malloc(sizeof(ChainedListNode));
	if (NewNode == NULL) {
		printf("\nMemory allocation error.\n");
		exit(4);
	}

	NewNode->Value = Value;
	NewNode->NextNode = pList;

	return NewNode;
}

void showList(pChainedListNode pList){
	printf("\n");
	while (pList -> NextNode != NULL) {
		printf("%d -> ", pList->Value);
		pList = pList->NextNode;
	}
	printf("%d\n", pList->Value);
}
