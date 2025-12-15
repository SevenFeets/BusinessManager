#pragma once

#include "General.h"
#include "Date.h"
#include "User.h"
#include "Address.h"
#include "Inventory.h"
#include "BusinessOrder.h"
#include "WorkSchedule.h"
#include "RatingsReviews.h"

typedef enum {eHomeRepair, eElectricRepair, 
			  eComputerRepair, eSmartphoneRepair, 
			  eNoOfBussinessType
} eBusinessType;

const char* typeStr[eNoOfBussinessType];

typedef struct {

	char			businessName[MAX_STR_LEN + 1];
	char			businessID[ID + 1];
	eBusinessType	businessType;
	Address			businessAddress;
	Schedule		workHours;
	Inventory		inventory;


	
	User*			businessCustomersArr;
	int				customersSize;
	
	Ratings_Reviews RatingsAndReviews;
	
	BusinessOrder*	ordersArr;
	int				numOfOrders;
	
}Business;


void			getBusinessId(char* id);
void			initBusinessName(Business* pBusiness);
int				isFoundBusiness(Business* pBusiness, const char* id);
void			initBusinessNoID(Business* pBusiness);
eBusinessType	getBusinessType();
void			printBusinessesInfo(const void* element);
void			printBusiness(const Business* business);
int				isFoundBusinessById(Business* pBusiness, const char* id);
const char*		getBusinessTypeStr(eBusinessType type);
void			initBusinessCustomersArr(Business* pBusiness);
void			printCustomersInBusiness(const Business* pBusiness);
void			printBusinessRatingReview(const Business* pBusiness);
void			printRatingReviewForEachBusiness(const Business* pBusiness);

int compareBusinessByName(const void* value1, const void* value2);
int compareBusinessByID(const void* value1, const void* value2);
int compareBusinessByType(const void* value1, const void* value2);
int compareBusinessByRatings(const void* value1, const void* value2);


void freeBusiness(Business* pBusiness);

int saveBusinessToBinFile(const Business* pBusiness, FILE* fp);
int readBusinessFromBinFiles(Business* pBusiness, FILE* binary_fp, FILE* compressed_fp);

int saveBusinessToTxtFile(const Business* pBusiness, FILE* fp);
int readBusinessFromTxtFile(Business* pBusiness, FILE* fp);


