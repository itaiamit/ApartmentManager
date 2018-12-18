#ifndef __N_R
#define __N_R

typedef struct dlnode {
	APT_DATA_BASE* data;
	struct dlnode *next;
	struct dlnode *prev;
}DLNODE;

typedef struct dlist {
	DLNODE * head;
	DLNODE * tail;
}DLIST;

void insertDataToStartListDList(DLIST *lst, APT_DATA_BASE* data);
DLNODE* createNewDListNode(APT_DATA_BASE* data, DLNODE *next, DLNODE *prev);
void insertNodeToStartDList(DLIST *lst, DLNODE *head);
void insertDataToEndListDList(DLIST *lst, APT_DATA_BASE* data);
void insertNodeToEndDList(DLIST *lst, DLNODE *tail);
void insertAfter(DLIST *lst, DLNODE * after, APT_DATA_BASE* data);
void insertNodeAfter(DLIST *lst, DLNODE * after, DLNODE *node);
void makeEmptyList(DLIST *lst);
BOOL isEmptyList(DLIST* lst);
void freeDlist(DLIST* lst);

#endif