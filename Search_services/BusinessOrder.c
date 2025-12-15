#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BusinessOrder.h"


void initOrder(BusinessOrder* pOrder)
{
	L_init(&pOrder->businessOrders);
}


int createBusinessOrder(BusinessOrder* pOrder, const char* itemName,
	int itemCount, float itemPrice)
{
	//int compareItemByName;
	LIST_NODE* pCurrentNode = pOrder->businessOrders.head.next;
	LIST_NODE* pPrevNode = &pOrder->businessOrders.head;

	Service* pService;

	while (pCurrentNode != NULL)
	{
		pService = (Service*)pCurrentNode->key;
		
		if (strcmp(pService->Name, itemName) == 0)
		{
			pService->Count += itemCount;
			return SUCCESS_ADD_ITEM;
		}

		else if (strcmp(pService->Name, itemName) > 0)
			return insertOrderToList(pPrevNode, itemName, itemCount, itemPrice);
	
		pPrevNode = pCurrentNode;
		pCurrentNode = pCurrentNode->next;
	
	}
	
	return insertOrderToList(pPrevNode, itemName, itemCount, itemPrice);
	
}

int insertOrderToList(LIST_NODE* pPrevNode, const char* name, int count, float price)
{

	Service* pService = createNewService(name, count, price);
	if (!pService)
	{
		printf("Error allocation memory\n");
		return 0;
	}

	LIST_NODE* newNode = L_insert(pPrevNode, pService);
	if (!newNode)
	{
		printf("Error creating new node\n");
		return 0;
	}

	return 1;
	
}

void printfBusinessOrder(const BusinessOrder* pOrder)
{
	float totalBusinessDeal = getBusinessInvoice(pOrder);
	L_print(&pOrder->businessOrders, printServiceTicket);
	printf("Total service price is %.2f\n\n", totalBusinessDeal);
}

float getBusinessInvoice(const BusinessOrder* pOrder)
{
	// iterate over the list, calculate each node price
	float total_price = 0.0;
	Service* pService;
	
	// pointing to first node
	LIST_NODE* pNode = pOrder->businessOrders.head.next; 

	while (pNode != NULL)
	{
		// fist casting to node key and inserting to service object
		pService = (Service*)pNode->key;
		total_price = (pService->Count) * (pService->price);
		pNode = pNode->next;
	}

	return total_price;
}

void freeBusinessOrder(BusinessOrder* pOrder)
{
	L_free(&pOrder->businessOrders, freeService);
}