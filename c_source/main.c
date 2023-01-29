#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef enum sort_algorithm {
	Unspecified = 0,
	AVL = 1,
	ABR = 2,
	TAB = 3
} sort_algorithm;

bool testArgument(char * argument, char * argumentExpected){
	for (int i = 0; i < strlen(argumentExpected); i++){
		if (argument[i] != argumentExpected[i]){
			return false;
		}
	}
	return true;
}

int main(int argc, char *argv[]) {
	bool inputFileArgumentSpecified = false;
	bool outputFileArgumentSpecified = false;
	bool useReverseMode = false;
	sort_algorithm sortingAlgorithm = Unspecified;
	char * inputFilePath;
	char * outputFilePath;

	bool nextArgumentShouldBeOutputFile = false;
	bool nextArgumentShouldBeInputFile = false;
	for (int i = 1; i < argc; i++) {
		if (nextArgumentShouldBeInputFile){
			inputFilePath = argv[i];
			nextArgumentShouldBeInputFile = false;
		} else if (nextArgumentShouldBeOutputFile){
			outputFilePath = argv[i];
			nextArgumentShouldBeOutputFile = false;
		}

		else if (testArgument(argv[i], "-f")){
			if(inputFileArgumentSpecified){
				printf("Error: input file already specified.\n");
				exit(1);
			}

			nextArgumentShouldBeInputFile = true;
			inputFileArgumentSpecified = true;
		} else if (testArgument(argv[i], "-o")){
			if(outputFileArgumentSpecified){
				printf("Error: output file already specified.\n");
				exit(1);
			}

			nextArgumentShouldBeOutputFile = true;
			outputFileArgumentSpecified = true;
		}
		
		else if (testArgument(argv[i], "-r")){
			if(useReverseMode){
				printf("Error: reverse mode already active.\n");
				exit(1);
			}

			useReverseMode = true;
		}

		//Switch only works with integers. I know it's ugly, but I have no choice.
		else if (testArgument(argv[i], "--abr")){
			if(sortingAlgorithm != Unspecified){
				printf("Error: sorting algorithm already specified.\n");
				exit(1);
			}

			sortingAlgorithm = ABR;
		} else if (testArgument(argv[i], "--tab")){
			if(sortingAlgorithm != Unspecified){
				printf("Error: sorting algorithm already specified.\n");
				exit(1);
			}
			
			sortingAlgorithm = TAB;
		} else if (testArgument(argv[i], "--avl")){
			if(sortingAlgorithm != Unspecified){
				printf("Error: sorting algorithm already specified.\n");
				exit(1);
			}

			sortingAlgorithm = AVL;
		}

		else {
			printf("Unknown argument: %s", argv[i]);
			return 1;
		}
	}

	if (!inputFileArgumentSpecified){
		printf("Error: input file not specified.\n");
		exit(1);
	} else if (!outputFileArgumentSpecified){
		printf("Error: output file not specified.\n");
		exit(1);
	} else if (sortingAlgorithm == Unspecified){
		sortingAlgorithm = AVL;
	}
					
    return 0;
}
