#ifndef __SERVICE__
#define __SERVICE__

#include "Date.h"
#include "General.h"


typedef struct {

	int		orderID; 
	Date	orderDate;
	
	char*	Name;
	int		Count;
	float	price;

}Service;

Service* createNewService(const char* name, int count, float price);

void printServiceTicket(const Service* pService);

void freeService(Service* pService);


#endif // !__SERVICE__
