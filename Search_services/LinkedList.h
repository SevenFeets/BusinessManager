/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_


/*** Definitions ***/
typedef void* DATA;   // works with every data

typedef enum { False, True } BOOL;	// a boolean type


// Node
typedef struct node
{
	DATA			key;
	struct node* next;
}LIST_NODE;


// List
typedef struct
{
	LIST_NODE head;
}LIST;


/*** Function prototypes ***/
// create new list
BOOL L_init(LIST* pList);

// add new node after *pNode
LIST_NODE* L_insert(LIST_NODE* pNode, DATA Value);

// erase node after *pNode
BOOL L_delete(LIST_NODE* pNode, void freeFunc(void*));

// return a pointer to the node 
const LIST_NODE* L_find(const LIST_NODE* pNode, DATA Value, int compare(const void*, const void*));

// free list memory
BOOL L_free(LIST* pList, void freeFunc(void*));

// print the list content
int L_print(const LIST* pList, void (*print)(const void*));
#endif
