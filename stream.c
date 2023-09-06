//Muntean Vlad-Andrei 315CA
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo.h"
#define LINE_LEN 70
#define NAME_LEN 31

void clean_file(FILE *file)
{
char s[LINE_LEN], blank;
int read = 1;
while (read) {
do {
	//catch first character and ignore spaces or newlines
	blank = fgetc(file);
} while (blank == ' ' || blank == '\n');
if (blank == '#') {
	//read the whole line to go to the next
	fgets(s, LINE_LEN, file);
} else {
	//come back to first element for normal read
	fseek(file, -1, SEEK_CUR);
	read = 0;
}
}
}

void allocate_matrix(photo *test)
{
int i;
//2 different matricies used for grayscale or color
if ((*test).type[1] == '2' || (*test).type[1] == '5') {
	(*test).gray = malloc((*test).lin * sizeof(unsigned char *));
	if (!(*test).gray) {
		//defensive programming method
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
	for (i = 0; i < (*test).lin; i++) {
		(*test).gray[i] = malloc((*test).col * sizeof(unsigned char));
		if (!(*test).gray[i]) {
			fprintf(stderr, "malloc failed\n");
			exit(-1);
		}
	}

} else if ((*test).type[1] == '3' || (*test).type[1] == '6') {
	(*test).color = malloc((*test).lin * sizeof(struct rgb *));
	if (!(*test).color) {
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
	for (i = 0; i < (*test).lin; i++) {
		(*test).color[i] = malloc((*test).col * sizeof(struct rgb));
	if (!(*test).color[i]) {
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
	}
}
}

void free_file(photo *nice)
{
if ((*nice).type[1] == '2' || (*nice).type[1] == '5') {
	//freeing the grayscale photo from heap
	for (int i = 0; i < (*nice).lin; i++)
		free((*nice).gray[i]);
	free((*nice).gray);
} else if ((*nice).type[1] == '3' || (*nice).type[1] == '6') {
	//freeing the colored photo from heap
	for (int i = 0; i < (*nice).lin; i++)
		free((*nice).color[i]);
	free((*nice).color);
}
}
void load_file(photo *nice, int *check)
{
photo test;
int i, j, pixel1, pixel2, pixel3;
char name[NAME_LEN];
scanf("%s", name);
FILE *file = fopen(name, "r");
if (!file) {
	printf("Failed to load %s\n", name);
	if (*check == 1) {
		//delete existing photo in case of error
		free_file(nice);
		*check = 0;
	}
	//force quit function in case of failure
	return;
}
//release previous image from heap
if (*check == 1)
	free_file(nice);
else
	*check = 1;
printf("Loaded %s\n", name);
//magic number read
fscanf(file, "%s", test.type);
//clean inbetween reads to avoid comments
clean_file(file);
fscanf(file, "%d %d", &test.col, &test.lin);

clean_file(file);
fscanf(file, "%d", &test.max);

//default selection for the whole photo
test.selected.x1 = 0;
test.selected.y1 = 0;
test.selected.x2 = test.col;
test.selected.y2 = test.lin;

clean_file(file);
allocate_matrix(&test);
for (i = 0; i < test.lin; i++) {
	for (j = 0; j < test.col; j++) {
		//specific reading method for every type of image
		switch (test.type[1]) {
		case '2':
		//P2 read
		fscanf(file, "%d", &pixel1);
		test.gray[i][j] = pixel1;
		break;

		case '3':
		//P3 read, group of 3 pixels for rgb values
		fscanf(file, "%d %d %d", &pixel1, &pixel2, &pixel3);
		//memory economy through storing integers into character arrays
		test.color[i][j].r = pixel1;
		test.color[i][j].g = pixel2;
		test.color[i][j].b = pixel3;
		break;

		case '5':
		//P5, binary read
		fread(&test.gray[i][j], sizeof(unsigned char), 1, file);
		break;

		case '6':
		//P6 read
		fread(&test.color[i][j].r, sizeof(unsigned char), 1, file);
		fread(&test.color[i][j].g, sizeof(unsigned char), 1, file);
		fread(&test.color[i][j].b, sizeof(unsigned char), 1, file);
		break;
		}
	}
}
*nice = test;
//file is unnecessary after read, so it's closed
fclose(file);
}

void write_text(photo nice, FILE *file)
{
// up to 70 pixels a line (format standard)
int max_len;
max_len = 0;
//function used to save in ascii format only
if (nice.type[1] == '2' || nice.type[1] == '5')
	fprintf(file, "P2\n");
else
	fprintf(file, "P3\n");
fprintf(file, "%d %d\n", nice.col, nice.lin);
fprintf(file, "%d\n", nice.max);

for (int i = 0; i < nice.lin; i++) {
	for (int j = 0; j < nice.col; j++)
		if (nice.type[1] == '2' || nice.type[1] == '5') {
			//all pixel values are written as integers
			fprintf(file, "%d ", nice.gray[i][j]);
			max_len++;
		} else {
			//rgb values written consecutively
			fprintf(file, "%d ", nice.color[i][j].r);
			fprintf(file, "%d ", nice.color[i][j].g);
			fprintf(file, "%d ", nice.color[i][j].b);
			max_len += 3;
		}
	if (max_len > LINE_LEN) {
		fprintf(file, "\n");
		max_len = 0;
	}
}
}

void write_binary(photo nice, FILE *file)
{
//function for saving plain files only
if (nice.type[1] == '2' || nice.type[1] == '5')
	fprintf(file, "P5\n");
else
	fprintf(file, "P6\n");
fprintf(file, "%d %d\n", nice.col, nice.lin);
fprintf(file, "%d\n", nice.max);

for (int i = 0; i < nice.lin; i++) {
	for (int j = 0; j < nice.col; j++)
		if (nice.type[1] == '2' || nice.type[1] == '5') {
			//pixel values are saved as char, no spaces or newlines
			fwrite(&nice.gray[i][j], sizeof(char), 1, file);

		} else {
			fwrite(&nice.color[i][j].r, sizeof(char), 1, file);
			fwrite(&nice.color[i][j].g, sizeof(char), 1, file);
			fwrite(&nice.color[i][j].b, sizeof(char), 1, file);
		}
}
}

void save_file(photo nice, int check)
{
char file_name[LINE_LEN], catch[LINE_LEN];
FILE *file;
int is_text;
//different functions for ascii and binary files
is_text = 0;
if (check == 0) {
	//catches redundant text in case of no file
	fgets(catch, LINE_LEN, stdin);
	printf("No image loaded\n");
	return;
}
// read whitespace
scanf("%c", (&file_name)[0]);
fgets(file_name, LINE_LEN, stdin);
//avoid spaces or newlines read by fgets()
if (file_name[strlen(file_name) - 2] == ' ')
	file_name[strlen(file_name) - 2] = '\0';
else
	file_name[strlen(file_name) - 1] = '\0';
//"ascii" compared with a beginning space to avoid ambiguous file names
if (strcmp(file_name + (strlen(file_name) - 6), " ascii") == 0) {
	//file will be saved as ascii
	is_text = 1;

	file_name[strlen(file_name) - 6] = '\0';
}
file = fopen(file_name, "w");
if (!file) {
	fprintf(stderr, "Could not save image..\n");
	exit(-1);
}
printf("Saved %s\n", file_name);
if (is_text == 1)
	write_text(nice, file);
else
	write_binary(nice, file);

fclose(file);
}
