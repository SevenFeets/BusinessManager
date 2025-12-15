#ifndef __BUSINESS__ORDER__
#define __BUSINESS__ORDER__


#include "General.h"
#include "LinkedList.h"
#include "Service.h"

#define SUCCESS_ADD_ITEM 1

typedef struct {	

	LIST	businessOrders;

}BusinessOrder;

void initOrder(BusinessOrder* pOrder);

int createBusinessOrder(BusinessOrder* pOrder, const char* itemName,
	int itemCount, float itemPrice);


int insertOrderToList(LIST_NODE* pPrevNode, const char* name, int itemCount, float itemPrice);

void printfBusinessOrder(const BusinessOrder* pOrder);

float getBusinessInvoice(const BusinessOrder* pOrder);

void freeBusinessOrder(BusinessOrder* pOrder);
#endif // !__BUSINESS__ORDERS__

