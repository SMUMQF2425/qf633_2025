#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point
{
    int x;
    int y;
};

struct Point2
{
    int x;
    int y;
    double z;
};

struct PointArray
{
    struct Point points[3];
};

void printPoint(struct Point p)
{
    printf("%d %d\n", p.x, p.y);
}

void printPoint2(struct Point2 p)
{
    printf("%d %d\n", p.x, p.y);
}

struct Point getPoint1()
{
    struct Point temp;
    temp.x = 50;
    temp.y = 100;
    return temp;
}

struct Point *getPoint2()
{
    struct Point *temp;
    struct Point initial = {50,10};
    temp = malloc(sizeof(struct Point));
    *temp = initial;
    return temp;
}

void printPointArray(struct PointArray pa)
{
    for (int i = 0; i < 3; i++)
    {
        printf("%d %d\n", pa.points[i].x, pa.points[i].y);
    }
}

void free_point(struct Point *p)
{
    printf("memory is free \n");
    free(p);
}