#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "User.h"
#include "General.h"
#include "Macros.h"


int initNewCustomer(User* pUser)
{
	pUser->name = getStrExactName("Enter customer name\n");
	if (!pUser->name)
		return 0;

	return 1;
}

void printCustomer(const User* pUser)
{
	printf("Name:%s ------>  %s\n",SPCAES,  pUser->name);
	
}

int isFoundCustomer(User* pUser, const char* name)
{
	if (strcmp(pUser->name, name) == 0)
		return 1;

	return 0;
}

void saveCustomerNameToBusiness(User* businessCustomer,  char* name)
{
	businessCustomer->name = name;
}

void freeUser(User* pUser)
{
	CHECK_NOT_NULL_FREE(pUser->name);
}
