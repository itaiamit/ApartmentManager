#include "header.h"


DLIST sortByMaxPrice(DLIST* lstApt_Data, int maxPrice)
{
	DLIST res;
	makeEmptyList(&res);
	DLNODE * curr = lstApt_Data->head;

	while (curr != NULL)
	{
		if (curr->data->apartment.price <= maxPrice)
			insertDataToEndListDList(&res, curr->data);

		curr = curr->next;
	}

	return res;
}

DLIST sortByMinNumRooms(DLIST* lstApt_Data, short int numOfRooms)
{
	DLIST res;
	makeEmptyList(&res);
	DLNODE * curr = lstApt_Data->head;

	while (curr != NULL) {

		if (curr->data->apartment.numOfRooms >= numOfRooms)
			insertDataToEndListDList(&res, curr->data);

		curr = curr->next;
	}

	return res;
}

DLIST sortByMaxNumRooms(DLIST* lstApt_Data, short int numOfRooms)
{
	DLIST res;
	makeEmptyList(&res);
	DLNODE * curr = lstApt_Data->head;

	while (curr != NULL)
	{
		if (curr->data->apartment.numOfRooms <= numOfRooms)
			insertDataToEndListDList(&res, curr->data);

		curr = curr->next;
	}

	return res;
}

DLIST sortByDate(DLIST* lstApt_Data, TIME userDate) {

	DLIST res;
	TIME dateToCompare;
	makeEmptyList(&res);
	DLNODE * curr = lstApt_Data->head;

	while (curr != NULL)
	{
		dateToCompare = curr->data->apartment.date;

		if (isDateEarlier(userDate, dateToCompare))
			insertDataToEndListDList(&res, curr->data);

		curr = curr->next;
	}

	return res;
}

DLIST sortByLastDays(DLIST* lstApt_Data, unsigned int days)
{
	DLIST res;
	makeEmptyList(&res);
	DLNODE * curr = lstApt_Data->head;
	time_t aptTime, currTime;
	double seconds;

	time(&currTime);

	while (curr != NULL)
	{
		aptTime = curr->data->enter_date_to_data;
		seconds = difftime(currTime, aptTime);

		if (isBetweenDates(days, seconds))
			insertDataToEndListDList(&res, curr->data);

		curr = curr->next;
	}

	return res;
}

BOOL isDateEarlier(TIME userDate, TIME dateToCompare)
{
	if (userDate.year > dateToCompare.year)
		return TRUE;

	if (userDate.year < dateToCompare.year)
		return FALSE;

	if (userDate.month > dateToCompare.month)
		return TRUE;

	if (userDate.month < dateToCompare.month)
		return FALSE;

	if (userDate.day > dateToCompare.day)
		return TRUE;

	if (userDate.day < dateToCompare.day)
		return FALSE;

	return FALSE; // it is the same date.
}

BOOL isBetweenDates(unsigned int days, double seconds) {

	if (seconds / SECONDS_TO_DAY <= days)
		return TRUE;

	else
		return FALSE;
}

void purchasedAptByCode(DLIST* lstApt_Data, int Acode) {

	DLNODE * curr = lstApt_Data->head;
	BOOL firstTime = TRUE;
	BOOL found = FALSE;

	while (curr->next != NULL)
	{
		if (curr->data->apartment.aptCode == Acode)
		{
			if (firstTime) {// the node is first in the list

				curr->next->prev = NULL;
				lstApt_Data->head = curr->next;
			}

			else {// the node in the middle of the list
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}

			found = TRUE;
			//free(curr->data->apartment.address); //  ìú÷ïïïïï
			free(curr);
			break;
		}

		curr = curr->next;
		firstTime = FALSE;
	}

	if (!found) {// the node is last in the list

		curr->prev->next = NULL;
		lstApt_Data->tail = curr->prev;
		free(curr->data->apartment.address);
		free(curr);
	}
}

void deleteAptByLastDays(DLIST* lstApt_Data, unsigned int days)
{
	time_t aptTime, currTime;
	double seconds;
	DLNODE * curr = lstApt_Data->head;

	time(&currTime);

	while (curr != NULL)
	{
		aptTime = curr->data->enter_date_to_data;
		seconds = difftime(currTime, aptTime);

		if (isBetweenDates(days, seconds))
		{
			if (curr->prev == NULL && curr->next == NULL)
			{
				makeEmptyList(lstApt_Data);
				//free(curr->data->apartment.address); // ìú÷ïïï
				free(curr);
				curr = NULL;
			}

			else if (curr->prev == NULL) // it is the first node
			{
				curr = curr->next;
				lstApt_Data->head = curr;
				//free(curr->prev->data->apartment.address); // ìú÷ïïï
				free(curr->prev);
				curr->prev = NULL;

			}

			else if (curr->next == NULL) // it is the last node
			{
				curr = curr->prev;
				lstApt_Data->tail = curr;
				//free(curr->next->data->apartment.address);// ìú÷ïïï
				free(curr->next);
				curr->next = NULL;
			}

			else // this node is in the middle
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				curr = curr->next;
				//free(curr->data->apartment.address);// ìú÷ïïï
				free(curr->prev);
			}
		}

		else
			curr = curr->next;
	}
}

void addApartment(DLIST* lstApt_Data, char* detalis)
{
	APT apt;
	APT_DATA_BASE dataApt;
	char* temp;
	DLNODE* curr = lstApt_Data->head;

	temp = strtok(detalis, "\"");
	apt.address = temp;

	temp = strtok(NULL, " ");
	apt.price = atoi(temp);

	temp = strtok(NULL, " ");
	apt.numOfRooms = atoi(temp);

	temp = strtok(NULL, " ");
	apt.date.day = atoi(temp);

	temp = strtok(NULL, " ");
	apt.date.month = atoi(temp);

	temp = strtok(NULL, " ");
	apt.date.year = atoi(temp);

	apt.aptCode = runningAptCode;
	dataApt.apartment = apt;
	runningAptCode++;// move forward the counter AptCode


	if (isEmptyList(lstApt_Data))
		insertAfter(lstApt_Data, curr, &dataApt);

	else if (curr->next == NULL)
	{
		if (curr->data->apartment.price > apt.price)
			insertDataToStartListDList(lstApt_Data, &dataApt);
		else
			insertDataToEndListDList(lstApt_Data, &dataApt);
	}

	else
	{
		while (curr->next != NULL)
		{
			if (curr->data->apartment.price >= apt.price) {
				curr = curr->prev;
				break;
			}

			curr = curr->next;
		}

		insertAfter(lstApt_Data, curr, &dataApt);
	}

}

void printList(DLIST* lstApt_Data, BOOL lowToHigh)
{
	DLNODE* curr;
	TIME date;


	if (lowToHigh)
		curr = lstApt_Data->head;

	else
		curr = lstApt_Data->tail;

	while (curr != NULL)
	{
		date = curr->data->apartment.date;
		printf("Apt details:\n");
		printf("Code: %d\n", curr->data->apartment.aptCode);
		printf("Address: %s\n", curr->data->apartment.address);
		printf("Number of rooms: %hd\n", curr->data->apartment.numOfRooms);
		printf("Price: %d\n", curr->data->apartment.price);
		printf("Entry date: %hd.%hd.%hd\n", date.day, date.month, date.year);
		printf("Database entry date: %d.%d.%d\n\n", curr->data->time.day, curr->data->time.month, curr->data->time.year);

		if (lowToHigh)
			curr = curr->next;

		else
			curr = curr->prev;
	}
}

void printAptCodes(DLIST* lstApt_Data)
{
	DLNODE* curr = lstApt_Data->head;

	printf("The apartments codes are:\n");

	while (curr != NULL) {

		printf("Apartments #%d\n", curr->data->apartment.aptCode);
		curr = curr->next;
	}
}

void makeEmptyShortTermHistoryList(char** short_term_history)
{
	int i;

	for (i = 0; i < N; i++)
		short_term_history[i] = NULL;
}

void insertCommandToShortTermHistory(char** short_term_history, char* str, HLIST* history)
{
	int length;

	length = strlen(str);


	if (short_term_history[N - 1] != NULL)
	{
		cmdNumUpdate(history);
		insertDataToStartHistoryList(history, short_term_history[N - 1], 8);
		advanceCommandToNextPlace(short_term_history);
	}

	else if (short_term_history[0] != NULL)
		advanceCommandToNextPlace(short_term_history);

	short_term_history[0] = (char*)malloc((length + 1) * sizeof(char));
	strcpy(short_term_history[0], str);
	short_term_history[0][length] = '\0';
}

void advanceCommandToNextPlace(char** short_term_history)
{
	int i;
	char* saverPrev, *saverNext;

	saverPrev = short_term_history[0];

	for (i = 0; short_term_history[i] != NULL && i < N - 1; i++)
	{
		saverNext = short_term_history[i + 1];
		short_term_history[i + 1] = saverPrev;
		saverPrev = saverNext;
	}
}

void getCommand(DLIST* lstApt_Data, char* str, char** short_term_history, HLIST* history)
{
	char* command, *temp;
	BOOL advance = TRUE;

	temp = strdup(str);

	if (temp[0] == '!')
	{
		restoreCommands(lstApt_Data, str, temp, short_term_history, history);
		advance = FALSE;
	}

	else if (temp[0] == 's') // short history
	{
		printShortTermHistory(short_term_history);
		advance = FALSE;
	}

	else if (temp[0] == 'h') // history
	{
		printShortTermHistory(short_term_history);
		printHistoryList(history);
		advance = FALSE;
	}

	if (advance)
	{
		command = strtok(temp, "-");

		if (command[0] == 'a') // add-apt
		{
			command = strtok(NULL, " ");
			command = strtok(NULL, "\0");
			addApartment(lstApt_Data, command);
			insertCommandToShortTermHistory(short_term_history, str, history);
		}

		else if (command[0] == 'b') // buy-apt
		{
			command = strtok(NULL, " ");
			command = strtok(NULL, "\0");
			purchasedAptByCode(lstApt_Data, atoi(command));
			insertCommandToShortTermHistory(short_term_history, str, history);
		}

		else if (command[0] == 'd') // delete-apt
		{
			command = strtok(NULL, " ");
			command = strtok(NULL, " ");
			command = strtok(NULL, "\0");
			deleteAptByLastDays(lstApt_Data, atoi(command));
			insertCommandToShortTermHistory(short_term_history, str, history);
		}

		else if (command[0] == 'f')
			findCommands(lstApt_Data, str, command, short_term_history, history);
	}

	free(temp);
}

char * getStrFromHistory(HLIST * history, char** short_term_history, int num)
{
	HNODE * curr = history->head;

	if (num > 7)
	{
		while (curr->cmdNum != num)
			curr = curr->next;


		return curr->data;
	}

	else
		return short_term_history[num - 1];
}

void printHistoryList(HLIST * history)
{
	HNODE* curr;

	curr = history->head;

	while (curr != NULL)
	{
		printf("#%d %s\n", curr->cmdNum, curr->data);
		curr = curr->next;
	}
}

void printShortTermHistory(char** short_term_history)
{
	for (int i = 0; i < N && short_term_history[i] != NULL; i++)
		printf("#%d %s\n", (i + 1), short_term_history[i]);
}

void restoreCommands(DLIST* lstApt_Data, char* str, char* temp, char** short_term_history, HLIST* history)
{
	char *str1, *str2, *ptr, *nCommand;
	int command;

	if (temp[1] == '!')
	{
		if (short_term_history[0] == NULL)
			return;
		else
			getCommand(lstApt_Data, short_term_history[0], short_term_history, history);
	}

	else
	{
		command = strtol(temp + 1, &ptr, 10);
		str1 = strtok(temp, "^");
		str1 = strtok(NULL, "^");

		if (str1 == NULL)
		{
			if (command <= N)
				getCommand(lstApt_Data, short_term_history[command - 1], short_term_history, history);

			else
			{
				str1 = getStrFromHistory(history, short_term_history, command);
				getCommand(lstApt_Data, str1, short_term_history, history);
			}
		}

		else
		{
			str2 = strtok(NULL, "^");
			str1 = getStrFromHistory(history, short_term_history, command);
			nCommand = (char*)malloc(strlen(str1) * sizeof(char));
			strcpy(nCommand, str1);
			nCommand = strtok(nCommand, "123456789");
			nCommand[strlen(str1)] = '\0';
			nCommand = (char*)realloc(nCommand, (strlen(str2) + strlen(str1)) * sizeof(char));
			strcat(nCommand, str2);
			getCommand(lstApt_Data, nCommand, short_term_history, history);
			free(nCommand);
		}
	}


}

void findCommands(DLIST* lstApt_Data, char* str, char* command, char** short_term_history, HLIST* history)
{
	DLIST smallList;
	TIME date;
	short int minRooms = 0, maxRooms = 0;
	int price = 0;
	BOOL up = TRUE;

	makeEmptyList(&smallList);

	date.day = date.month = date.year = 0;

	command = strtok(NULL, "-");
	command = strtok(NULL, " ");

	if (strcmp(command, "Enter") == 0)
	{
		command = strtok(NULL, "\0");
		smallList = sortByLastDays(lstApt_Data, atoi(command));
		printAptCodes(&smallList);
		insertCommandToShortTermHistory(short_term_history, str, history);
	}

	else
	{
		while (command != NULL)
		{
			if (strcmp(command, "MaxPrice") == 0)
			{
				command = strtok(NULL, "-\0");
				price = atoi(command);
			}

			if (strcmp(command, "MinNumRooms") == 0)
			{
				command = strtok(NULL, "-\0");
				minRooms = atoi(command);
			}

			if (strcmp(command, "MaxNumRooms") == 0)
			{
				command = strtok(NULL, "-\0");
				maxRooms = atoi(command);
			}

			if (strcmp(command, "Date") == 0)// it is date
			{
				command = strtok(NULL, "-\0");
				date.day = (command[0]-'0') * 10 + (command[1]-'0');
				date.month = (command[2] - '0') * 10 + (command[3] - '0');
				date.year = (command[6] - '0') * 10 + (command[7] - '0');
			}

			if (strcmp(command, "sr") == 0)
			{
				command = strtok(NULL, "\0");
				up = FALSE;
			}

			else if (strcmp(command, "s") == 0)
				command = strtok(NULL, "\0");

			command = strtok(NULL, " -\0");

		} //while

		insertCommandToShortTermHistory(short_term_history, str, history);
		smallList = makeAsmallSortedList(lstApt_Data, price, minRooms, maxRooms, date);
		printList(&smallList, up);
	}

	freeDlist(&smallList);
}

DLIST makeAsmallSortedList(DLIST* lstApt_Data, int price, int minRooms, int maxRooms, TIME date)
{
	DLIST smallList;
	BOOL firstTime = TRUE;

	makeEmptyList(&smallList);

	if (price != 0)
	{
		smallList = sortByMaxPrice(lstApt_Data, price);
		firstTime = FALSE;
	}

	if (minRooms != 0)
	{
		if (firstTime)
		{
			smallList = sortByMinNumRooms(lstApt_Data, minRooms);
			firstTime = FALSE;
		}
		else
			smallList = sortByMinNumRooms(&smallList, minRooms);
	}

	if (maxRooms != 0)
	{
		if (firstTime)
		{
			smallList = sortByMaxNumRooms(lstApt_Data, maxRooms);
			firstTime = FALSE;
		}
		else
			smallList = sortByMaxNumRooms(&smallList, maxRooms);
	}

	if (date.year != 0)
	{
		if (firstTime)
		{
			smallList = sortByDate(lstApt_Data, date);
			firstTime = FALSE;
		}
		else
			smallList = sortByDate(&smallList, date);
	}

	return smallList;
}

TIME convertTMtoTIME(TM* tm)
{
	TIME res;

	res.day = tm->tm_mday;
	res.month = tm->tm_mon + 1;
	res.year = tm->tm_year - 100;

	return res;
}

void convertTIMEToTime_t(APT_DATA_BASE* data)
{
	TM tm;

	tm.tm_year = data->time.year + 100;
	tm.tm_mon = data->time.month - 1;
	tm.tm_mday = data->time.day;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 1;
	tm.tm_isdst = -1;

	data->enter_date_to_data = mktime(&tm);
}

char* getInput(int* longestStr)
{
	char* input;
	char ch;
	int logSize = 0, phsSize = 1;
	BOOL endOfInput = FALSE;

	input = (char*)malloc(phsSize * sizeof(char*));

	while (!endOfInput)
	{
		ch = getchar();
		if (ch == '\n')
			endOfInput = TRUE;

		else
		{
			if (logSize == phsSize)
			{
				phsSize *= 2;
				input = (char*)realloc(input, phsSize * sizeof(char*));
			}

			input[logSize] = ch;
			logSize++;
		}
	}

	input = (char*)realloc(input, (logSize + 1) * sizeof(char*));
	input[logSize] = '\0';

	if (strlen(input) > *longestStr)
		*longestStr = strlen(input);

	return input;
}

void printPrompt()
{
	printf("\nPlease insert your desired command.\n\n");

	printf("1) find-apt -MaxPrice <price>\n"); // command
											   //(All the apartments with maximum price <price>// description

	printf("2) find-apt -MinNumRooms <number>\n"); // command
												   //(All the apartments with minimum rooms <num> // description

	printf("3) find-apt -MaxNumRooms <number>\n"); // command
												   //(All the apartments with minimum rooms <num>) // description

	printf("4) find-apt -Date <dd><mm><yyyy>\n"); // command
												  //(All the apartments that evacuate until the date <date>) // description

												  //For each commands of the above, you can also add "-s" or "-sr" to the end.
												  //"-s" - Will be shown the requsted data sorted by lowest to highest price.
												  //"-sr" - Will be shown the requsted data sorted by highest to lowest price.

	printf("5) add-apt <\"address\"> <price> <numOfRooms> <dd> <mm> <yy>\n"); // command
																			  //printf("(Adding apartment to the data base)\n\n"); // description

	printf("6) buy-apt <number>\n"); // command
									 //(Indicates that an apartment <code> was bought) // description

	printf("7) find-apt -Enter <x>\n"); // command
										//(Will be printed all the apartments codes that have ne entered to the data base in the last <x> days) // description

	printf("8) delete-apt -Enter <x>\n"); // command
										  //(Will be deleted all the apartments that have ne entered to the data base in the last <x> days) // description

	printf("9) !!\n"); // command
					   //(Runs the last command that has been entered) // description

	printf("10) short_history\n"); // command
								   //(Will be printed the last 7 commands) // description

	printf("11) history\n"); // command
							 //(Will be printed all the last commands) // description

	printf("12) !<number>\n"); // command
							   //(Runs the last command that has been entered) // description

	printf("13) !<number>^str1^str2\n\n"); // command
										   //(Runs command #<num> that contains substring <str1> by substring <str2>) // description
}

void freeDlist(DLIST* lst)
{
	DLNODE * curr, *saver;

	curr = lst->head;

	while (curr != NULL)
	{
		saver = curr->next;
		free(curr->data->apartment.address);
		free(curr->data);
		free(curr);
		curr = saver;
	}
}

void freeHlist(HLIST* lst)
{
	HNODE * curr, *saver;

	curr = lst->head;

	while (curr != NULL)
	{
		saver = curr->next;
		free(curr->data);
		free(curr);
		curr = saver;
	}
}

void freeShort_Term_History(char** short_term_history)
{
	for (int i = 0; i < N; i++)
		free(short_term_history[i]);
}