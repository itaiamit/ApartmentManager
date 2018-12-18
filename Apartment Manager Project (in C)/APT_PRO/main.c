#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"


void main(int argc, char** argv)
{
	DLIST lstApt_Data;
	HLIST  history;
	char * input , *short_term_history[N];
	int longestStr = 0;
	runningAptCode = 1;

	makeEmptyHList(&history);
	makeEmptyList(&lstApt_Data);
	makeEmptyShortTermHistoryList(short_term_history);

	readDetails(argv[3], &longestStr);// gets the running apartments codes and the length of the longest command.
	readCommandsFromFile(argv[1], short_term_history, &history, longestStr);
	readApartmentsFromBinFile(argv[2], &lstApt_Data);

	printPrompt();
	input = getInput(&longestStr);

	while (strcmp(input,"exit") != 0)
	{
		printf("----------------------------------------------------------\n");
		printf(">>%s\n\n", input);
		getCommand(&lstApt_Data, input, short_term_history, &history);
		printf("----------------------------------------------------------\n");
		printPrompt();
		input = getInput(&longestStr);
	}
	printf("Good Bye!\n");

	saveDetails(argv[3], longestStr);
	saveCommandsToFile(argv[1], short_term_history, &history);
	saveApartmentsToBinFile(argv[2], &lstApt_Data);

	free(input);
	freeDlist(&lstApt_Data);
	freeHlist(&history);
	freeShort_Term_History(short_term_history);
}

