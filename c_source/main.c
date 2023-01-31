#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Needed for strptime
#define __USE_XOPEN
#include <time.h>

// Used for sleep, needed to look at memory usage
#include <unistd.h>

#include "cly-meteo-headers.h"

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

// ------------------------------------- Main program -------------------------------------
int main(int argc, char *argv[]) {
	//------------------------------------- Argument parsing -------------------------------------
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
			exit(1);
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
















	
	FILE * inputFile = fopen(inputFilePath, "r");
	if (inputFile == NULL){
		printf("Error: input file unreadable.\n");
		exit(2);
	}

	// Preparation
	char *line = NULL;
    size_t len = 0;
	int h = 0;

	//We prepare the storage method for the data
	pAVLNode fileAVLTree = NULL;
	pABRNode fileABRTree = NULL;
	tabList * fileTab = NULL;

	while ((getline(&line, &len, inputFile)) != -1) {
		char * lineBackup = malloc(sizeof(char) * strlen(line));
		strcpy(lineBackup, line);
		
		//Extract the first number from the line
		char * firstInfoInLine = strtok(lineBackup,";");

		strcpy(lineBackup, line);

		//This is for the hours, which aren't fully numeric.
		long int stringConvertedIntoNumber;
		if(firstInfoInLine[4] == '-'){
			struct tm tm = {0};
    		strptime(firstInfoInLine, "%Y-%m-%dT%H:%M:%S%z", &tm);
    		time_t unix_time = mktime(&tm);

			//We convert the string to a float
			stringConvertedIntoNumber = (long int) unix_time;
		} else {
			stringConvertedIntoNumber = atoi(firstInfoInLine);
		}

		//We add the data to the storage method
		switch (sortingAlgorithm){
			case AVL:
				fileAVLTree = insertInAVL(fileAVLTree, stringConvertedIntoNumber, &h, lineBackup);
				break;
			case ABR:
				fileABRTree = insertInABR(fileABRTree, stringConvertedIntoNumber, lineBackup);
				break;
			case TAB:
				//
				break;
			default:
				printf("Error: sorting algorithm not specified.\n");
				printf("This message shouldn't even appear ??\n");
				exit(4);
		}
    }

    // Close the file
    fclose(inputFile);















	//We create the output file
	FILE * outputFile = fopen(outputFilePath, "w");
	if (outputFile == NULL){
		printf("Error: output file unreadable.\n");
		exit(3);
	}

	//If the file isn't empty, we delete its content
	fseek(outputFile, 0, SEEK_END);
	if (ftell(outputFile) != 0){
		fclose(outputFile);
		outputFile = fopen(outputFilePath, "w");
	}

	//We write the data to the file
	switch (sortingAlgorithm){
		case AVL:
			writeAVLTreeDataToFile(fileAVLTree, outputFile, useReverseMode);
			break;
		case ABR:
			writeABRTreeDataToFile(fileABRTree, outputFile, useReverseMode);
			break;
		case TAB:
			//
			break;
		default:
			printf("Error: sorting algorithm not specified.\n");
			printf("This message shouldn't even appear ??\n");
			exit(1);
	}

	//I would wipe properly the memory, but this is broken because of the workaround for lines ending with 1 instead of /0.
	//wipeAVL(fileAVLTree);
	//wipeABR(fileABRTree);
	//Oh well, the memory is wiped by the OS right after anyways.

	//Insert here for tabs
	fclose(outputFile);

    return 0;
}
