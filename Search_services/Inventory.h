#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_INVENTORY_ITEMS 300 
#include "General.h"

typedef struct {
    char itemName[MAX_STR_LEN];
    int quantity;
    float price;
} InventoryItem;

typedef struct {
    InventoryItem items[MAX_INVENTORY_ITEMS];
    int numItems;
} Inventory;

void    printParts(Inventory* pInventory);
void    initBusinessInventory(Inventory* pInventory);
void    printBusinessInventory(const Inventory* pInventory);
int     checkInventory(Inventory* pInventory, const char* name, const int count);
float   getPriceByItemName(const Inventory* pInventory, const char* itemName);
void    updateInventory(Inventory* pInventory, const char* name, int count);
int     checkItemName(const Inventory* pInventory, const char* name);


int saveBusinessInventoryToFile(const Inventory* pBusinessInventory, FILE* fp);
int readBusinessInventoryFromBinFile(Inventory* pBusinessInventory, FILE* fp);

int savingBusinessInventoryToTxtFile(const Inventory* pBusinessInventory, FILE* fp);
int readingBusinessInventoryFromTxtFile(Inventory* pBusinessInventory, FILE* fp);


void getValidItemName(char itemName[MAX_STR_LEN]);
float getValidPrice();
int getValidQuantity();

#endif /* INVENTORY_H */

