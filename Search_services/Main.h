#pragma once

typedef enum
{
	eAddBusiness, eAddCustomer , eCustomerDoBusiness , eAddReview,
	efindBusiness, eSortBusiness, eShowRatingsAndReviews, eShowTopBusinesses,
	ePrintUsers, ePrintBusiness,  ePrintBusinessByType,
	eGeneralOptions
}eMainMenuOptions;

const char* mainMenu[eGeneralOptions];



#define EXIT				-1
#define EXIT_AUTORIZATION	-2
#define USER				 1
#define OWNER				 2


#define PROGRAM_FILE_TEXT				"program.txt"
#define PROGRAM_FILE_BIN				"program.bin"
#define MANAGER_ADDRESS_COMPRESSED		"Compressed.bin"


int showGlobalMenu();
