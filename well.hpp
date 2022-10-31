/* well.hpp --- 
 * 
 * Filename: well.hpp
 * Description: 
 * Author: Bryce
 * Maintainer: Adeel Bhutta
 * Created: Tue Sep  6 11:08:59 2016
 * Last-Updated: 01-10-2021
 *           By: Adeel Bhutta
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2016 IUB.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

typedef struct well {
  int upper_left_x;
  int upper_left_y;
  int width;
  int height;
  char draw_char;
  char color[3];
} well_t;

well_t *init_well(int, int, int, int);
void draw_well(well_t *);

well_t *changeWellSize(int upper_left_x, int upper_left_y, int width, int height, well_t *);

void undraw_well(well_t *);

int prune_well(well_t *, int);

void clear_well(well_t *);

/* well.hpp ends here */
