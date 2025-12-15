#pragma once

#define ERROR_WRITING_COMPRESSED	2
#define ERROR_WRITING_UNCOMPRESSED	3
#define ERROR_WRITING_TXT			4
#define ERROR_READING_BINARY_FILES	5
#define ERROR_READING_TXT_FILE		6


#define SUCCRESS					1
#define ERROR						0

#define MAX_CELLPHONE_NUMBER		10


#define CLEAN_BUFFER()	{getchar(); }

#define CHECK_RETURN_0(value) \
    if (value == NULL) {      \
        return 0;             \
    }

#define CHECK_MSG_RETURN_0(value, msg) \
    if (value == NULL) {               \
        puts(msg);                     \
        return 0;                      \
    }

#define RETURN_MSG_0(msg){  \
        puts(msg)           \
        return 0;           \
    } 
    
#define CHECK_RETURN_NULL(value){   \
    if(value==NULL)                 \
    return NULL;                    \
    }

#define CLR_SCREEN_INPUT {  \
    for (int i = 0; i < SCREEN_CLR; i++)   \
        printf("\n");                      \
    }

#define CHECK_NOT_NULL_FREE(value){ \
    if (value != NULL)              \
        free(value);                \
   }