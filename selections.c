//Muntean Vlad-Andrei 315CA
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo.h"
#include "stream.h"
#define LINE_LEN 70

int compare_coordinates(photo nice, int *x1, int *y1, int *x2, int *y2)
{
int aux;
if (*x1 == *x2 || *y1 == *y2) {
	//equal coordinates on same axis cannot exist
	printf("Invalid set of coordinates\n");
	return 0;
}
if (*x1 > *x2) {
	aux = *x1;
	*x1 = *x2;
	*x2 = aux;
}

if (*y1 > *y2) {
	aux = *y1;
	*y1 = *y2;
	*y2 = aux;
}
if (*x1 < 0 || *y1 < 0 || *x2 > nice.col || *y2 > nice.lin) {
	//out-of-bound selections prohibited
	printf("Invalid set of coordinates\n");
	return 0;
}
return 1;
}

void selection(photo *nice, int check)
{
char command[LINE_LEN], catch[LINE_LEN], *word = NULL, space;
int word_count = 0, coord[4];
if (check == 0) {
	fgets(catch, LINE_LEN, stdin);
	printf("No image loaded\n");
	return;
}
// catching the whitespace
scanf("%c", &space);
fgets(command, LINE_LEN, stdin);
//erase newline
command[strlen(command) - 1] = '\0';
word = strtok(command, " ");

if (strcmp(word, "ALL") == 0) {
	//reset default coordinates
	(*nice).selected.x1 = 0;
	(*nice).selected.y1 = 0;
	(*nice).selected.x2 = (*nice).col;
	(*nice).selected.y2 = (*nice).lin;

	printf("Selected ALL\n");
} else {
	do {
		//extract coordinates from string, only as numbers
		if (word[0] == '-' || (word[0] <= '9' && word[0] >= '0')) {
			coord[word_count++] = atoi(word);
		} else {
			//in case of inappropriate coordinates
			printf("Invalid command\n");
			return;
		}
		word = strtok(NULL, " ");
	} while (word && word_count < 5);

	if (word_count != 4) {
		//wrong number of coordinates read
		printf("Invalid command\n");
		return;
	}
	//sort coordinates to ease work
	if (compare_coordinates(*nice, &coord[0], &coord[1], &coord[2],
	&coord[3])) {
		printf("Selected %d %d ", coord[0], coord[1]);
		printf("%d %d\n", coord[2], coord[3]);

		(*nice).selected.x1 = coord[0];
		(*nice).selected.y1 = coord[1];
		(*nice).selected.x2 = coord[2];
		(*nice).selected.y2 = coord[3];
	}
}
}

void crop_image(photo *nice, int check)
{
int i, j, new_y, new_x;
photo test;
if (check == 0) {
	printf("No image loaded\n");
	return;
}
strcpy(test.type, (*nice).type);
test.max = (*nice).max;

//new dimensions for cropped image
test.lin = (*nice).selected.y2 - (*nice).selected.y1;
test.col = (*nice).selected.x2 - (*nice).selected.x1;
//distance between new and old coordinates
new_y = (*nice).selected.y1;
new_x = (*nice).selected.x1;
allocate_matrix(&test);

if (test.type[1] == '2' || test.type[1] == '5') {
	for (i = 0; i < test.lin; i++)
		for (j = 0; j < test.col; j++)
			test.gray[i][j] = (*nice).gray[i + new_y][j + new_x];

	for (i = 0; i < (*nice).lin; i++)
		free((*nice).gray[i]);
	free((*nice).gray);
} else if (test.type[1] == '3' || test.type[1] == '6') {
	for (i = 0; i < test.lin; i++)
		for (j = 0; j < test.col; j++) {
			test.color[i][j].r = (*nice).color[i + new_y][j +
			new_x].r;
			test.color[i][j].g = (*nice).color[i + new_y][j +
			new_x].g;
			test.color[i][j].b = (*nice).color[i + new_y][j +
			new_x].b;
		}
	for (i = 0; i < (*nice).lin; i++)
		free((*nice).color[i]);
	free((*nice).color);
}
//set new default and replace old matrix
test.selected.x1 = 0;
test.selected.y1 = 0;
test.selected.x2 = test.col;
test.selected.y2 = test.lin;

(*nice) = test;
printf("Image cropped\n");
}
