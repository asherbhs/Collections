#include <stddef.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include <limits.h> /* INT_MAX */
#include <stdio.h> /* fprintf(stderr, ...) */
#include <stdbool.h>

/* 
this file provides implementations of common data structures

generic types are supported by the use of void pointers

when instantiating a structure, the caller must provide the size_t of the type
of data to be contained in the collection

it is then the caller's responsibility to provide the correct type of data for
values in the collection in subsequent function calls, and to cast value returns
to the correct type
*/

/*
Collection requirements:
    new
    destroy
    add
    size
*/

/*
List requirements (on top of collection requirements):
    insert (add with index)
    get (by index)
    set (by index)
    remove (by index)
*/

/*
List Eq requirements (on top of list):
    contains
    removeFirst
    removeLast
    removeAll
    firstIndexOf
    lastIndexOf
*/

/*
Deque requirements (on top of collection requirements):
    push
    pushLast
    pop
    popLast 
    setFirst
    setLast
    getFirst
    getLast
*/

/*
Set requirements (on top of collection requirements):
    tbd
*/

/*
Map requirements:
    tbd
*/

/*
[/] array list (AList)
[/] array list with equals (AListEq) (equality function)
[ ] sorted array list (AListSort) (comparator function) ???

[ ] (doubly) linked list (LList) 
[ ] linked list with equals (LListEq) (equality function)
[ ] sorted linked list (LListSort) (comparator function) ???

[ ] hashmap (HMap) (hash function on keys)
[ ] hashset (HSet) (wrap hashmap)

[ ] treemap (TMap) (comparator on keys)
[ ] treeset (TSet) (wrap treemap)
*/

/* array list */
typedef struct AList {
    void * content;
    size_t cellSize;
    int capacity;
    int length;
} AList;

/*
takes the size of a data type, and an initial capacity

returns a pointer to a new dynamic array for storing elements with the given
size, or a null pointer if memory could not be allocated
*/
AList * AList_new(size_t cellSize, int initialCapacity) 
{
    AList * this = malloc(sizeof(AList));
    if (this == NULL) {
        fprintf(stderr, "AList_new: unable to allocate new array struct\n");
        free(this);
        return NULL;
    }

    this->content = malloc(cellSize * initialCapacity);
    if (this->content == NULL) {
        fprintf(stderr, "AList_new: unable to allocate new array content\n");
        free(this);
        return NULL;
    }

    if (initialCapacity < 0) {
        fprintf(stderr, "AList_new: initialCapacity is negative");
        free(this);
        return NULL;
    }

    this->cellSize = cellSize;
    this->capacity = initialCapacity;
    this->length = 0;
    return this;
} 

/*
takes a pointer to an array list and an index

if the index is valid, returns a pointer to the element at that index, else
returns a null pointer
*/
void * AList_get(AList * this, int index) 
{
    if (this == NULL) {
        fprintf(stderr, "AList_get: null pointer to AList\n");
        return NULL;
    }

    if (index >= this->length) {
        fprintf(stderr, "AList_get: index out of bounds\n");
        return NULL;
    }
    
    return this->content + this->cellSize * index;
}

/*
takes a pointer to an array list, an index, and the address of a value

sets the value the given index of the given array list to the given value

returns 0 on a success, and 1 on a failure (if the index is invalid)
*/
int AList_set(AList * this, int index, void * valuePointer) 
{
    if (this == NULL) {
        fprintf(stderr, "AList_set: null pointer to AList\n");
        return 1;
    }
    
    if (index < 0 || index >= this->length) {
        fprintf(stderr, "AList_set: index out of bounds\n");
        return 1;
    }

    void * p = AList_get(this, index);
    if (p == NULL) return 1;
    memcpy(p, valuePointer, this->cellSize);
    return 0;
}

/*
takes a pointer to an array list and returns the number of elements it contains,
or -1 if the pointer is null
*/
int AList_size(AList * this) 
{ 
    if (this == NULL) {
        fprintf(stderr, "AList_size: null pointer to AList\n");
        return -1;
    }
    return this->length; 
}

/*
takes a pointer to an array list and the address of a value

adds the given value to the given array list

returns 0 if the value was successfully added, or 1 if it couldn't be
*/
int AList_add(AList * this, void * valuePointer) 
{
    if (this == NULL) {
        fprintf(stderr, "AList_add: null pointer to AList\n");
        return 1;
    }

    if (this->length < this->capacity) {
        this->length++;
        return AList_set(this, this->length - 1, valuePointer);
    }

    if (this->capacity > INT_MAX * 0.5) {
        fprintf(stderr, "AList_add: unable to expand array\n");
        return 1;
    }

    this->capacity *= 2;
    void * newContent = realloc(this->content, this->capacity * this->cellSize);
    if (newContent == NULL) {
        fprintf(stderr, "AList_add: unable to reallocate array\n");
        return 1;
    }

    this->content = newContent;
    this->length++;
    return AList_set(this, this->length - 1, valuePointer);
}

/*
takes a pointer to an arraylist and frees all associated memory (including the
pointer itself)
*/
void AList_destroy(AList * this) 
{
    if (this == NULL) {
        fprintf(stderr, "AList_destroy: null pointer to AList\n");
        return;
    }

    free(this->content);
    free(this);
}

/*
takes a pointer to an array list and an index

removes the value at the index from the array list and shifts all successive
elements down to fill the new space

returns 0 on a success, and 1 on a failure (if the index is out of bounds)
*/
int AList_remove(AList * this, int index)
{
    if (this == NULL) {
        fprintf(stderr, "AList_remove: null pointer to AList\n");
        return 1;
    }
    
    if (index < 0 || index >= this->length) {
        fprintf(stderr, "AList_remove: index out of bounds");
        return 1;
    }

    for (int i = index; i < this->length - 1; i++) {
        AList_set(this, i, AList_get(this, i + 1));
    }

    this->length--;

    return 0;
}

/*
takes a pointer to an array list, an index, and the address of a value

inserts the value at the index in the array list, shifting values orignally at
and after the index to the right

returns 0 on a success, and 1 on a failure (if the index is out of bounds)
*/
int AList_insert(AList * this, int index, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AList_insert: null pointer to AList\n");
        return 1;
    }

    if (index < 0 || index > this->length) {
        fprintf(stderr, "AList_insert: index out of bounds");
        return 1;
    }
    
    void * new = malloc(this->cellSize);
    memcpy(new, valuePointer, this->cellSize);
    void * nextNew = malloc(this->cellSize);
    for (int i = index; i < this->length; i++) {
        memcpy(nextNew, AList_get(this, i), this->cellSize);
        memcpy(AList_get(this, i), new, this->cellSize);
        memcpy(new, nextNew, this->cellSize);
    }

    AList_add(this, new);
    free(new);
    free(nextNew);
}

void * AList_getArray(AList * this) {
    if (this == NULL) {
        fprintf(stderr, "AList_getArray: null pointer to AList\n");
        return NULL;
    }

    return AList_get(this, 0);
}

/* array list with equals function */

typedef struct AListEq {
    AList * list;
    bool (* eq)(void *, void *);
} AListEq;

AListEq * AListEq_new(
    bool (* eq)(void *, void *), 
    int cellSize, 
    int initialCapacity
) {
    AListEq * this = malloc(sizeof(AListEq));
    this->list = AList_new(cellSize, initialCapacity);

    if (this->list == NULL) {
        fprintf(stderr, "AListEq: could not create internal AList");
        free(this);
        return NULL;
    }

    if (eq == NULL) {
        fprintf(stderr, "AListEq: NULL function input");
        free(this);
        return NULL;
    }

    this->eq = eq;

    return this;
}

void * AListEq_get(AListEq * this, int index) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_get: null pointer to AListEq\n");
        return NULL;
    }

    return AList_get(this->list, index);
}

int AListEq_set(AListEq * this, int index, void * valuePointer) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_set: null pointer to AListEq\n");
        return 1;
    }

    return AList_set(this->list, index, valuePointer);
}

int AListEq_size(AListEq * this) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_size: null pointer to AListEq\n");
        return 1;
    }

    return AList_size(this->list);
}

int AListEq_add(AListEq * this, void * valuePointer) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_add: null pointer to AListEq\n");
        return 1;
    }
    
    return AList_add(this->list, valuePointer);
}

int AListEq_remove(AListEq * this, int index) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_remove: null pointer to AListEq\n");
        return 1;
    }

    return AList_remove(this->list, index);
}

int AListEq_insert(AListEq * this, int index, void * valuePointer) 
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_insert: null pointer to AListEq\n");
        return 1;
    }

    return AList_insert(this->list, index, valuePointer);
}

void AListEq_destroy(AListEq * this)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_destroy: null pointer to AListEq\n");
        return;
    }

    AList_destroy(this->list);
    free(this);
}

int AListEq_firstIndexOf(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_firstIndexOf: null pointer to AListEq\n");
        return -1;
    }

    for (int i = 0; i < AListEq_size(this); i++) {
        if (this->eq(AListEq_get(this, i), valuePointer)) {
            return i;
        }
    }

    return -1;
}

bool AListEq_contains(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_contains: null pointer to AListEq\n");
        return -1;
    }

    return AListEq_firstIndexOf(this, valuePointer) >= 0;
}

int AListEq_lastIndexOf(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_lastIndexOf: null pointer to AListEq\n");
        return -1;
    }

    for (int i = AListEq_size(this) - 1; i >= 0; i--) {
        if (this->eq(AListEq_get(this, i), valuePointer)) {
            return i;
        }
    }

    return -1;
}

bool AListEq_removeAll(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_removeAll: null pointer to AListEq\n");
        return false;
    }

    int i = 0;
    bool anyRemoved = false;
    while (i < AListEq_size(this)) {
        if (this->eq(AListEq_get(this, i), valuePointer)) {
            AListEq_remove(this, i);
            anyRemoved = true;
        } else {
            i++;
        }
    }
    return anyRemoved;
}

bool AListEq_removeFirst(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_removeFirst: null pointer to AListEq\n");
        return false;
    }
    
    int i = AListEq_firstIndexOf(this, valuePointer);
    if (i == -1) return false;
    AListEq_remove(this, i);
    return true;
}

bool AListEq_removeLast(AListEq * this, void * valuePointer)
{
    if (this == NULL) {
        fprintf(stderr, "AListEq_removeLast: null pointer to AListEq\n");
        return false;
    }

    int i = AListEq_lastIndexOf(this, valuePointer);
    if (i == -1) return false;
    AListEq_remove(this, i);
    return true;
}

void * AListEq_getArray(AListEq * this) {
    if (this == NULL) {
        fprintf(stderr, "AListEq_getArray: null pointer to AListEq\n");
        return NULL;
    }

    return AList_getArray(this->list);
}
