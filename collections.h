#include <stddef.h>
#include <stdbool.h>

#ifndef COLLECTIONS_H
#define COLLECTIONS_H

/* AList */

typedef struct AList AList;

AList * AList_new(size_t cellSize, int initialCapacity);

void * AList_get     (AList * this, int index);
int    AList_set     (AList * this, int index, void * valuePointer);
int    AList_size    (AList * this);
int    AList_add     (AList * this, void * valuePointer);
int    AList_insert  (AList * this, int index, void * valuePointer);
int    AList_remove  (AList * this, int index);
void   AList_destroy (AList * this);
void * AList_getArray(AList * this);



/* AListEq */

typedef struct AListEq AListEq;

AListEq * AListEq_new(
    bool (*eq)(void *, void*), 
    size_t cellSize, 
    int initialCapacity
);

void * AListEq_get         (AListEq * this, int index);
int    AListEq_set         (AListEq * this, int index, void * valuePointer);
int    AListEq_size        (AListEq * this);
int    AListEq_add         (AListEq * this, void * valuePointer);
int    AListEq_insert      (AListEq * this, int index, void * valuePointer);
int    AListEq_remove      (AListEq * this, int index);
void   AListEq_destroy     (AListEq * this);
bool   AListEq_contains    (AListEq * this, void * valuePointer);
int    AListEq_firstIndexOf(AListEq * this, void * valuePointer);
int    AListEq_lastIndexOf (AListEq * this, void * valuePointer);
bool   AListEq_removeAll   (AListEq * this, void * valuePointer);
bool   AListEq_removeFirst (AListEq * this, void * valuePointer);
bool   AListEq_removeLast  (AListEq * this, void * valuePointer);
void * AListEq_getArray    (AListEq * this);

#endif