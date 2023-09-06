//Muntean Vlad-Andrei 315CA
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo.h"

void init_rot_gray(photo *nice, int rot_lin, int rot_col)
{
//rotation for grayscale images only
unsigned char **rot_gray;
rot_gray = NULL;
int rx, ry, i, j;
//difference for rotation coordinates
rx = (*nice).selected.x1;
ry = (*nice).selected.y1;

rot_gray = malloc(rot_col * sizeof(unsigned char *));
if (!rot_gray) {
	printf("malloc failed\n");
	exit(-1);
}
for (i = 0; i < rot_col; i++) {
	rot_gray[i] = malloc(rot_lin * sizeof(unsigned char));

	if (!rot_gray[i]) {
		printf("malloc failed\n");
		exit(-1);
	}
	//selection copied as a 270 degree rotation
	for (j = 0; j < rot_lin; j++)
		rot_gray[i][j] = (*nice).gray[j + ry][rot_col - i - 1 + rx];
}
if (rot_lin != rot_col) {
	//in case of whole selections that are not square
	for (i = 0; i < (*nice).lin; i++)
		free((*nice).gray[i]);
	free((*nice).gray);

	//copy new coordinates and swap dimensions of new photo
	(*nice).selected.y2 = rot_col;
	(*nice).lin = rot_col;
	(*nice).selected.x2 = rot_lin;
	(*nice).col = rot_lin;

	(*nice).gray = rot_gray;

} else {
	for (i = ry; i < (*nice).selected.y2; i++)
		for (j = rx; j < (*nice).selected.x2; j++)
			(*nice).gray[i][j] = rot_gray[i - ry][j - rx];

	for (i = 0; i < rot_col; i++)
		free(rot_gray[i]);
	free(rot_gray);
}
}

void init_rot_color(photo *nice, int rot_lin, int rot_col)
{
//rotation for color images only
int i, j, rx, ry;
struct rgb **rot_color;
rot_color = NULL;
rx = (*nice).selected.x1;
ry = (*nice).selected.y1;
rot_color = malloc(rot_col * sizeof(struct rgb *));
if (!rot_color) {
	printf("malloc failed\n");
	exit(-1);
}
for (i = 0; i < rot_col; i++) {
	rot_color[i] = malloc(rot_lin * sizeof(struct rgb));
	if (!rot_color[i]) {
		printf("malloc failed\n");
		exit(-1);
	}
	for (j = 0; j < rot_lin; j++)
		rot_color[i][j] = (*nice).color[j + ry][rot_col - i - 1 + rx];
}
if (rot_lin != rot_col) {
	for (i = 0; i < (*nice).lin; i++)
		free((*nice).color[i]);
	free((*nice).color);

	(*nice).selected.y2 = rot_col;
	(*nice).lin = rot_col;
	(*nice).selected.x2 = rot_lin;
	(*nice).col = rot_lin;

	(*nice).color = rot_color;
} else {
	for (i = ry; i < (*nice).selected.y2; i++)
		for (j = rx; j < (*nice).selected.x2; j++)
			(*nice).color[i][j] = rot_color[i - ry][j - rx];

	for (i = 0; i < rot_col; i++)
		free(rot_color[i]);
	free(rot_color);
}
}

void rot_270(photo *nice, int rot_lin, int rot_col)
{
//270 degree rotation
if ((*nice).type[1] == '2' || (*nice).type[1] == '5')
	init_rot_gray(nice, rot_lin, rot_col);
else
	init_rot_color(nice, rot_lin, rot_col);
}

void rotate(photo *nice, int check)
{
int rot_lin, rot_col, angle;
scanf("%d", &angle);
if (check == 0) {
	printf("No image loaded\n");
	return;
}
//size of selection to be rotated
rot_lin = (*nice).selected.y2 - (*nice).selected.y1;

rot_col = (*nice).selected.x2 - (*nice).selected.x1;

//rotating non-square selections impossible, unless it's full size
if (rot_lin != rot_col)
	if (rot_lin != (*nice).lin || rot_col != (*nice).col) {
		printf("The selection must be square\n");
		return;
	}
//equivalent angles
if (angle == -90 || angle == 270) {
	rot_270(nice, rot_lin, rot_col);

	printf("Rotated %d\n", angle);
}

else if (angle == 180 || angle == -180) {
	//180 degrees = 270 rotated twice
	rot_270(nice, rot_lin, rot_col);
	rot_270(nice, rot_col, rot_lin);

	printf("Rotated %d\n", angle);
} else if (angle == 90 || angle == -270) {
	//90 degrees = 270 rotated three times
	rot_270(nice, rot_lin, rot_col);
	rot_270(nice, rot_col, rot_lin);
	rot_270(nice, rot_lin, rot_col);

	printf("Rotated %d\n", angle);
} else if (angle == 0 || angle == 360 || angle == -360) {
	//unaltered selection
	printf("Rotated %d\n", angle);
} else {
	printf("Unsupported rotation angle\n");
}
}
