#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "FileHelper.h"
#include "Macros.h"
#include "Business.h"

const char* typeStr[eNoOfBussinessType] = { "Home Repair", "Electric Repair", "Computer Repair", "Smartphone Repair" };

void getBusinessId(char* id)
{
	char temp[MAX_STR_LEN];

	char msgIntro[MAX_STR_LEN];
	char msgLengthError[MAX_STR_LEN];
	char msgInputError[MAX_STR_LEN];
	char msgPositiveError[MAX_STR_LEN];

	int ok = 1;

	sprintf(msgIntro, "id should be exactly %d length , exactly two capital letters and numbers\n", ID);
	sprintf(msgLengthError, "Error - id is not length of %d\n", ID);
	sprintf(msgInputError, "Id input error, try again\n");
	sprintf(msgPositiveError, "Error - id must contain a positive number\n");
	do
	{
		ok = 1;
		int capitalCount = 0;
		printf("Enter business ID\n");
		getsStrFixSize(temp, MAX_STR_LEN, msgIntro);
		int len = (int)strlen(temp);
		if (len != ID)
		{
			puts(msgLengthError);
			ok = 0;
		}

		else
		{
			for (int i = 0; i < ID; i++)
			{
				if (!isdigit(temp[i]) && !isupper(temp[i]))
				{
					puts(msgInputError);
					ok = 0;
					break;
				}

				if (temp[i] == '0')
				{
					puts(msgPositiveError);
					ok = 0;
				}

				if (isupper(temp[i]))
					capitalCount++;
			}
		}

		if (capitalCount != 2)
		{
			puts(msgInputError);
			ok = 0;
		}

	} while (!ok);

	
	strcpy(id, temp);
	
}


int isFoundBusiness(Business* pBusiness, const char* id)
{
	if ( (strcmp(pBusiness->businessID, id)) == 0 )
		return 1;

	return 0;
}

void initBusinessNoID(Business* pBusiness)
{
	initBusinessName(pBusiness);

	pBusiness->businessType = getBusinessType();
	
	initBusinessInventory(&pBusiness->inventory);
	
	initSchedule(&pBusiness->workHours);
	
	initAddress(&pBusiness->businessAddress, "Enter address for your business ");
	
	initBusinessCustomersArr(pBusiness);

	initRatingsReviews(&pBusiness->RatingsAndReviews);	

	
}

void initBusinessName(Business* pBusiness)
{
	printf("Enter a name for a business up to %d chars\n", MAX_STR_LEN - 1);
	myGets(pBusiness->businessName, MAX_STR_LEN, stdin);
}

// new function
void initBusinessCustomersArr(Business* pBusiness)
{
	pBusiness->businessCustomersArr = NULL;
	pBusiness->customersSize = 0;
}

eBusinessType getBusinessType()
{
	int opt;
	printf("\n\n\n");
	do
	{
		printf("Choose a type of business\n ");
		for (int i = 0; i < eNoOfBussinessType; i++)
			printf("\t%d ------> %s\n", i, typeStr[i]);
		scanf("%d", &opt);

	} while (opt < 0 || opt >= eNoOfBussinessType);

	getchar(); // clear buffer

	return (eBusinessType)opt;
}

void printBusiness(const Business* business)
{
	printf("%-20s %-20s %-20s\n", business->businessName, business->businessID, typeStr[business->businessType]);
	printf("%s\nBusiness Address : \n", SPACE_BARS);
	printAddress(&business->businessAddress);
	printSchedule(&business->workHours);
	printRatingReview(&business->RatingsAndReviews);
}


void printCustomersInBusiness(const Business* pBusiness)
{
	printf("There are %d customers who shopped in business %s\n", pBusiness->customersSize, pBusiness->businessName);
	generalArrayFunction((void*)pBusiness->businessCustomersArr, pBusiness->customersSize,
		sizeof(User), printCustomer);

}

void printBusinessesInfo(const void* element)
{
	const Business* pB = *(const Business**)element;
	printBusiness(pB);
}


void printBusinessRatingReview(const Business* pBusiness)
{
	printf("Printing business - %s\n", pBusiness->businessName);
	printRatingReview(&pBusiness->RatingsAndReviews);
}

void printRatingReviewForEachBusiness(const Business* pBusiness)
{
	printf("Printing business - %s\n", pBusiness->businessName);
	printRatingReview(&pBusiness->RatingsAndReviews);
	printf("\n%s\n", SPACE_BARS);
}


const char* getBusinessTypeStr(eBusinessType type)
{
	if (type < 0 || type >= eNoOfBussinessType)
		return NULL;

	return typeStr[type];
}

int isFoundBusinessById(Business* pBusiness, const char* id)
{
	if (strcmp(pBusiness->businessID, id) == 0)
		return 1;

	return 0;
}



int compareBusinessByName(const void* value1, const void* value2)
{
	const Business* pB1 = *(const Business**)value1;
	const Business* pB2 = *(const Business**)value2;

	return strcmp(pB1->businessName, pB2->businessName);
}

int compareBusinessByID(const void* value1, const void* value2)
{
	const Business* pB1 = *(const Business**)value1;
	const Business* pB2 = *(const Business**)value2;

	return strcmp(pB1->businessID, pB2->businessID);
}

int compareBusinessByType(const void* value1, const void* value2)
{
	const Business* pB1 = *(const Business**)value1;
	const Business* pB2 = *(const Business**)value2;

	return (pB1->businessType - pB2->businessType);
}
// --------------------------------------------------------------------
int compareBusinessByRatings(const void* value1, const void* value2)
{
	const Business* BusinessA = *(const Business**)value1;
	const Business* BusinessB = *(const Business**)value2;

	float averageBusinessA = getAverageBusinessRating(&BusinessA->RatingsAndReviews);
	float averageBusinessB = getAverageBusinessRating(&BusinessB->RatingsAndReviews);

	// Compare average ratings and return appropriate integer value
	if (averageBusinessA < averageBusinessB)
		return -1;
	else if (averageBusinessA > averageBusinessB) 
		return 1;
	else 
		return 0;
	
}

void freeBusiness(Business* pBusiness)
{
	freeAddress(&pBusiness->businessAddress);
	// no need to free schedule - fixed
	// no need to free inventory - fixed
	// no need to free ratingsReviews - fixed

	generalArrayFunction((void*)pBusiness->businessCustomersArr,
		pBusiness->customersSize, sizeof(User), freeUser);
}


int saveBusinessToBinFile(const Business* pBusiness, FILE* fp)
{
	if (!writeStringToFile(pBusiness->businessName, fp, "Error writing business name to file\n"))
		return 0;

	if (!writeStringToFile(pBusiness->businessID, fp, "Error writing business id to file\n"))
		return 0;

	if (!writeIntToFile(pBusiness->businessType, fp, "Error writing business type to file\n"))
		return 0;

	if (!saveAddressToBinFile(&pBusiness->businessAddress, fp))
		return 0;

	if (!saveBusinessInventoryToFile(&pBusiness->inventory, fp))
		return 0;

	return 1;
}

int readBusinessFromBinFiles(Business* pBusiness, FILE* binary_fp, FILE* compressed_fp)
{
	if (!readFixSizeStrFromFile(pBusiness->businessName, binary_fp, "Error reading business name from binary file\n"))
		return 0;

	if (!readFixSizeStrFromFile(pBusiness->businessID, binary_fp, "Error reading business ID from binary file\n"))
		return 0;

	int type;
	if (!readIntFromFile(&type, binary_fp, "Error reading business type from binary file\n"))
		return 0;

	pBusiness->businessType = (eBusinessType)type;

	if (!readAddressFromBinFile(&pBusiness->businessAddress, binary_fp))
		return 0;

	if (!readBusinessInventoryFromBinFile(&pBusiness->inventory, binary_fp))
		return 0;

	if (!readBusinessWorkHoursFromCompressedBinFile(&pBusiness->workHours, compressed_fp))
		return 0;

	return 1;


}

int saveBusinessToTxtFile(const Business* pBusiness, FILE* fp)
{
	if (!fprintf(fp, "%s\n", pBusiness->businessName))
		return 0;

	if (!fprintf(fp, "%s\n", pBusiness->businessID))
		return 0;

	if (!fprintf(fp, "%d\n", (int)pBusiness->businessType))
		return 0;

	if (!saveAddressToTxtFile(&pBusiness->businessAddress, fp))
		return 0;

	if (!savingBusinessWorkHoursToTxtFile(&pBusiness->workHours, fp))
		return 0;

	if (!savingBusinessInventoryToTxtFile(&pBusiness->inventory, fp))
		return 0;

	return 1;
}

int readBusinessFromTxtFile(Business* pBusiness, FILE* fp)
{
	if (!fscanf(fp, "%s\n", pBusiness->businessName))
		return 0;

	if (!fscanf(fp, "%s\n", pBusiness->businessID))
		return 0;

	int type;
	if (!fscanf(fp, "%d\n", &type))
		return 0;

	pBusiness->businessType = type;

	if (!readAddressFromTxtFile(&pBusiness->businessAddress, fp))
		return 0;

	if (!readingBusinessWorkHoursToTxtFile(&pBusiness->workHours, fp))
		return 0;

	if (!readingBusinessInventoryFromTxtFile(&pBusiness->inventory, fp))
		return 0;

	return 1;
}
	