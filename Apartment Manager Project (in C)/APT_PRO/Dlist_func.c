#include "header.h"


void insertDataToStartListDList(DLIST *lst, APT_DATA_BASE* data)
{
	DLNODE *newHead;
	newHead = createNewDListNode(data, NULL, NULL);
	insertNodeToStartDList(lst, newHead);
}

DLNODE* createNewDListNode(APT_DATA_BASE* data, DLNODE *next, DLNODE *prev)
{
	DLNODE *res;
	int size;
	TM* tm;

	size = strlen(data->apartment.address);
	res = (DLNODE*)malloc(sizeof(DLNODE));
	res->data = (APT_DATA_BASE*)malloc(sizeof(APT_DATA_BASE));
	res->data->apartment.address = (char*)malloc((size + 1) * sizeof(char));
	strcpy(res->data->apartment.address, data->apartment.address);
	res->data->apartment.date = data->apartment.date;
	res->data->apartment.numOfRooms = data->apartment.numOfRooms;
	res->data->apartment.price = data->apartment.price;
	res->data->apartment.aptCode = data->apartment.aptCode;
	res->next = next;
	res->prev = prev;

	time(&res->data->enter_date_to_data);
	tm = localtime(&res->data->enter_date_to_data);
	res->data->time = convertTMtoTIME(tm);

	return res;
}

void insertNodeToStartDList(DLIST *lst, DLNODE *head)
{
	if (isEmptyList(lst) == TRUE)
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

void insertDataToEndListDList(DLIST *lst, APT_DATA_BASE* data)
{
	DLNODE *newTail;
	newTail = createNewDListNode(data, NULL, NULL);
	insertNodeToEndDList(lst, newTail);
}

void insertNodeToEndDList(DLIST *lst, DLNODE *tail)
{
	if (isEmptyList(lst) == TRUE)
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

void insertAfter(DLIST *lst, DLNODE * after, APT_DATA_BASE* data) {

	DLNODE * newNode;

	newNode = createNewDListNode(data, NULL, NULL);
	insertNodeAfter(lst, after, newNode);
}

void insertNodeAfter(DLIST *lst, DLNODE * after, DLNODE *node) {

	if (isEmptyList(lst) == TRUE) {

		lst->head = lst->tail = node;
		node->next = node->prev = NULL;
	}

	else if (after == NULL)
		insertNodeToStartDList(lst, node);

	else if (after->next == NULL)
		insertNodeToEndDList(lst, node);

	else {

		node->next = after->next;
		node->prev = after;

		after->next->prev = node;
		after->next = node;
	}

}

void makeEmptyList(DLIST *lst)
{
	lst->head = lst->tail = NULL;
}

BOOL isEmptyList(DLIST* lst)
{
	if (lst->head == NULL)
		return TRUE;

	else
		return FALSE;
}