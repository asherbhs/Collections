#include "collections.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Point {
    int x, y;
} Point;

void printPoint(Point * p) {
    printf("(%d, %d)\n", p->x, p->y);
}

Point * pointRefAtAddr(void * it) 
{
    return *((Point **) it);
}

bool equalPoints(void * x, void * y)
{
    Point * pointx = pointRefAtAddr(x);
    Point * pointy = pointRefAtAddr(y);

    return pointx->x == pointy->x && pointx->y == pointy->y;
}

int main(int argc, char ** argv)
{
    if (argc != 3) return 1;

    if (strcmp(argv[2], "packed") == 0) {
        AList * points = AList_new(sizeof(Point), 1);

        for (int i = 0; i < atoi(argv[1]); i++) {
            Point * newPoint = malloc(sizeof(Point));
            newPoint->x = i;
            newPoint->y = i;
            AList_add(points, newPoint);
        }

        for (int i = 0; i < AList_size(points); i++) {
            ((Point *) AList_get(points, i))->y *= 2;
        }

        /*
        for (int i = 0; i < AList_size(points); i++) {
            printPoint(AList_get(points, i));
        }
        */
    } else if (strcmp(argv[2], "referenced") == 0) {
        AList * refs = AList_new(sizeof(Point *), 1);

        for (int i = 0; i < atoi(argv[1]); i++) {
            Point * newPoint = malloc(sizeof(Point));
            newPoint->x = i;
            newPoint->y = i;
            AList_add(refs, &newPoint);
        }

        for (int i = 0; i < AList_size(refs); i++) {
            (*((Point **) AList_get(refs, i)))->y *= 2;
        }

        /*
        for (int i = 0; i < AList_size(refs); i++) {
            printPoint(*(Point **) AList_get(refs, i));
        }
        */
    }

    return 0;
}