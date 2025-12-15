#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Address.h"
#include "General.h"
#include "Macros.h"
#include "fileHelper.h"

#define WORD_SEP " "

int initAddress(Address* pAddress, const char* msg)
{
	char allData[MAX_STR_LEN];
	char** elements;
	int count;
	int ok = 1;
	do {
		printf("%s", msg);
		getsStrFixSize(allData, sizeof(allData),
			"\nFormat: street,house number,city\ncity and street can have spaces\n");

		elements = splitCharsToWordsByDel(allData, &count, ",");
		ok = checkElements(elements, count);
		if (!ok)
			freeElements(elements, count);
	} while (!ok);

	//get the house number
	sscanf(elements[1], "%d", &pAddress->buildingNum);
	pAddress->street = fixAddressParam(elements[0]);
	if (!pAddress->street)
	{
		freeElements(elements, count);
		return 0;
	}

	pAddress->city = fixAddressParam(elements[2]);
	if (!pAddress->city)
	{
		free(pAddress->street);
		freeElements(elements, count);
		return 0;
	}

	freeElements(elements, count);
	return 1;
}


char* fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWords(param, &count, &totalLength);
	//add size for the WORD_SEP between words and for '\0'
	fixParamStr = (char*)calloc(totalLength + count, sizeof(char));
	if (!fixParamStr)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	int len;
	for (int i = 0; i < count; i++)
	{
		len = (int)strlen(wordsArray[i]);
		wordsArray[i][0] = toupper(wordsArray[i][0]); //upper
		for (int j = 1; j < len - 1; j++)
		{
			wordsArray[i][j] = tolower(wordsArray[i][j]); //small
		}
		if (count == 1)
			wordsArray[i][len - 1] = toupper(wordsArray[i][len - 1]); //upper
		else
			wordsArray[i][len - 1] = tolower(wordsArray[i][len - 1]); //small

		strcat(fixParamStr, wordsArray[i]);
		if (i != count - 1) //not last
			strcat(fixParamStr, WORD_SEP);
	}

	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return fixParamStr;

}

void freeElements(char** elements, int count)
{
	for (int i = 0; i < count; i++)
		free(elements[i]);
	free(elements);
}

int checkElements(char** elements, int count)
{
	if (count != 3)
		return 0;
	//check second word is a number;
	char* streetNum = elements[1];
	while (*streetNum)
	{
		if (!isdigit(*streetNum))
			return 0;
		streetNum++;
	}
	return 1;
}

int saveAddressToBinFile(const Address* pBusinessAddress, FILE* fp)
{
	if (!writeIntToFile(pBusinessAddress->buildingNum, fp, "Error writing building number to binary file\n"))
		return 0;

	if (!writeStringToFile(pBusinessAddress->street, fp, "Error writing street name to binary file\n"))
		return 0;

	if (!writeStringToFile(pBusinessAddress->city, fp, "Error writing city name to binary file\n"))
		return 0;

	return 1;
}

int readAddressFromBinFile(Address* pBusinessAddress, FILE* fp)
{
	if (!readIntFromFile(&pBusinessAddress->buildingNum, fp, "Error reading building number for binary file\n"))
		return 0;

	pBusinessAddress->street = readStringFromFile(fp, "Error reading street name from binary file\n");
	if (!pBusinessAddress->street)
		return 0;

	pBusinessAddress->city = readStringFromFile(fp, "Error reading city name from binary file\n");
	if (!pBusinessAddress->city)
		return 0;

	return 1;
}


int saveAddressToTxtFile(const Address* pAddress, FILE* fp)
{
	if (!fprintf(fp, "%d\n", pAddress->buildingNum))
		return 0;

	if (!fprintf(fp, "%s\n", pAddress->street))
		return 0;

	if (!fprintf(fp, "%s\n", pAddress->city))
		return 0;

	return 1;
}

int readAddressFromTxtFile(Address* pAddress, FILE* fp)
{
	if (!fscanf(fp, "%d\n", &pAddress->buildingNum))
		return 0;

	if (!fscanf(fp, "%s\n", pAddress->street))
		return 0;

	if (!fscanf(fp, "%s\n", pAddress->city))
		return 0;

	return 1;
}


void printAddress(const Address* address)
{
	printf("%s , %d , %s\n", address->street, address->buildingNum, address->city);
}


void 	freeAddress(Address* pAddress)
{
	free(pAddress->street);
	free(pAddress->city);
}






//int saveBusinessAddressToCompressedBinary(Address* pManagerAddress, const char* fileName)
//{
//	FILE* fp = fopen(fileName, "wb");
//	if (!fp)
//	{
//		printf("Error opening file to write compressed data\n");
//		return ERROR;
//	}
//
//	BYTE addressData[2];
//	int cityLength = (int)strlen(pManagerAddress->city);
//	int streetLength = (int)strlen(pManagerAddress->street);
//
//	addressData[0] = (pManagerAddress->buildingNum << 1) | ((cityLength >> 3) & 0x1);
//	addressData[1] = ((cityLength & 0x7) << 5) | (streetLength & 0x1F);
//
//	if (fwrite(&addressData, sizeof(BYTE), 2, fp) != 2)
//		return 0;
//
//	if (fwrite(pManagerAddress->city, sizeof(char), cityLength, fp) != cityLength)
//		return 0;
//
//	if (fwrite(pManagerAddress->street, sizeof(char), streetLength, fp) != streetLength)
//		return 0;
//
//	return 1;
//}
