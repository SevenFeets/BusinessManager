#ifndef __ADDRESS__
#define __ADDRESS__

typedef struct
{
	int			buildingNum;
	char*		street;
	char*		city;

}Address;



int		initAddress(Address* pAddress, const char* msg);
int		checkElements(char** elements, int count);
char*	fixAddressParam(char* param);
void	freeElements(char** elements, int count);
void	printAddress(const Address* address);
void 	freeAddress(Address* pAddress);


int saveAddressToTxtFile(const Address* pAddress, FILE* fp);
int readAddressFromTxtFile(Address* pAddress, FILE* fp);


int saveAddressToBinFile(const Address* pBusinessAddress, FILE* fp);
int readAddressFromBinFile(Address* pBusinessAddress, FILE* fp);



//int saveBusinessAddressToCompressedBinary(Address* pManagerAddress, const char* fileName);


#endif // !__ADDRESS__
