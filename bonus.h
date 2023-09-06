//Muntean Vlad-Andrei 315CA
#pragma once
#include "photo.h"

void init_rot_gray(photo * nice, int rot_lin, int rot_col);
void init_rot_color(photo *nice, int rot_lin, int rot_col);
void rot_270(photo *nice, int rot_lin, int rot_col);
void rotate(photo *nice, int check);
