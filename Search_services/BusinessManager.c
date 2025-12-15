#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6308)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>

#include "ManagerFiles.h"
#include "BusinessManager.h"
#include "BusinessOrder.h"
#include "User.h"
#include "Macros.h"

static const char* sortOptStr[eNofSortOpt] = { "None", "Name", "id", "service type" };

int initManager(BusinessManager* pManager, const char* txtFile,
	const char* binFile, const char* compressedBinFile, int option)
{
	int res;
	pManager->customersArr = NULL;
	pManager->businessesArr = NULL;
	pManager->customersCount = 0;
	pManager->businessesCount = 0;
	pManager->sortOpt = eNone;
	if (option >=1 && option <= 2)
	{
		if (readAllDataFromFile(pManager, txtFile, binFile, compressedBinFile, 1))
		{
			CLR_SCREEN_INPUT;
			printf("Program successfully loaded from a file\n");
			return 1;
		}

		else {
			printf("Error loading from files, continue loading manually\n");
		}
			
	}

	getchar(); // clean buffer
	CLR_SCREEN_INPUT;
	printf("\t\t\tLoading manually\n\n");
	pManager->name = getStrExactName("Enter manager company name\n");
	do {
		 res = initAddress(&pManager->address, "Enter address for manager ");
	} while (!res);
	
	return 1;
}


int addBusiness(BusinessManager* pManager)
{
	char id[ID + 1]; 
	Business* pBusiness = getBusinessFromUser(pManager, id);
	if (pBusiness != NULL) // business exists , inventory updated
		return 0;
	else {
		if (!addNewBusiness(pManager, id))
			return 0;
	}

	return 1;
}

Business* getBusinessFromUser(BusinessManager* pManager, char* id)
{
	getBusinessId(id);
	// checking if that business is already added
	return findBusinessByNameId(pManager, id);
}


Business* findBusinessByNameId(BusinessManager* pManager, const char* id)
{
	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (isFoundBusiness(pManager->businessesArr[i], id))
			return pManager->businessesArr[i];

	}

	return NULL;
}

int addNewBusiness(BusinessManager* pManager, const char* id)
{
	pManager->businessesArr = (Business**)realloc(pManager->businessesArr, (pManager->businessesCount + 1) * sizeof(Business*));
	if (!pManager->businessesArr)
		return 0;

	Business* pBusiness = (Business*)calloc(1, sizeof(Business));
	if (!pBusiness)
	{
		free(pManager->businessesArr);
		return 0;
	}

	strcpy(pBusiness->businessID, id);

	initBusinessNoID(pBusiness);

	pManager->businessesArr[pManager->businessesCount] = pBusiness;
	pManager->businessesCount++;
	pManager->sortOpt = eNone;
	return 1;
}

void addBusinessDetails(BusinessManagermanager);

int addCustomer(BusinessManager* pManager)
{
	pManager->customersArr = (User*)realloc(pManager->customersArr, 
		(pManager->customersCount + 1) * sizeof(User));
	CHECK_RETURN_0(pManager->customersArr);
	
	/*if (!pManager->customersArr)
	{
		printf("Mmemory allocation error for customer\n");
		return 0;
	}*/

	if (!initNewCustomer(&pManager->customersArr[pManager->customersCount]))
	{
		printf("Error initializing new customer\n");
		return 0;
	}

	pManager->customersCount++;
	return 1;
}


int doBusiness(BusinessManager* pManager)
{
	if (pManager->businessesCount == 0)
	{
		printf("No businesses added\n");
		return 0;
	}

	if (pManager->customersCount == 0)
	{
		printf("There no customers which can do business\n");
		return 0;
	}

	User* customer = findCustomersWhoWantsShop(pManager);
	if (!customer)
	{
		printf("Customer is not in the list, first add him\n");
		return 0;
	}
	
	BusinessOrder order;
	initOrder(&order);
	createNewOrder(pManager, &order, customer->name);

	printf("-------------> End of business order <--------------\n");
	printf("\n----------------- Business receipt for customer:    %s\n", customer->name);

	printfBusinessOrder(&order);
	printf("%s\n", SPACE_BARS);
	printf("------------------> Service ticket paid <-------------------\n\n");
	freeBusinessOrder(&order);
	printf("------------------- DONE DO BUSINESS ------------------------\n\n");

	return 1;

}

void createNewOrder(BusinessManager* pManager, BusinessOrder* pOrder, char* customerName)
{
	char	answer;
	char	itemName[MAX_STR_LEN];
	int		itemCount;
	float	itemPrice;

	printAllBusinesses(pManager);
	while (1)
	{
		printf("Do you want to make a deal? y / Y - yes, else to exit\n");
		do
		{
			scanf("%c", &answer);
		} while (isspace(answer));

		getchar();
		if (answer != 'y' && answer != 'Y')
			break;

		char id[ID + 1];
		Business* pB = choosesBusiness(pManager, id);
		if (pB == NULL)
			printf("Chosen business in not in list\n");

		else
		{
			pB = getBusinessInventory(pB, itemName, &itemCount, &itemPrice);
			if (pB)
			{
				pB->businessCustomersArr = (User*)realloc(pB->businessCustomersArr,
					(pB->customersSize + 1) * sizeof(User));

				if (!pB->businessCustomersArr)
				{
					printf("Memory allocation error\n"); 
					return;
				}

				saveCustomerNameToBusiness(&pB->businessCustomersArr[pB->customersSize], customerName);
				pB->customersSize++;

				if (!createBusinessOrder(pOrder, itemName, itemCount, itemPrice))
				{
					printf("Error creating order\n");
					return;
				}

				updateInventory(&pB->inventory, itemName, itemCount);
			}
		}

		
	}

}

void businessSaveCustomer(User* pCustomer, char* name)
{
	pCustomer->name = name;
}


Business* getBusinessInventory(Business* pB, char* itemName, int* itemCount, float* itemPrice)
{
	int res;
	
	if (pB->inventory.numItems == 0)
	{
		printf("Business out of inventory\n");
		return NULL;
	}
	
	// we need to check if business have enough items in inventory
	printParts(&pB->inventory);
	int ok; 

	do {
		ok = 1; // Set ok to true 
		printf("Choose part name you need\n");
		myGets(itemName, MAX_STR_LEN, stdin);

		if (!checkItemName(&pB->inventory, itemName))
			ok = 0;
		else
		{
			printf("How many parts you need?\t");
			scanf(" %d", itemCount);

			res = checkInventory(&pB->inventory, itemName, *itemCount);
			if (res == -1)
			{
				printf("No quantity in  inventory\n");
				return NULL;
			}
			else if (res == 0)
			{
				printf("Business doesn't have enough %s parts\n", itemName);
				ok = 0;
			}
			else
				ok = 1;
		}
	} while (!ok);

	float tempPrice = getPriceByItemName(&pB->inventory, itemName);
	*itemPrice = tempPrice;

	return pB;
}

Business* choosesBusiness(BusinessManager* pManager, char* b_Id)
{
	getBusinessId(b_Id);
	return chooseBusinessByID(pManager, b_Id);

}

void printAllBusinesses(BusinessManager* pManager)
{
	
	printf("There are %d Businesses\n", pManager->businessesCount);
	for (int i = 0; i < pManager->businessesCount; i++)
	{
		printf("%s\n\n", ASTERICKS);
		printf("%-20s %-20s %-20s\n", "Business name", "Business ID", "Business type");
		printf("%s\n", SPACE_BARS);

		generalArrayFunction((void*)pManager->businessesArr, pManager->businessesCount,
			sizeof(Business*), printBusinessesInfo);

		SCREEN_CLR;
	}
}


User* findCustomersWhoWantsShop(BusinessManager* pManager)
{
	printAllCustomers(pManager);
	char newCustomerName[MAX_STR_LEN];
	getsStrFixSize(newCustomerName, MAX_STR_LEN, "Who wants to do business ?\n");
	User* customer = findCustomerByName(pManager, newCustomerName);

	return customer;
}


void printAllCustomers(BusinessManager* pManager)
{
	printf("There are %d customers who can do business\n%s\n",pManager->customersCount, ASTERICKS);
	generalArrayFunction((void*)pManager->customersArr, pManager->customersCount, sizeof(User),
		printCustomer);

}	

User* findCustomerByName(BusinessManager* pManager, const char* name)
{
	for (int i = 0; i < pManager->customersCount; i++)
	{
		if (isFoundCustomer(&pManager->customersArr[i], name))
			return &pManager->customersArr[i];
	}

	return NULL;
}

Business* chooseBusinessByID(BusinessManager* pManager, const char* id)
{
	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (isFoundBusinessById(pManager->businessesArr[i], id))
			return pManager->businessesArr[i];
	}

	return NULL;
}

void printBusinessByType(const BusinessManager* pManager)
{
	int count = 0; // to count number of businesses with same type
	if (pManager->businessesCount == 0)
	{
		printf("No businesses initialized\n");
		return;
	}

	eBusinessType type = getBusinessType();
	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (pManager->businessesArr[i]->businessType == type)
		{
			count++;
			printBusiness(pManager->businessesArr[i]);
		}
	}
	// if not businesses with that type
	if (count == 0)
		printf("There are no business with type %s\n", getBusinessTypeStr(type));
}

void printManager(BusinessManager* pManager)
{
	printf("Manager name is %s\n", pManager->name);
	printf("Manager address is :\n");
	printAddress(&pManager->address);
	printf("\n%s\n", ASTERICKS);
	printAllBusinesses(pManager);
}

void sortBusiness(BusinessManager* pManager)
{
	pManager->sortOpt = showSortingMenu();
	int (*compare)(const void* bs1, const void* bs2) = NULL;
	compare = getCompareFunction(pManager);
	if (compare != NULL) {
		qsort(pManager->businessesArr, pManager->businessesCount,
			sizeof(Business*), compare);
	}
	else
		printf("Sort business error\n");

}

eSortOption showSortingMenu()
{
	int option;
	printf("How do you want to sort?\n");
	do
	{
		for (int i = 0; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &option);
	} while (option < eNone || option > eNofSortOpt);

	return (eSortOption)option;
}

void findBusiness(const BusinessManager* pManager)
{
	int (*compare)(const void* bs1, const void* bs2) = NULL;
	/*Business business = { 0 };
	Business* pBusiness = &business;*/ 

	Business* pBusiness = malloc(sizeof(Business)); 
	if (pBusiness == NULL)
	{
		printf("Memory allocation failed.\n");
		return;
	}
	compare = getCompareFunction(pManager);
	
	switch (pManager->sortOpt)
	{
	case eName:
		initBusinessName(pBusiness);
		break;

	case eID:
		getBusinessId(pBusiness->businessID);
		break;

	case eType:
		getBusinessType(pBusiness->businessType);
		break;

	}

	if (compare != NULL)
	{
		Business** pB = bsearch(&pBusiness, pManager->businessesArr,
			pManager->businessesCount, sizeof(Business*), compare);

		if (pB == NULL)
			printf("Business wasn't found\n");
		else
		{
			printf("Found business:\n\n");
			printBusiness(*pB);
		}
	}
	else
		printf("Cant search , businesses not sorted\n");

	free(pBusiness);
}

void* getCompareFunction(const BusinessManager* pManager)
{
	int(*compare)(const void* b1, const void* b2) = NULL;

	switch (pManager->sortOpt)
	{

	case eName:
		compare = compareBusinessByName;
		break;

	case eID:
		compare = compareBusinessByID;
		break;

	case eType:
		compare = compareBusinessByType;
		break;

	case eNone:
	case eNofSortOpt:
	default:
		compare = NULL;
		break;
	}

	return compare;
}

void printAllUsers(BusinessManager* pManager)
{
	int opt;
	printf("\tType 1 to print all users of the system\t");
	printf("else to print users who did business by each business\n\n");

	scanf("%d", &opt);
	if (opt == 1)
	{
		printf("Manager has %d customers\n", pManager->customersCount);
		generalArrayFunction((void*)pManager->customersArr, pManager->customersCount, 
			sizeof(User), printManagerUser);

		//    SWAPED WITH ABOVE
		//printManagerUser(pManager);
	}
		
	else
		printCustomersbyBusiness(pManager);
}

void 		printManagerUser(BusinessManager* pManager)
{
	for (int i = 0; i < pManager->customersCount; i++)
		printf("%s\n", pManager->customersArr[i].name);
}

void printCustomersbyBusiness(BusinessManager* pManager)
{
	for (int i = 0; i < pManager->businessesCount; i++)
		printCustomersInBusiness(pManager->businessesArr[i]);

	/* CHECK if works
	 printf("Customers for Business %d:\n", i + 1);
        generalArrayFunction((void*)pManager->businessesArr[i]->businessCustomersArr,
                             pManager->businessesArr[i]->customersSize,
                             sizeof(User),
                             printCustomer);
        printf("\n");
	*/
}

void showRatingsReviews(BusinessManager* pManager)
{
		printAllBusinessesRatingsReviews(pManager);
}

int addRatingReview(BusinessManager* pManager)
{
	char id[ID + 1] = { 0 };
	printAllBusinesses(pManager);
	if (pManager->businessesCount == 0)
	{
		printf("There are no business, add 1 first\n");
		return ERROR;
	}

	Business* pB = choosesBusiness(pManager, id);
	if (!pB)
	{
		printf("Wrong Business chosen, try again\n");
		return ERROR;
	}

	printf("Adding new review to business - %s\n", pB->businessName);
	if (!addNewReview(&pB->RatingsAndReviews, pB->businessName))
		return 0;


	return 1;
}

void printSpecificBusinessRatingReview(BusinessManager* pManager)
{
	char id[ID];
	printAllBusinesses(pManager);
	Business* pB = choosesBusiness(pManager, id);
	printBusinessRatingReview(pB);
}

void printAllBusinessesRatingsReviews(BusinessManager* pManager)
{
	for (int i = 0; i < pManager->businessesCount; i++)
		printRatingReviewForEachBusiness(pManager->businessesArr[i]);

	printf("%s\n", ASTERICKS);
}

void showTopBusiness(BusinessManager* pManager)
{
	int res, ok = 0;
	printf("Enter how many top businesses you want to see: ");
	do {
		if (scanf("%d", &res) != 1) {
			printf("Invalid input! Please enter a valid positive integer.\n");
			while (getchar() != '\n'); // Clear the input buffer
		}
		else if (res <= 0) {
			printf("Invalid input! Please enter a positive number.\n");
		}
		else {
			ok = 1;
		}
	} while (!ok);

	if (pManager->businessesCount < res)
	{
		printf("There are fewer than %d businesses inserted.\n", res);
		return;
	}

	qsort(pManager->businessesArr, pManager->businessesCount,
		sizeof(Business*), compareBusinessByRatings);

	printf("Top %d Businesses by Rating:\n", res);
	/*for (int i = 0; i < res; i++)
	{
		Business* business = pManager->businessesArr[i];
		float averageRating = getAverageBusinessRating(&business->RatingsAndReviews);
		printf("%d. %s (Avg. Rating: %.2f)\n", i + 1, business->businessName, averageRating);
	}*/
	generalArrayFunction((void*)pManager->businessesArr, res, sizeof(Business*), printBusinessRating);

}

void printBusinessRating(const void* element)
{
	const Business* business = *(const Business**)element; // Cast the element pointer

	float averageRating = getAverageBusinessRating(&business->RatingsAndReviews);
	printf("%s (Avg. Rating: %.2f)\n", business->businessName, averageRating);
}

void freeManager(BusinessManager* pManager)
{
	free(pManager->name);
	freeAddress(&pManager->address);
	for (int i = 0; i < pManager->businessesCount; i++)
	{
		// free all fields of business
		freeBusiness(pManager->businessesArr[i]); 
		// free each business[i], entire business
		free(pManager->businessesArr[i]); 
	}

	free(pManager->businessesArr); // free business**

	generalArrayFunction((void*)pManager->customersArr, pManager->customersCount,
		sizeof(User), freeUser);
	free(pManager->customersArr);
}








