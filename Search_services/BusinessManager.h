#ifndef __MANAGER__
#define __MANAGER__

#include "Business.h"
#include "Address.h"
#include "User.h"
#include "BusinessOrder.h"

typedef enum { eNone, eName, eID, eType, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];

typedef struct {

	char*				name;
	Address				address;
	eSortOption			sortOpt;

	User*				customersArr;
	int					customersCount;

	Business**			businessesArr;
	int					businessesCount;
}BusinessManager;


int initManager(BusinessManager* pManager, const char* txtFile, 
	const char* binFile, const char* compressedBinFile, int option);

int			addBusiness(BusinessManager* pManager);
int			addNewBusiness(BusinessManager* pManager, const char* id);
int			addCustomer(BusinessManager* pManager);
int			doBusiness(BusinessManager* pManager);

Business*	getBusinessFromUser(BusinessManager* pManager, char* id);
Business*	getBusinessInventory(Business* pManager, char* itemName, int* itemCount, float* itemPrice);
Business*	choosesBusiness(BusinessManager* pManager, char* b_Id);
Business*	findBusinessByNameId(BusinessManager* pManager, const char* id);
Business*	chooseBusinessByID(BusinessManager* pManager, const char* id);
User*		findCustomerByName(BusinessManager* pManager, const char* name);

void		printBusinessByType(const BusinessManager* pManager);
void		printAllBusinesses(BusinessManager* pManager);
void		printAllCustomers(BusinessManager* pManager);
void		createNewOrder(BusinessManager* pManager, BusinessOrder* pOrder, char* customerName);

eSortOption showSortingMenu();

User* findCustomersWhoWantsShop(BusinessManager* pManager);


void		findBusiness(const BusinessManager* pManager);
void		sortBusiness(BusinessManager* pManager);
void		printAllUsers(BusinessManager* pManager);

void 		printManagerUser(BusinessManager* pManager);
void		printCustomersbyBusiness(BusinessManager* pManager);
void		printSpecificBusinessRatingReview(BusinessManager* pManager);
void		printAllBusinessesRatingsReviews(BusinessManager* pManager);
void        showTopBusiness(BusinessManager* pManager);

void*		getCompareFunction(const BusinessManager* pManager);

void		showRatingsReviews(BusinessManager* pManager);
int			addRatingReview(BusinessManager* pManager);

void		freeManager(BusinessManager* pManager);


//new
void printBusinessRating(const void* element);
#endif // !__MANAGER__




