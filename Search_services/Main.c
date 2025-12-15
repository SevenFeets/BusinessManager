#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>

#include "Main.h"
#include "BusinessManager.h"
#include "User.h"
#include "Business.h"
#include "ManagerFiles.h"
#include "Macros.h"




const char* mainMenu[eGeneralOptions] = 
{   "Add business", "Add customer", "Do business", "Add Review", 
    "Find business", "Sort business", "Show business full review",
    "Show top business", "print all users",
    "Print all businesses", "Print business by type"
};



int main() 
{
    int option, stop = 0;
    int opt;

    printf("Enter: 1 -> load from txt\t 2 -> load from bin files\n\nOther number -> load manually\n");
    do {
        if (scanf("%d", &opt) == 1) {
        // Check if the input is a number
        stop = 1;
        }

        else {
        // If input is not a number, clear the input buffer
        while (getchar() != '\n');
        printf("Enter only a number\n");
        }

    } while (!stop);
    
    BusinessManager     manager;
   
    if (!initManager(&manager, PROGRAM_FILE_TEXT, PROGRAM_FILE_BIN, MANAGER_ADDRESS_COMPRESSED, opt))
    {
        printf("Error init Business manager\n");
        return 1;
    }
    
    stop = 0;
    do {
        
        option = showGlobalMenu();
        switch (option) {
        case eAddBusiness:
            if (!addBusiness(&manager))
                printf("Error adding business\n");
            break;

        case eAddCustomer:
            if (!addCustomer(&manager))
                printf("Error adding customers\n");
            break;

        case eCustomerDoBusiness:
            if (!doBusiness(&manager))
                printf("Business was not made\n");
            break;

        case eAddReview:
            if (!addRatingReview(&manager))
                printf("Reviews wan not added\n");
            break;

        case efindBusiness:
            findBusiness(&manager);
            break;

        case eSortBusiness:
            sortBusiness(&manager);
            break;

        case eShowRatingsAndReviews:
            showRatingsReviews(&manager);
            break;

        case eShowTopBusinesses:
            showTopBusiness(&manager);
            break;

        case ePrintBusiness:
            printAllBusinesses(&manager);
            break;

        case ePrintUsers:
            printAllUsers(&manager);
            break;

        case ePrintBusinessByType:
            printBusinessByType(&manager);
            break;

        case EXIT:
            stop = 1;
            printf("Bye-bye\n\n"); // Quit the program
            break;

        default:
            printf("Invalid option, try again\n");
            break;
        }

    } while (!stop);

    int res = saveAllDataToFiles(&manager, PROGRAM_FILE_TEXT, PROGRAM_FILE_BIN, 
        MANAGER_ADDRESS_COMPRESSED);

    if (res == ERROR_WRITING_COMPRESSED)
        printf("Error saving compressed binary data to file\n");
    else if (res == ERROR_WRITING_UNCOMPRESSED)
        printf("Error saving uncompressed binary data to file\n");
    else if (res == ERROR_WRITING_TXT)
        printf("Error saving text data to file\n");
    else
        printf("All files successfully saved\n\n");

    freeManager(&manager);
    return 0;
}


int showGlobalMenu()
{
    int option;
    printf("\n\n");
    printf("Main Menu:\n");
    // Show options available for non-authorized users
    for (int i = 0; i < eGeneralOptions; i++)
        printf("\t%d - %s\n", i, mainMenu[i]);

    printf("%d - Quit\n", EXIT); // Quit option

    scanf("%d", &option);
    // clean buffer
    char tav;
    scanf("%c", &tav);
    return option;
}




























