#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "General.h"
#include "FileHelper.h"
#include "Inventory.h"
#include "WorkSchedule.h"
#include "ManagerFiles.h"
#include "Macros.h"


// ratingsReviews, businessOrder, customersArr


int     saveAllDataToFiles(const BusinessManager* pManager, const char* programFileTxt,
	const char* programFileBin, const char* managerWorkHoursCompressed)
{
	if (!saveProgramToUncompressedBinary(pManager, programFileBin))
		return ERROR_WRITING_UNCOMPRESSED;
	else if
		(!saveBusinessWorkHours(pManager, managerWorkHoursCompressed))
		return ERROR_WRITING_COMPRESSED;
	else if
		(!saveManagerAndBusinessToTxt(pManager, programFileTxt))
		return ERROR_WRITING_TXT;
	else
		return SUCCRESS;

}

int readAllDataFromFile(BusinessManager* pManager, const char* programFileTxt,
	const char* programFileBin, const char* managerWorkHoursCompressed, int isBinary)
{
	FILE* binFile = fopen(programFileBin, "rb");
	if (!binFile)
	{
		printf("Error opening binary file to read from\n");
		return ERROR;
	}

	FILE* compressedBinFile = fopen(managerWorkHoursCompressed, "rb");
	if (!compressedBinFile)
	{
		printf("Error opening compressed bin file to red from\n");
		return 0;
	}

	if (isBinary)
	{
		if (!readProgramFromBinaryFiles(pManager, binFile, compressedBinFile))
			return ERROR_READING_BINARY_FILES;

	}

	else
	{
		if (!readManagerAndBusinessFromTxt(pManager, programFileTxt))
			return ERROR_READING_TXT_FILE;

	}
		
	return SUCCRESS;
		
}


int saveProgramToUncompressedBinary(const BusinessManager* pManager, const char* programFileBin)
{
	FILE* fp = fopen(programFileBin, "wb");
	if (!fp)
	{
		printf("Error opening bin file to write in\n");
		return ERROR;
	}

	if (!writeStringToFile(pManager->name, fp, "Error writing manager name to file\n"))
	{
		fclose(fp);
		return 0;
	}

	if (!saveAddressToBinFile(&pManager->address, fp))
	{
		fclose(fp);
		return 0;
	}

	if (!writeIntToFile((int)pManager->sortOpt, fp, "Error writing manager sort options to file\n"))
	{
		fclose(fp);
		return 0;
	}

	if (!writeIntToFile(pManager->businessesCount, fp, "Error writing businesses count to file\n"))
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (!saveBusinessToBinFile(pManager->businessesArr[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}

	return 1;
}

int saveBusinessWorkHours(const BusinessManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
	{
		printf("Error opening compressed .bin file\n");
		return 0;
	}

	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (!saveBusinessWorkHoursToCompressedBinFile(&pManager->businessesArr[i]->workHours, fp))
		{
			printf("Error saving business %s workHours\n", pManager->businessesArr[i]->businessName);
			return 0;
		}
	}

	return 1;
}



int readProgramFromBinaryFiles(BusinessManager* pManager, FILE* binary_fp, FILE* compressed_fp)
{
	pManager->name = readStringFromFile(binary_fp, "Error reading manager name from binary file\n");
	if (!pManager->name)
	{
		fclose(binary_fp);
		fclose(compressed_fp);
		return 0;
	}

	if (!readAddressFromBinFile(&pManager->address, binary_fp))
	{
		free(pManager->name);
		fclose(binary_fp);
		fclose(compressed_fp);
		return 0;
	}

	int opt;
	if (!readIntFromFile(&opt, binary_fp, "Error reading manager sorting option from binary file\n"))
	{
		free(pManager->name);
		fclose(binary_fp);
		fclose(compressed_fp);
		return 0;
	}

	if (!readIntFromFile(&pManager->businessesCount, binary_fp, "Error reading business count from binary file\n"))
	{
		free(pManager->name);
		fclose(binary_fp);
		fclose(compressed_fp);
		return 0;
	}

	return readBusinessesFromBinFile(pManager, binary_fp, compressed_fp);
}


int readBusinessesFromBinFile(BusinessManager* pManager, FILE* binary_fp, FILE* compressed_fp)
{
	if (pManager->businessesCount > 0)
	{
		pManager->businessesArr = (Business**)malloc(pManager->businessesCount * sizeof(Business*));
		if (!pManager->businessesArr)
		{
			free(pManager->name);
			printf("Business arr memory allocation error\n");
			fclose(binary_fp);
			fclose(compressed_fp);
			return 0;
		}
	}

	for (int i = 0; i < pManager->businessesCount; i++)
	{
		pManager->businessesArr[i] = (Business*)calloc(1, sizeof(Business));
		if (!pManager->businessesArr[i])
		{
			printf("Allocation for Business - ERROR\n");
			for (int j = 0; j < i; j++)
				free(pManager->businessesArr[i]);
			
			free(pManager->businessesArr);
			free(pManager->name);
			fclose(binary_fp);
			fclose(compressed_fp);
			return 0;
		}


		int res = readBusinessFromBinFiles(pManager->businessesArr[i], binary_fp, compressed_fp);
		if (!res)
		{
			for (int j = 0; j < i; j++)
				free(pManager->businessesArr[i]);

			free(pManager->businessesArr);
			free(pManager->name);
			fclose(binary_fp);
			fclose(compressed_fp);
			return 0;
		}
	}

	return 1;
}

int saveManagerAndBusinessToTxt(const BusinessManager* pManager, const char* programFileTxt)
{
	FILE* fp = fopen(programFileTxt, "w");
	if (!fp)
	{
		printf("Error opening file to write txt data\n");
		return 0;
	}

	if (!fprintf(fp, "%s\n", pManager->name))
	{
		printf("Error saving manager name to txt file\n");
		fclose(fp);
		return 0;
	}

	if (!fprintf(fp, "%d\n", pManager->businessesCount))
	{
		printf("Error saving business count to txt file\n");
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pManager->businessesCount; i++)
	{
		if (!saveBusinessToTxtFile(pManager->businessesArr[i], fp))
		{
			printf("Error saving business to txt file\n");
			return 0;
		}
	}

	if (!saveAddressToTxtFile(&pManager->address, fp))
	{
		printf("Error saving manager address to .txt file\n");
		return 0;
	}



	// Save eSortOption
	fprintf(fp, "%d\n", pManager->sortOpt);

	return 1;

}


int readManagerAndBusinessFromTxt(BusinessManager* pManager, const char* programFileTxt)
{

	FILE* fp = fopen(programFileTxt, "r");
	if (!fp)
	{
		printf("Error opening file to read txt data\n");
		return 0;
	}

	if (!fscanf(fp, "%s\n", pManager->name))
	{
		printf("Error reading manager name from txt file\n");
		fclose(fp);
		return 0;
	}

	if (!fscanf(fp, "%d\n", &pManager->businessesCount))
	{
		free(pManager->name);
		printf("Error reading business count from txt file\n");
		fclose(fp);
		return 0;
	}

	pManager->businessesArr = (Business**)malloc(pManager->businessesCount * sizeof(Business*));
	if (!pManager->businessesArr)
	{
		free(pManager->name);
		printf("Error allocating memory for businessArr\n");
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pManager->businessesCount; i++)
	{
		pManager->businessesArr[i] = (Business*)calloc(1, sizeof(Business));
		if (!pManager->businessesArr[i])
		{
			for (int j = 0; j < i; j++)
				free(pManager->businessesArr[j]);
			free(pManager->businessesArr);

			free(pManager->name);
			printf("Error allocating memory for business\n");
			fclose(fp);
			return 0;
		}

		if (!readBusinessFromTxtFile(pManager->businessesArr[i], fp))
		{
			for (int j = 0; j < i; j++)
				free(pManager->businessesArr[j]);
			free(pManager->businessesArr);

			printf("Error reading business from txt file\n");
			fclose(fp);
			return 0;
		}
	}

	if (!readAddressFromTxtFile(&pManager->address, fp))
	{
		for (int i = 0; i < pManager->businessesCount; i++)
			free(pManager->businessesArr[i]);
		free(pManager->businessesArr);

		free(pManager->name);
		printf("Error reading manager address from txt file\n");
		fclose(fp);
		return 0;
	}

	int sortOpt;
	if (!fscanf(fp, "%d\n", &sortOpt))
	{
		for (int i = 0; i < pManager->businessesCount; i++)
			free(pManager->businessesArr[i]);
		free(pManager->businessesArr);

		free(pManager->name);
		printf("Error reading manager sort options from txt file\n");
		fclose(fp);
		return 0;
	}

	pManager->sortOpt = sortOpt;

	return 1;
}









// IMPROVEMENT OVER readManagerAndBusinessFromTxt: 


/*



void cleanup(BusinessManager* pManager) {
	if (pManager) {
		if (pManager->name) {
			free(pManager->name);
			pManager->name = NULL;
		}
		if (pManager->businessesArr) {
			for (int i = 0; i < pManager->businessesCount; i++) {
				if (pManager->businessesArr[i]) {
					free(pManager->businessesArr[i]);
					pManager->businessesArr[i] = NULL;
				}
			}
			free(pManager->businessesArr);
			pManager->businessesArr = NULL;
		}
	}
}

int readManagerAndBusinessFromTxt(BusinessManager* pManager, const char* programFileTxt) {
	FILE* fp = fopen(programFileTxt, "r");
	if (!fp) {
		printf("Error opening file to read txt data\n");
		return 0;
	}

	if (!fscanf(fp, "%s\n", pManager->name)) {
		printf("Error reading manager name from txt file\n");
		fclose(fp);
		return 0;
	}

	if (!fscanf(fp, "%d\n", &pManager->businessesCount)) {
		printf("Error reading business count from txt file\n");
		cleanup(pManager);
		fclose(fp);
		return 0;
	}

	pManager->businessesArr = (Business**)malloc(pManager->businessesCount * sizeof(Business*));
	if (!pManager->businessesArr) {
		printf("Error allocating memory for businessArr\n");
		cleanup(pManager);
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pManager->businessesCount; i++) {
		pManager->businessesArr[i] = (Business*)calloc(1, sizeof(Business));
		if (!pManager->businessesArr[i]) {
			printf("Error allocating memory for business\n");
			cleanup(pManager);
			fclose(fp);
			return 0;
		}

		if (!readBusinessFromTxtFile(pManager->businessesArr[i], fp)) {
			printf("Error reading business from txt file\n");
			cleanup(pManager);
			fclose(fp);
			return 0;
		}
	}

	if (!readAddressFromTxtFile(&pManager->address, fp)) {
		printf("Error reading manager address from txt file\n");
		cleanup(pManager);
		fclose(fp);
		return 0;
	}

	int sortOpt;
	if (!fscanf(fp, "%d\n", &sortOpt)) {
		printf("Error reading manager sort options from txt file\n");
		cleanup(pManager);
		fclose(fp);
		return 0;
	}

	pManager->sortOpt = sortOpt;

	fclose(fp);
	return 1;
}

*/