//Muntean Vlad-Andrei 315CA
#pragma once
#include "photo.h"

void clean_file(FILE * file);
void allocate_matrix(photo *test);
void free_file(photo *nice);
void load_file(photo *nice, int *check);
void write_text(photo nice, FILE *file);
void write_binary(photo nice, FILE *file);
void save_file(photo nice, int check);
