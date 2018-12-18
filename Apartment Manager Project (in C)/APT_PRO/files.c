#include "header.h"


void readCommandsFromFile(char* fileName, char** short_term_history, HLIST* history, int longestStr)
{
	FILE* fpCmd;
	char* command;
	long int size;
	int i, cmdSize, cmdNum = 8;


	command = (char*)malloc((longestStr + 3) * sizeof(char));

	fpCmd = fopen(fileName, "r");

	if (fpCmd != NULL) // checks if the file does not exists
	{
		fseek(fpCmd, 0, SEEK_END);
		size = ftell(fpCmd);
		fseek(fpCmd, 0, SEEK_SET);


		for (i = 0; i < N && ftell(fpCmd) < size; i++) // if there are 7 or less commands
		{
			fgets(command, longestStr + 3, fpCmd);
			cmdSize = strlen(command);
			command[cmdSize - 1] = '\0';
			cmdSize = strlen(command);
			short_term_history[i] = (char*)malloc((cmdSize + 1) * sizeof(char));
			strcpy(short_term_history[i], command);
		}

		while (i < N)
		{
			short_term_history[i] = NULL;
			i++;
		}

		while (ftell(fpCmd) < size)
		{
			fgets(command, longestStr + 3, fpCmd);
			cmdSize = strlen(command);
			command[cmdSize - 1] = '\0';
			command = (char*)realloc(command, (cmdSize + 1) * sizeof(char));
			insertDataToEndHistoryList(history, command, cmdNum);
			cmdNum++;
		}

		
		fclose(fpCmd);
		
	}

		free(command);

}

void saveCommandsToFile(char* fileName, char** short_term_history, HLIST* history)
{
	FILE * fp;
	int i;
	HNODE * curr = history->head;

	fp = fopen(fileName, "w");

	for (i = 0; i < N && short_term_history[i] != NULL; i++)
		fprintf(fp, "%s\n", short_term_history[i]);

	while (curr != NULL)
	{
		fprintf(fp, "%s\n", curr->data);
		curr = curr->next;
	}

	fclose(fp);
}

void readDetails(char * fileName, int * longestStr)
{
	FILE * fp;
	int size;
	int run, length;

	fp = fopen(fileName, "r");

	if (fp != NULL) // checks if the file does not exists
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		if (size != 0)
		{
			fscanf(fp, "%d", &run);
			fscanf(fp, "%d", &length);
		}

		runningAptCode = run;
		*longestStr = length;

		fclose(fp);
	}
}

void saveDetails(char * fileName, int longestStr)
{
	FILE * fp;

	fp = fopen(fileName, "w");

	fprintf(fp, "%d\n", runningAptCode);
	fprintf(fp, "%d\n", longestStr);

	fclose(fp);
}

void readApartmentsFromBinFile(char * fileName, DLIST* lstApt_Data)
{
	FILE* fp;
	APT_DATA_BASE data;
	int size;
	short int length;
	BYTE bytes[5];
	TM temp;

	fp = fopen(fileName, "rb");

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		while (ftell(fp) < size)
		{
			fread(&data.apartment.aptCode, sizeof(int), 1, fp);
			fread(&length, sizeof(short int), 1, fp);
			data.apartment.address = (char*)malloc((length + 1) * sizeof(char));
			fread(data.apartment.address, sizeof(char), length, fp);
			data.apartment.address[length] = '\0';
			fread(&data.apartment.price, sizeof(int), 1, fp);
			fread(bytes, sizeof(BYTE), 5, fp);
			getDetailsFromBYTEarr(&data, bytes);
			insertDataToEndListDList(lstApt_Data, &data);
		}
		fclose(fp);
	}
}

void getDetailsFromBYTEarr(APT_DATA_BASE* data, BYTE* bytes)
{
	BYTE mask1 = 0x0F, mask2 = 0x80, mask3 = 0x07;
	BYTE mask4 = 0xF, mask5 = 0xF8, mask6 = 0x7F, mask7 = 0x78;
	BYTE bt1, bt2, ch;

	{ // get the number of rooms and the entry date
		ch = bytes[0] >> 4;
		data->apartment.numOfRooms = (short int)(ch);

		bt1 = bytes[0] & mask1;
		bt1 <<= 1;
		bt2 = bytes[1] & mask2;
		bt2 >>= 7;
		ch = bt1 | bt2;
		data->apartment.date.day = (short int)(ch);

		bt1 = bt2 = 0;

		ch = bytes[1] & mask7;
		ch >>= 3;
		data->apartment.date.month = (short int)(ch);

		bt1 = bytes[1] & mask3;
		bt1 <<= 4;
		bt2 = bytes[2] & mask4;
		bt2 >>= 4;
		ch = bt1 | bt2;
		data->apartment.date.year = (short int)(ch);
	}

	{ // entry date
		bt1 = bt2 = 0;

		ch = bytes[3] & mask5;
		ch >>= 3;
		data->time.day = (short int)(ch);

		bt1 = bytes[3] & mask3;
		bt1 <<= 1;
		bt2 = bytes[4] & mask2;
		bt2 >>= 7;
		ch = bt1 | bt2;
		data->time.month = (short int)(ch);

		bt1 = 0;

		bt1 = bytes[4] & mask6;
		ch = bt1;
		data->time.year = (short int)(ch);

		convertTIMEToTime_t(data);
	}
}

void saveApartmentsToBinFile(char* fileName, DLIST* lstApt_Data)
{
	int i;
	short int length;
	FILE* fp;
	BYTE bytes[5];
	BYTE bt1, bt2, bt3;
	BYTE mask1 = 0x01, mask2 = 0x0E, mask3 = 0x38, mask4 = 0x7F;
	DLNODE* curr;
	TM* temp;

	fp = fopen(fileName, "wb");

	curr = lstApt_Data->head;

	while (curr != NULL)
	{
		length = strlen(curr->data->apartment.address);
		fwrite(&curr->data->apartment.aptCode, sizeof(int), 1, fp);
		fwrite(&length, sizeof(short int), 1, fp);
		fwrite(curr->data->apartment.address, sizeof(char), length, fp);
		fwrite(&curr->data->apartment.price, sizeof(int), 1, fp);

		bt1 = (curr->data->apartment.numOfRooms) << 4;
		bt2 = (curr->data->apartment.date.day) >> 1;
		bytes[0] = bt1 | bt2;

		bt1 = bt2 = 0;

		bt1 = (curr->data->apartment.date.day) & mask1;
		bt1 <<= 7;
		bt2 = (curr->data->apartment.date.month) & 0x0F;
		bt2 <<= 3;
		bt3 = (curr->data->apartment.date.year) & mask3;
		bt3 >>= 4;
		bytes[1] = bt1 | bt2 | bt3;

		bt1 = bt2 = 0;

		bt1 = (curr->data->apartment.date.year) & mask4;
		bt1 <<= 4;
		bytes[2] = bt1;

		bt1 = bt2 = 0;

		temp = localtime(&curr->data->enter_date_to_data);
		temp->tm_mon++;
		temp->tm_year -= 100;
		bt1 = (temp->tm_mday);
		bt1 <<= 3;
		bt2 = (temp->tm_mon);
		bt2 >>= 1;
		bytes[3] = bt1 | bt2;

		bt1 = bt2 = 0;

		bt1 = (temp->tm_mon) & mask1;
		bt1 <<= 7;
		bt2 = (temp->tm_year) & mask4;
		bytes[4] = bt1 | bt2;

		fwrite(bytes, sizeof(BYTE), 5, fp);

		curr = curr->next;
	}
}