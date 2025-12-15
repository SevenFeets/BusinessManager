#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Service.h"


Service* createNewService(const char* name, int count, float price)
{
	Service* serviceTicket = (Service*)malloc(1 * sizeof(Service));
	if (!serviceTicket)
	{
		printf("Error allocation memory for new service ticket\n");
		return NULL;
	}

	serviceTicket->orderID = generateOrderID();
	getCorrectDate(&serviceTicket->orderDate);

	serviceTicket->Name =  _strdup(name);
	serviceTicket->Count = count;
	serviceTicket->price = price;

	return serviceTicket;
}

void printServiceTicket(const Service* pService)
{
	printf("--------------- SERVICE TICKET -----------------\n\n");
	printf("%-10s %-10s %-10s %-10s %-10s\n",	"Order ID", "Item name", "quantity", "Price" , "Order date");
	printf("%s\n", SPACE_BARS);

	printf("%-10d %-10s %-10d %-10.2f\t",pService->orderID , pService->Name, pService->Count, pService->price);
	printDate(&pService->orderDate); // Assuming printDate() prints date in the appropriate format
	printf("%s\n\n", ASTERICKS);
}


void freeService(Service* pService)
{
	if (pService->Name != NULL)
		free(pService->Name);
}