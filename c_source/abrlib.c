#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#include "cly-meteo-headers.h"

// ------------------------------------- ABR Manipulation -------------------------------------
int searchForValueInABR(pABRNode tree, int e) {
	if(tree == NULL){
		return 0;
	}

	if(tree->Value == e){
		return 1;
	}

	if(tree->Value > e){
		return searchForValueInABR(tree->leftNode, e);
	} else{
		return searchForValueInABR(tree->rightNode, e);
	}
}

void showABRPrefix(pABRNode tree){
	if(tree != NULL){
		printf("%d ", tree->Value);
		showABRPrefix(tree->leftNode);
		showABRPrefix(tree->rightNode);
	} else {
		printf("| ");
	}
}

int IsABRProperlyFormatted(pABRNode tree){
	if(tree == NULL){
		return 1;
	} else {
		if(tree->leftNode != NULL){
			if(tree->leftNode->Value > tree->Value){
				return 0;
			}
		}
		if(tree->rightNode != NULL){
			if(tree->rightNode->Value < tree->Value){
				return 0;
			}
		}
		return IsABRProperlyFormatted(tree->leftNode) && IsABRProperlyFormatted(tree->rightNode);
	}
}

// ------------------------------- ABR Modification -------------------------------
// Some optimisation could be done.
pABRNode insertABRiterative(pABRNode tree, int e){
	pABRNode temp = tree;
	pABRNode storageTemp = temp;
	while(temp != NULL){
		storageTemp = temp;
		if(temp->Value < e){
			temp = temp->leftNode;
		} else {
			temp = temp->rightNode;
		}
	}
	temp = malloc(sizeof(ABRNode));
	if(temp == NULL){
		printf("Memory allocation error.");
		exit(4);
	}
	temp->Value = e;
	temp->leftNode = NULL;
	temp->rightNode = NULL;

	if(storageTemp->Value < e){
		storageTemp->leftNode = temp;
	} else {
		storageTemp->rightNode = temp;
	}

	return tree;
}

pABRNode delMax(pABRNode ABRNode, int * ValueToRemoveFromABR){
	pABRNode tmp;

	if (ABRNode->rightNode != NULL){
		ABRNode->rightNode = delMax(ABRNode->rightNode,ValueToRemoveFromABR);
	} else {
		*ValueToRemoveFromABR = ABRNode->Value;
		tmp = ABRNode;
		ABRNode = ABRNode->leftNode;
		free(tmp);
	}
	return ABRNode;
}

pABRNode deleteElementFromABR(pABRNode ABRNode, int ValueToRemoveFromABR){
	pABRNode tmp;

	//Element not in tree
	if(ABRNode == NULL){
		return ABRNode;
	}

	//Recursion
	else if (ValueToRemoveFromABR > ABRNode->Value){
		ABRNode->rightNode = deleteElementFromABR(ABRNode->rightNode,ValueToRemoveFromABR);
	} else if (ValueToRemoveFromABR < ABRNode->Value){
		ABRNode->leftNode = deleteElementFromABR(ABRNode->leftNode,ValueToRemoveFromABR);
	}

	//It's equal.
	else if (ABRNode->leftNode == NULL){
		tmp = ABRNode;
		ABRNode = ABRNode->rightNode;
		free(tmp);
	}

	else {
		ABRNode->leftNode = delMax(ABRNode->leftNode,&ABRNode->Value);
	}

	return NULL;
}

void wipeABR(pABRNode tree) {
	if(tree != NULL) {
		wipeABR(tree->leftNode);
		wipeABR(tree->rightNode);
		free(tree);
	}
}