#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255
#define SCREEN_CLR	5
#define ID			9
#define ASTERICKS		"*****************************************************************"
#define SPACE_BARS		"-----------------------------------------------------------------"
#define SPCAES			"       "
#define MAX_RANDOM_NUMBER 999999998
#define MIN_RANDOM_NUMBER 000000001

#define ITEMS_QUATITY_0		-1

char*	getStrExactName(const char* msg);
char*	myGets(char* buffer, int size, FILE* source);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str, int* pCount, int* pTotalLength);
char**	splitCharsToWordsByDel(char* str, int* pCount, const char* del);
char*	getsStrFixSize(char* buffer, int size, const char* msg);

void	generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));
int		generateOrderID();






#endif // !__GENERAL__
