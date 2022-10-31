/* well.cpp ---
 *
 * Filename: well.cpp
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

/* Copyright (c) 2016 IUB
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

#include "well.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <string.h>

well_t *init_well(int upper_left_x, int upper_left_y, int width, int height) {
  well_t *w;
  w = (well_t *)malloc(sizeof(well_t));
  w->upper_left_x = upper_left_x;
  w->upper_left_y = upper_left_y;
  w->width = width;
  w->height = height;
  w->draw_char = '#';
  w->color[0] = 0;
  w->color[1] = 0;
  w->color[2] = 0;
  return (w);
}

void draw_well(well_t *w) {
  int row_counter, column_counter;
  start_color();
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  attron(COLOR_PAIR(7));
  // Draw left side of well
  for (column_counter = w->upper_left_y;
       column_counter <= (w->upper_left_y + w->height); column_counter++) {
    mvprintw(column_counter, w->upper_left_x, "%c", w->draw_char);
  }

  // Draw right side of well
  for (column_counter = w->upper_left_y;
       column_counter <= (w->upper_left_y + w->height); column_counter++) {
    mvprintw(column_counter, (w->upper_left_x + w->width), "%c", w->draw_char);
  }

  // Draw Bottom of well
  for (row_counter = w->upper_left_x;
       row_counter <= (w->upper_left_x + w->width); row_counter++) {
    mvprintw(w->upper_left_y + w->height, row_counter, "%c", w->draw_char);
  }
  attroff(COLOR_PAIR(7));
}

well_t *changeWellSize(int upper_left_x, int upper_left_y, int width,
                       int height, well_t *w) {

  w->upper_left_x = upper_left_x;
  w->upper_left_y = upper_left_y;
  if (width < 10)
    w->width = 10;
  else
    w->width = width;

  w->height = height;

  return (w);
}

void undraw_well(well_t *w) {
  int row_counter, column_counter;
  // Undraw left side of well
  for (column_counter = w->upper_left_y;
       column_counter <= (w->upper_left_y + w->height); column_counter++) {
    mvprintw(column_counter, w->upper_left_x, " ", w->draw_char);
  }

  // Undraw right side of well
  for (column_counter = w->upper_left_y;
       column_counter <= (w->upper_left_y + w->height); column_counter++) {
    mvprintw(column_counter, (w->upper_left_x + w->width), " ", w->draw_char);
  }

  // Undraw Bottom of well
  for (row_counter = w->upper_left_x;
       row_counter <= (w->upper_left_x + w->width); row_counter++) {
    mvprintw(w->upper_left_y + w->height, row_counter, " ", w->draw_char);
  }
}

int prune_well(well_t *well, int random_color) {
  int column = well->upper_left_x + 1;
  int row_width = well->width - 1;
  int complete_rows = 0;

  for (int row = well->upper_left_y + well->height - 1; row >= 0; row--) {
    chtype scanned_row[row_width];
    mvinchnstr(row, column, scanned_row, row_width);
    bool row_complete = true;
    for (int i = 0; i < row_width; i++) {
      if (scanned_row[i] == ' ') {
        row_complete = false;
      }
    }

    if (row_complete) {
      complete_rows++;
      // delete row
      for (int col = 0; col < row_width; col++) {
        mvprintw(row, column + col, " ");
      }
    } else {
      // move the row down complete_rows times
      for (int col = 0; col < row_width; col++) {
        if ((scanned_row[col] & A_CHARTEXT) == '%') {
          start_color();
          attron(scanned_row[col] & A_COLOR);
          mvprintw(row + complete_rows, column + col, "%c", scanned_row[col]);
          attroff(scanned_row[col] & A_COLOR);
        } else {
          mvprintw(row + complete_rows, column + col, "%c", scanned_row[col]);
        }
      }
    }
  }

  return complete_rows;
}

void clear_well(well_t *well) {
  int column = well->upper_left_x + 1;
  int row_width = well->width - 1;

  for (int row = well->upper_left_y + well->height - 1; row >= 0; row--) {
    for (int col = 0; col < row_width; col++) {
      mvprintw(row, column + col, " ");
    }
  }
}
/* well.cpp ends here */
