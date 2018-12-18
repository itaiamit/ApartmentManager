#ifndef __N_I
#define __N_I

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char BYTE;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define SECONDS_TO_DAY 86400
#define N 7

typedef struct time{
	short int day;
	short int month;
	short int year;
}TIME;

typedef struct Apartment {
	int aptCode;
	char* address;
	int price;
	short int numOfRooms;
	TIME date;
}APT;

typedef struct ApartmentDataBase {
	APT apartment;
	time_t enter_date_to_data;
	TIME time;
}APT_DATA_BASE;

typedef struct tm TM;

#include "hlist_header.h"
#include "dlist_header.h"

int runningAptCode;// An external variable that is used for running code to the apartments

char* getInput(int* longestStr);
DLIST sortByMaxPrice(DLIST* lstApt_Data, int maxPrice);
DLIST sortByMinNumRooms(DLIST* lstApt_Data, short int numOfRooms);
DLIST sortByMaxNumRooms(DLIST* lstApt_Data, short int numOfRooms);
DLIST sortByDate(DLIST* lstApt_Data, TIME userDate);
DLIST sortByLastDays(DLIST* lstApt_Data, unsigned int days);
BOOL isDateEarlier(TIME userDate, TIME dateToCompare);
BOOL isBetweenDates(unsigned int days, double seconds);
void purchasedAptByCode(DLIST* lstApt_Data, int Acode);
void deleteAptByLastDays(DLIST* lstApt_Data, unsigned int days);
void printList(DLIST* lstApt_Data, BOOL lowToHigh);
void addApartment(DLIST* lstApt_Data, char* detalis);
void printAptCodes(DLIST* lstApt_Data);
void makeEmptyShortTermHistoryList(char** short_term_history);
void insertCommandToShortTermHistory(char** short_term_history, char* str, HLIST* history);
void advanceCommandToNextPlace(char** short_term_history);
void getCommand(DLIST* lstApt_Data, char* str, char** short_term_history, HLIST* history);
char * getStrFromHistory(HLIST * history, char** short_term_history, int num);
void printHistoryList(HLIST * history);
void printShortTermHistory(char** short_term_history);
void restoreCommands(DLIST* lstApt_Data, char* str, char* temp, char** short_term_history, HLIST* history);
void findCommands(DLIST* lstApt_Data, char* str, char* command, char** short_term_history, HLIST* history);
DLIST makeAsmallSortedList(DLIST* lstApt_Data, int price, int minRooms, int maxRooms, TIME date);
void printPrompt();
void readCommandsFromFile(char* fileName, char** short_term_history, HLIST* history, int longestStr);
void saveCommandsToFile(char* fileName, char** short_term_history, HLIST* history);
void readDetails(char * fileName, int * longestStr);
void saveDetails(char * fileName, int longestStr);
void readApartmentsFromBinFile(char * fileName, DLIST* lstApt_Data);
void getDetailsFromBYTEarr(APT_DATA_BASE* data, BYTE* bytes);
void saveApartmentsToBinFile(char* fileName, DLIST* lstApt_Data);
TIME convertTMtoTIME(TM* tm);
void convertTIMEToTime_t(APT_DATA_BASE* data);
void freeShort_Term_History(char** short_term_history);


#endif