//Muntean Vlad-Andrei 315CA
#pragma once
#include <stdio.h>

typedef struct {
char type[2];
int lin, col, max;
unsigned char **gray;
struct rgb {
	unsigned char r, g, b;
} **color;
struct coordonates {
	int x1, y1, x2, y2;
} selected;

} photo;
