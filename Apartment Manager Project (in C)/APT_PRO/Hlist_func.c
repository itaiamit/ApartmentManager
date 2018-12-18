#include "header.h"


void makeEmptyHList(HLIST *lst)
{
	lst->head = lst->tail = NULL;
}

BOOL isEmptyHList(HLIST* lst)
{
	if (lst->head == NULL)
		return TRUE;

	else
		return FALSE;
}

void insertDataToStartHistoryList(HLIST *lst, char* data, int cmdNum)
{
	HNODE *newHead;
	newHead = createNewHistoryListNode(data, cmdNum, NULL, NULL);
	insertNodeToStartHistoryList(lst, newHead);
}

HNODE* createNewHistoryListNode(char* data, int cmdNum, HNODE *next, HNODE *prev)
{
	HNODE *res;
	int size;

	size = strlen(data);
	res = (HNODE*)malloc(sizeof(HNODE));
	res->data = (char*)malloc((size + 1)*sizeof(char));
	strcpy(res->data, data); // strcp
	res->cmdNum = cmdNum;
	res->next = next;
	res->prev = prev;

	return res;
}

void insertNodeToStartHistoryList(HLIST *lst, HNODE *head)
{
	if (isEmptyHList(lst) == TRUE)
	{
		head->next = head->prev = NULL;
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		head->prev = NULL;
		lst->head->prev = head;
		lst->head = head;
	}
}

void cmdNumUpdate(HLIST * history)
{
	HNODE * curr = history->head;

	while (curr != NULL)
	{
		curr->cmdNum++;
		curr = curr->next;
	}
}

void insertDataToEndHistoryList(HLIST *lst, char* data, int cmdNum)
{
	HNODE *newTail;
	newTail = createNewHistoryListNode(data, cmdNum, NULL, NULL);
	insertNodeToEndHistoryList(lst, newTail);
}

void insertNodeToEndHistoryList(HLIST *lst, HNODE *tail)
{
	if (isEmptyHList(lst) == TRUE)
	{
		tail->next = tail->prev = NULL;
		lst->head = lst->tail = tail;
	}
	else
	{
		tail->prev = lst->tail;
		tail->next = NULL;
		lst->tail->next = tail;
		lst->tail = tail;
	}
}