/*
 *      filename:       dlinkedlist.c
 *      author:         @keiferchiang
 *      date:           2 May 2018
 *      version:        0.0.1
 *
 *      description:    Implementation of the DLinkedList module
 *
 *      note:           This implementation of DLinkedList is
 *                      lightly optimized for end-node manipulation,
 *                      trading off memory conservation. The
 *                      optimization allows the DLinkedList to be a
 *                      base for LIFO and FIFO data structures while
 *                      allowing said structures to run most
 *                      operations with a time complexity of O(1).
 *                      list_start and list_end keep track of the
 *                      effective client-side linked list, whereas
 *                      front and tail keep track of the entire
 *                      malloc'd list. See "design" for an example of
 *                      a three-element linked list. 
 * 
 *      design:         front <-> [ ] <-> [ ] <-> [ ] <-> [ ] <-> tail
 *                                         ^               ^
 *                                    list_start       list_end
 */

#include "dlinkedlist.h"

/*-------------------------------------
 * Representation
 -------------------------------------*/
typedef struct node_t {
        void *elem;
        struct node_t *next;
        struct node_t *prev;
} *Node_T;

struct dlinkedlist_t {
        Node_T front;
        Node_T tail;
        Node_T list_start;
        Node_T list_end;
        unsigned capacity;
        unsigned size;
};

/*-------------------------------------
 * Helper/Private Prototypes
 -------------------------------------*/
/*
 * Mallocs and inits a new node
 */
Node_T Node_new(Node_T prev, Node_T next, void *elem);

/*
 * Removes the current node from the list and frees it
 */
void Node_free(DLinkedList_T list, Node_T *curr);

/*
 * Mallocs "hint" empty nodes. Helper to constructor
 */
Node_T malloc_hint(Node_T prev, unsigned hint);

/*
 * Removes the given node from the list. Helper to 
 * DLinkedList_remove
 */
void remove_node(DLinkedList_T list, Node_T curr);

/*-------------------------------------
 * Function Definitions
 -------------------------------------*/
DLinkedList_T DLinkedList_new(unsigned hint)
{
        DLinkedList_T list;

        list = malloc(sizeof(struct dlinkedlist_t));
        assert(list != NULL);

        list->capacity = hint;
        list->size = 0;
        list->front = malloc_hint(NULL, hint);
        list->tail = list->front + (hint * sizeof(struct node_t));
        list->list_start = list->front;
        list->list_end = list->front;

        return list;
}

void DLinkedList_free(DLinkedList_T *list)
{
        assert(list != NULL);
        assert(*list != NULL);

        while((*list)->capacity > 0)
                Node_free(*list, &((*list)->front));

        (*list)->front = NULL;
        (*list)->tail = NULL;
        (*list)->list_start = NULL;
        (*list)->list_end = NULL;

        free(*list);
        *list = NULL;
}

//////////////////////////////////
//      Getter Functions        //
//////////////////////////////////
unsigned DLinkedList_length(DLinkedList_T list)
{
        (void) list;
        return 0;
}

void *DLinkedList_get(DLinkedList_T list, unsigned index)
{
        (void) list, (void) index;
        return NULL;
}

void *DLinkedList_gethi(DLinkedList_T list)
{
        (void) list;
        return NULL;
}

void *DLinkedList_getlo(DLinkedList_T list)
{
        (void) list;
        return NULL;
}

//////////////////////////////////
//      Setter Functions        //
//////////////////////////////////
void DLinkedList_set(DLinkedList_T list, void *elem, unsigned index)
{
        (void) list, (void) elem, (void) index;
}

void DLinkedList_sethi(DLinkedList_T list, void *elem)
{
        (void) list, (void) elem;
}

void DLinkedList_setlo(DLinkedList_T list, void *elem)
{
        (void) list, (void) elem;
}

//////////////////////////////////
//      Remove Functions        //
//////////////////////////////////
void DLinkedList_remove(DLinkedList_T list, unsigned index)
{
        (void) list, (void) index;
}

void DLinkedList_removehi(DLinkedList_T list)
{
        (void) list;
}

void DLinkedList_removelo(DLinkedList_T list)
{
        (void) list;
}

/*-------------------------------------
 * Helper/Private Definitions
 -------------------------------------*/
Node_T Node_new(Node_T prev, Node_T next, void *elem)
{
        Node_T node = NULL;

        node = malloc(sizeof(struct node_t));
        assert(node != NULL);

        node->elem = elem;
        node->prev = prev;
        node->next = next;

        return node;
}

void Node_free(DLinkedList_T list, Node_T *curr)
{
        Node_T temp = NULL;

        assert(list != NULL);
        assert(curr != NULL);
        assert(*curr != NULL);
        assert(list->capacity != 0);

        temp = *curr;

        if (list->capacity > 1) {
                if (temp == list->front) {
                        (temp->next)->prev = NULL;
                        *curr = temp->next;
                } else if (temp == list->tail) {
                        (temp->prev)->next = NULL;
                        *curr = (temp)->prev;
                } else {
                        ((*curr)->next)->prev = (*curr)->prev;
                        (((*curr)->next)->prev)->next = (*curr)->next;
                }
        }

        list->capacity--;
        temp->elem = NULL;
        temp->next = NULL;
        temp->prev = NULL;
        free(temp);
        temp = NULL;
}

Node_T malloc_hint(Node_T prev, unsigned hint)
{
        Node_T node = NULL;

        if (hint == 0)
                return NULL; 

        node = Node_new(prev, malloc_hint(node, hint - 1), NULL);
        assert(node != NULL);

        return node;
}

void remove_node(DLinkedList_T list, Node_T curr)
{
        assert(list != NULL);
        assert(curr != NULL);
        assert(list->size != 0);

        if (list->size > 1) {
                if (curr == list->list_start) {
                        list->list_start = curr->next;
                } else if (curr == list->list_end) {
                        list->list_end = curr->prev;
                } else {
                        Node_free(list, &curr);  
                }
        }

        list->size--;
}
