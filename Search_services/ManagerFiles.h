#pragma once

#include <stdio.h>
#include "BusinessManager.h"


int     saveAllDataToFiles(const BusinessManager* pManager, const char* programFileTxt,
	const char* programFileBin, const char* managerWorkHoursCompressed);


int readAllDataFromFile(BusinessManager* pManager, const char* programFileTxt,
	const char* programFileBin, const char* managerWorkHoursCompressed, int isBinary);

int saveProgramToUncompressedBinary(const BusinessManager* pManager, const char* programFileBin);

int saveManagerAndBusinessToTxt(const BusinessManager* pManager, const char* programFileTxt);

int saveBusinessWorkHours(const BusinessManager* pManager, const char* fileName);




int readProgramFromBinaryFiles(BusinessManager* pManager, FILE* binary_fp, FILE* compressed_fp);

int readBusinessesFromBinFile(BusinessManager* pManager, FILE* binary_fp, FILE* compressed_fp);

int readManagerAndBusinessFromTxt(BusinessManager* pManager, const char* programFileTxt);









