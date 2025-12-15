#pragma once



typedef struct {
	char*			name;
	
}User;

int		initNewCustomer(User* pUser);
void	printCustomer(const User* pUser);
int		isFoundCustomer(User* pUser, const char* name);
void	freeUser(User* pUser);

void	saveCustomerNameToBusiness(User* businessCustomer, char* name);
