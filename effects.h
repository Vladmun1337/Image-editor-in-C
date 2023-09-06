//Muntean Vlad-Andrei 315CA
#pragma once
#include "photo.h"

int **init_kernel(int **kernel, int choice);
void check_colors(double *pixel);
void convolve(photo *nice, int **ker, struct rgb **new_color, int div);
void apply_effect(photo *nice, int check);
long long *calc_histo(photo nice, long long *histo_map, int bins);
void histogram(photo nice, int check);
void equalize(photo *nice, int check);
