//Muntean Vlad-Andrei 315CA
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo.h"
#include "bonus.h"
#include "stream.h"
#include "selections.h"
#include "effects.h"
#define LINE_LEN 70

char choose_command(char *command)
{
scanf("%s", command);
if (strcmp(command, "LOAD") == 0)
	return 'L';
if (strcmp(command, "SELECT") == 0)
	return 'S';
if (strcmp(command, "CROP") == 0)
	return 'C';
if (strcmp(command, "APPLY") == 0)
	return 'A';
if (strcmp(command, "SAVE") == 0)
	return 'V';
if (strcmp(command, "HISTOGRAM") == 0)
	return 'H';
if (strcmp(command, "EQUALIZE") == 0)
	return 'E';
if (strcmp(command, "ROTATE") == 0)
	return 'R';
if (strcmp(command, "EXIT") == 0)
	return 'X';
return 0;
}

int main(void)
{
photo nice;
char catch[LINE_LEN], command[LINE_LEN];
int check;
check = 0;
while (1) {
	switch (choose_command(command)) {
	case 'L':
	load_file(&nice, &check);
	break;

	case 'S':
	selection(&nice, check);
	break;

	case 'C':
	crop_image(&nice, check);
	break;

	case 'A':
	apply_effect(&nice, check);
	break;

	case 'V':
	save_file(nice, check);
	break;

	case 'H':
	histogram(nice, check);
	break;

	case 'E':
	equalize(&nice, check);
	break;

	case 'R':
	rotate(&nice, check);
	break;

	case 'X':
	if (check == 0)
		printf("No image loaded\n");
	else
		free_file(&nice);
	return 0;

	default:
	fgets(catch, LINE_LEN, stdin);
	printf("Invalid command\n");
	}
}
}
