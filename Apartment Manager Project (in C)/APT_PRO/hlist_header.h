#ifndef __I_A
#define __I_A

typedef struct hnode {
	char* data;
	int cmdNum;
	struct hnode *next;
	struct hnode *prev;
}HNODE;

typedef struct hlist {
	HNODE * head;
	HNODE * tail;
}HLIST;

void insertDataToStartHistoryList(HLIST* lst, char* data, int cmdNum);
HNODE* createNewHistoryListNode(char* data, int cmdNum, HNODE *next, HNODE *prev);
void insertNodeToStartHistoryList(HLIST* lst, HNODE *head);
void insertNodeToEndHistoryList(HLIST *lst, HNODE *tail);
void insertDataToEndHistoryList(HLIST *lst, char* data, int cmdNum);
BOOL isEmptyHList(HLIST* lst);
void makeEmptyHList(HLIST *lst);
void makeEmptyShortTermHistoryList(char** short_term_history);
void cmdNumUpdate(HLIST * history);
void freeHlist(HLIST* lst);

#endif

