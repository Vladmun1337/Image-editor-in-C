//Muntean Vlad-Andrei 315CA
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo.h"
#include "stream.h"
#define LINE_LEN 70
#define NAME_LEN 31

int **init_kernel(int **kernel, int choice)
{
int i, j;
//initialise a 3*3 kernel for convolution
kernel = malloc(3 * sizeof(int *));

if (!kernel) {
	fprintf(stderr, "malloc failed\n");
	exit(-1);
}
for (i = 0; i < 3; i++) {
	kernel[i] = malloc(3 * sizeof(int));
	if (!kernel) {
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
}
//type of kernel we need
switch (choice) {
case 1:
//edge detection kernel
for (i = 0; i < 3; i++)
	for (j = 0; j <= i; j++) {
		kernel[i][j] = -1;
		kernel[j][i] = -1;
	}
kernel[1][1] = 8;
break;

case 2:
//sharpen kernel
for (i = 0; i < 3; i++)
	for (j = 0; j <= i; j++)
		if ((i + j) % 2 == 0) {
			kernel[i][j] = 0;
			kernel[j][i] = 0;
		} else {
			kernel[i][j] = -1;
			kernel[j][i] = -1;
		}
kernel[1][1] = 5;
break;

case 3:
//box blur kernel
for (i = 0; i < 3; i++)
	for (j = 0; j <= i; j++) {
		kernel[i][j] = 1;
		kernel[j][i] = 1;
	}
break;

case 4:
//gaussian blur kernel
for (i = 0; i < 3; i++)
	for (j = 0; j <= i; j++)
		if ((i + j) % 2 == 0) {
			kernel[i][j] = 1;
			kernel[j][i] = 1;
		} else {
			kernel[i][j] = 2;
			kernel[j][i] = 2;
		}
kernel[1][1] = 4;
break;
}
return kernel;
}

void check_colors(double *pixel)
{
//rounding doubles and clamping inappropriate values
*pixel = round(*pixel);
if (*pixel < 0)
	*pixel = 0;
else if (*pixel > 255)
	*pixel = 255;
}

void convolve(photo *nice, int **ker, struct rgb **new_color, int div)
{
int start_x, stop_x, start_y, stop_y, i, j;
//doubles for division approximation
double sum_r, sum_g, sum_b;

start_x = (*nice).selected.x1;
stop_x = (*nice).selected.x2;
start_y = (*nice).selected.y1;
stop_y = (*nice).selected.y2;

//start and stop values modified in case of marginal pixels
if (start_x == 0)
	start_x++;
if (stop_x == (*nice).col)
	stop_x--;
if (start_y == 0)
	start_y++;
if (stop_y == (*nice).lin)
	stop_y--;

for (i = start_y; i < stop_y; i++)
	for (j = start_x; j < stop_x; j++) {
		sum_r = 0;
		sum_g = 0;
		sum_b = 0;
		//multiplying and adding values from kernel and photo
		for (int k = i - 1; k <= i + 1; k++)
			for (int p = j - 1; p <= j + 1; p++) {
				sum_r += (*nice).color[k][p].r *
				ker[k - i + 1][p - j + 1];
				sum_g += (*nice).color[k][p].g *
				ker[k - i + 1][p - j + 1];
				sum_b += (*nice).color[k][p].b *
				ker[k - i + 1][p - j + 1];
			}
		if (div != 1) {
			//only for blurring effects
			sum_r /= div;
			sum_g /= div;
			sum_b /= div;
		}
		check_colors(&sum_r);
		check_colors(&sum_g);
		check_colors(&sum_b);

		new_color[i - start_y][j - start_x].r = sum_r;
		new_color[i - start_y][j - start_x].g = sum_g;
		new_color[i - start_y][j - start_x].b = sum_b;
}
//copy new values to the photo
for (i = start_y; i < stop_y; i++)
	for (j = start_x; j < stop_x; j++)
		(*nice).color[i][j] = new_color[i - start_y][j - start_x];
}

void apply_effect(photo *nice, int check)
{
char effect[NAME_LEN], jump, catch[LINE_LEN];
struct rgb **new_color;
int i, div;
int new_lin, new_col, **ker;
new_color = NULL;
ker = NULL;
div = 0;
if (check == 0) {
	fgets(catch, LINE_LEN, stdin);
	printf("No image loaded\n");
	return;
}
jump = fgetc(stdin);
if (jump == '\n') {
	printf("Invalid command\n");
	return;
}
if ((*nice).type[1] == '2' || (*nice).type[1] == '5') {
	fgets(catch, LINE_LEN, stdin);
	printf("Easy, Charlie Chaplin\n");
	return;
}
scanf("%s", effect);
//sizing the new array needed for convolution
new_lin = (*nice).selected.y2 - (*nice).selected.y1;
new_col = (*nice).selected.x2 - (*nice).selected.x1;

if ((*nice).selected.x1 == 0)
	new_col--;
if ((*nice).selected.x2 == (*nice).col)
	new_col--;
if ((*nice).selected.y1 == 0)
	new_lin--;
if ((*nice).selected.x2 == (*nice).lin)
	new_lin--;
//new matrix to store convolved pixels
new_color = malloc(new_lin *sizeof(struct rgb *));

if (!new_color) {
	fprintf(stderr, "malloc failed\n");
	exit(-1);
}
for (i = 0; i < new_lin; i++) {
	new_color[i] = malloc(new_col *sizeof(struct rgb));
	if (!new_color) {
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
}
if (strcmp(effect, "EDGE") == 0) {
	div = 1;
	ker = init_kernel(ker, 1);
	printf("APPLY %s done\n", effect);
} else if (strcmp(effect, "SHARPEN") == 0) {
	div = 1;
	ker = init_kernel(ker, 2);
	printf("APPLY %s done\n", effect);
} else if (strcmp(effect, "BLUR") == 0) {
	div = 9;
	ker = init_kernel(ker, 3);
	printf("APPLY %s done\n", effect);
} else if (strcmp(effect, "GAUSSIAN_BLUR") == 0) {
	div = 16;
	ker = init_kernel(ker, 4);
	printf("APPLY %s done\n", effect);
} else {
	printf("APPLY parameter invalid\n");
}
if (div) {
	convolve(nice, ker, new_color, div);
	for (i = 0; i < 3; i++)
		free(ker[i]);
	free(ker);
}

for (i = 0; i < new_lin; i++)
	free(new_color[i]);
free(new_color);
}

long long *calc_histo(photo nice, long long *histo_map, int bins)
{
int i, j, step;
//step value between each bin
step = (nice.max + 1) / bins;
for (i = 0; i < nice.lin; i++)
	for (j = 0; j < nice.col; j++)
		histo_map[(nice.gray[i][j] / step)]++;
return histo_map;
}

void histogram(photo nice, int check)
{
int max_value, bins, i, j, cmnd[2], word_count;
long long *histo_map, max_freq;
double star_length;
char catch[LINE_LEN], *word, jump;

word_count = 0;
max_freq = 0;
histo_map = NULL;
word = NULL;

if (check == 0) {
	fgets(catch, LINE_LEN, stdin);
	printf("No image loaded\n");
	return;
}
jump = fgetc(stdin);
if (jump == '\n') {
	printf("Invalid command\n");
	return;
}
if (nice.type[1] == '3' || nice.type[1] == '6') {
	fgets(catch, LINE_LEN, stdin);
	printf("Black and white image needed\n");
	return;
}
fgets(catch, LINE_LEN, stdin);
catch[strlen(catch) - 1] = '\0';
word = strtok(catch, " ");
do {
	cmnd[word_count++] = atoi(word);
	word = strtok(NULL, " ");
} while (word && word_count < 3);

if (word_count != 2) {
	//star length and bins are the only values accepted
	printf("Invalid command\n");
	return;
}

max_value = cmnd[0];
bins = cmnd[1];

//bitwise power of 2 check for number of bins
if (bins < 2 || bins > 256 || (bins & (bins - 1)) != 0) {
	printf("Invalid set of parameters\n");
	return;
}
histo_map = calloc(bins, sizeof(long long));
if (!histo_map) {
	fprintf(stderr, "Calloc failed\n");
	free_file(&nice);
	exit(-1);
}

histo_map = calc_histo(nice, histo_map, bins);

for (i = 0; i < bins; i++)
	if (histo_map[i] > max_freq)
		max_freq = histo_map[i];

for (i = 0; i < bins; i++) {
	star_length = (histo_map[i] * (1.0) / max_freq) *max_value;

	printf("%d	|	", (int)star_length);
	for (j = 0; j < (int)star_length; j++)
		printf("*");
	printf("\n");
}
free(histo_map);
}

void equalize(photo *nice, int check)
{
int i, j, area;
long long *histo_map, *histo_sum;
double new;
histo_map = NULL;
histo_sum = NULL;
if (check == 0) {
	printf("No image loaded\n");
	return;
}
if ((*nice).type[1] == '3' || (*nice).type[1] == '6') {
	printf("Black and white image needed\n");
	return;
}
histo_map = calloc((*nice).max + 1, sizeof(long long));
if (!histo_map) {
	fprintf(stderr, "Calloc failed\n");
	free_file(nice);
	exit(-1);
}
histo_sum = calloc((*nice).max + 1, sizeof(long long));
if (!histo_sum) {
	fprintf(stderr, "Calloc failed\n");
	free_file(nice);
	exit(-1);
}
//reusing function to calculate the freuency of every pixel
histo_map = calc_histo(*nice, histo_map, (*nice).max);
//fast array of sums calculated
histo_sum[0] = histo_map[0];
for (i = 1; i <= (*nice).max; i++)
	histo_sum[i] = histo_map[i] + histo_sum[i - 1];

area = (*nice).lin * (*nice).col;
for (i = 0; i < (*nice).lin; i++)
	for (j = 0; j < (*nice).col; j++) {
		//calculating new values using formula
		new = (*nice).max * histo_sum[(*nice).gray[i][j]] * (1.0)
		/ area;
		new = round(new);
	if (new < 0)
		new = 0;
	else if (new > 255)
		new = 255;
	(*nice).gray[i][j] = (unsigned char)new;
}
printf("Equalize done\n");
free(histo_sum);
free(histo_map);
}
