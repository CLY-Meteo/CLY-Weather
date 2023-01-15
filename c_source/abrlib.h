#include "common_types.h"

// ------------------------------- ABR Manipulation -------------------------------
int searchForValueInABR(pABR a, int e);

void showABRPrefix(pABR a);

int IsABRProperlyFormatted(pABR a);

// ------------------------------- ABR Modification -------------------------------
pABR insertABRrecursive(pABR a, int e);

pABR insertABRiterative(pABR a, int e);

pABR delMax(pABR ABRNode, int * ValueToRemoveFromABR);

pABR deleteElementFromABR(pABR ABRNode, int ValueToRemoveFromABR);