#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "Inventory.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"


int getValidQuantity() 
{
    int quantity;
    char input[MAX_STR_LEN]; // Buffer to store user input

    do {
        printf("Quantity: ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
        }
        if (sscanf(input, "%d", &quantity) != 1 || quantity <= 0) {
            printf("Invalid input. Please enter a valid positive integer.\n");
        }
        else {
            // Check if the input contains a decimal point
            char* decimalPointPos = strchr(input, '.');
            if (decimalPointPos != NULL) {
                printf("Invalid input. Quantity must be an integer.\n");
                quantity = 0; // Reset quantity
            }
        }
    } while (quantity <= 0);

    return quantity;
}

float getValidPrice()
{
    float price;
    char input[MAX_STR_LEN]; // Buffer to store user input

    /*do {
        printf("Price: ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
        }
        if (sscanf(input, "%f", &price) != 1 || price <= 0) {
            printf("Invalid input. Please enter a valid positive number.\n");
        }
    } while (price <= 0);*/

    do {
        printf("Price: ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
            exit(EXIT_FAILURE);
        }

        // Replace common signs with dot
        char* p = input;
        while (*p) {
            if (*p == ',' || *p == '+' || *p == '-') {
                *p = '.';
            }
            p++;
        }

        if (sscanf(input, "%f", &price) != 1 || price <= 0) {
            printf("Invalid input. Please enter a valid positive number.\n");
        }
    } while (price <= 0);


    return price;
}

void getValidItemName(char itemName[MAX_STR_LEN])
{
    do {
        printf("Item name: ");
        fgets(itemName, MAX_STR_LEN, stdin);
        // Remove newline character from input
        itemName[strcspn(itemName, "\n")] = '\0';
    } while (!isalpha(itemName[0]));
}

void initBusinessInventory(Inventory* pInventory) {
    printf("------------ Initializing Inventory ----------------- \n");
    int numItems;
    do {
        printf("Enter how many items to add into inventory: ");
        if (scanf("%d", &numItems) != 1 || numItems <= 0 || numItems > MAX_INVENTORY_ITEMS) {
            printf("Invalid input. Please enter a positive integer between 1 and %d.\n", MAX_INVENTORY_ITEMS);
            // Clear input buffer
            while (getchar() != '\n');
        }
    } while (numItems <= 0 || numItems > MAX_INVENTORY_ITEMS);

    SCREEN_CLR;
    pInventory->numItems = numItems;
    int change;
    do {
        int newNumItems;
        printf("You added %d items.\n", numItems);
        printf("Do you want to change how many items to insert?\n");
        printf("1 - Change, 2 - Stop and proceed with current number, else - Continue\n");
        scanf("%d", &change);
        while (getchar() != '\n');

        if (change == 1) {
            do {
                printf("Enter the new number of items to add: ");
                scanf("%d", &newNumItems);
                // Check if the input is positive
                if (newNumItems <= 0 || newNumItems > MAX_INVENTORY_ITEMS) {
                    printf("Invalid input. Please enter a positive integer between 1 and %d.\n", MAX_INVENTORY_ITEMS);
                    // Clear input buffer
                    while (getchar() != '\n');
                    continue; // Continue the loop to prompt for input again
                }
                // Clear input buffer
                while (getchar() != '\n');

                if (newNumItems <= 0 || newNumItems > MAX_INVENTORY_ITEMS) {
                    printf("Invalid number of items. Please enter a positive integer between 1 and %d.\n", MAX_INVENTORY_ITEMS);
                }
            } while (newNumItems <= 0 || newNumItems > MAX_INVENTORY_ITEMS);
            numItems = newNumItems;
        }
        else if (change == 2) {
            break;
        }
    } while (change == 1);

    // Now proceed with initializing the items
    for (int i = 0; i < numItems; i++) {
        printf("Enter details for item %d:\n", i + 1);
        // Input validation for item name (only characters)
        getValidItemName(pInventory->items[i].itemName);

        // Input validation for quantity (positive integer)
        pInventory->items[i].quantity = getValidQuantity();

        // Input validation for price (positive float or integer)
        pInventory->items[i].price = getValidPrice();

        printf("\n");
    }

    printf("-----------> Inventory initialized successfully.\n");
}

void printBusinessInventory(const Inventory* pInventory) 
{
    printf("There are %d items in Inventory:\n\n", pInventory->numItems);
    printf("%-20s | %-10s | %-10s\n", "Item name", "Quantity", "Price");
    printf("%s\n", SPACE_BARS);

    for (int i = 0; i < pInventory->numItems; i++)
        printf("%-20s | %-10d | %-10.2f\n", pInventory->items[i].itemName, pInventory->items[i].quantity,
            pInventory->items[i].price);
}

void printParts(Inventory* pInventory)
{
    printf("Inventory parts:\n");
    printf("%-20s %-10s %s\n", "Item name", "quantity", "price");
    printf("%s\n\n", SPACE_BARS);

    for (int i = 0; i < pInventory->numItems; i++)
        printf("%-20s %-10d %.2f\n", pInventory->items[i].itemName, pInventory->items[i].quantity,
        pInventory->items[i].price);
}


int checkInventory(Inventory* pInventory, const char* name, const int count)
{
    for (int i = 0; i < pInventory->numItems; i++)
    {
        if (pInventory->items[i].quantity <= 0)
            return ITEMS_QUATITY_0;

        if (strcmp(pInventory->items[i].itemName, name) == 0)
        {
            if (pInventory->items[i].quantity >= count)
                return 1;
        }
    }

    return 0;
}

int checkItemName(const Inventory* pInventory, const char* name)
{
    for (int i = 0; i < pInventory->numItems; i++)
    {
        if (strcmp(pInventory->items[i].itemName, name) == 0)
        {
            printf("found item with name %s\n\n", name);
            return 1;
        }
    }
    printf("Name does not exists in inventory\n");
    return 0;
}

float   getPriceByItemName(const Inventory* pInventory, const char* itemName)
{
    float basePrice = 10.0;
    for (int i = 0; i < pInventory->numItems; i++)
    {
        if (strcmp(pInventory->items[i].itemName, itemName) == 0)
            return pInventory->items[i].price;
    }

    return basePrice;
}

void updateInventory(Inventory* pInventory, const char* name, int count)
{
    for (int i = 0; i < pInventory->numItems; i++)
    {
        if (strcmp(pInventory->items[i].itemName, name) == 0)
        {
            pInventory->items[i].quantity -= count; // Decrease quantity by count
            break; // No need to continue searching once the item is found
        }
    }
}



int saveBusinessInventoryToFile(const Inventory* pBusinessInventory, FILE* fp)
{
    // Write the number of items in the inventory
    if (!writeIntToFile(pBusinessInventory->numItems, fp, "Error writing number of items.")) return 0;

    // Iterate over each item in the inventory
    for (int i = 0; i < pBusinessInventory->numItems; i++) {
        const InventoryItem* item = &(pBusinessInventory->items[i]); // Adjusted pointer declaration

        // Write the item's fields to the file
        if (!writeStringToFile(item->itemName, fp, "Error writing item name.")) return 0;
        if (!writeIntToFile(item->quantity, fp, "Error writing item quantity.")) return 0;
        if (fwrite(&item->price, sizeof(float), 1, fp) != 1)
        {
            puts("error writing price");
            return 0;
        }
    }

    return 1;
}



int readBusinessInventoryFromBinFile(Inventory* pBusinessInventory, FILE* fp)
{
    // Read the number of items in the inventory
    if (!readIntFromFile(&pBusinessInventory->numItems, fp, "Error reading number of items."))
        return 0;

    // Iterate over each item in the inventory
    for (int i = 0; i < pBusinessInventory->numItems; i++) {
        InventoryItem* item = &(pBusinessInventory->items[i]);

        // Read the item's fields from the file
        char* itemName = readStringFromFile(fp, "Error reading item name.");
        if (!itemName)
            return 0;

        // Copy the read string into the itemName array
        strncpy(item->itemName, itemName, MAX_STR_LEN - 1);
        item->itemName[MAX_STR_LEN - 1] = '\0'; // Ensure null termination

        // Free the dynamically allocated memory for itemName
        free(itemName);

        if (!readIntFromFile(&item->quantity, fp, "Error reading item quantity."))
            return 0;

        if (fread(&item->price, sizeof(float), 1, fp) != 1) {
            puts("Error reading item price.");
            return 0;
        }
    }

    return 1;
}


int savingBusinessInventoryToTxtFile(const Inventory* pBusinessInventory, FILE* fp)
{
    // Write the number of items in the inventory to the file
    fprintf(fp, "%d\n", pBusinessInventory->numItems);

    // Iterate over each item in the inventory
    for (int i = 0; i < pBusinessInventory->numItems; i++) {
        // Write the item's fields to the file
        fprintf(fp, "%s %d %.2f\n", pBusinessInventory->items[i].itemName,
            pBusinessInventory->items[i].quantity,
            pBusinessInventory->items[i].price);
    }

    return 1; 

}

int readingBusinessInventoryFromTxtFile(Inventory* pBusinessInventory, FILE* fp)
{
    // Read the number of items in the inventory from the file
    if (fscanf(fp, "%d", &pBusinessInventory->numItems) != 1) {
        printf("Error reading number of items.\n");
        return 0; // Return failure if unable to read the number of items
    }

    // Iterate over each item in the inventory
    for (int i = 0; i < pBusinessInventory->numItems; i++) {
        // Read the item's fields from the file
        if (fscanf(fp, "%s %d %f",
            pBusinessInventory->items[i].itemName,
            &pBusinessInventory->items[i].quantity,
            &pBusinessInventory->items[i].price) != 3) {
            printf("Error reading item data.\n");
            return 0; // Return failure if unable to read item data
        }
    }

    return 1;
}
