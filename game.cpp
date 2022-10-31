/* game.cpp ---
 *
 * Filename: game.cpp
 * Description:
 * Author: Bryce
 * Maintainer: Adeel Bhutta
 * Created: Tue Sep  6 11:08:59 2016
 * Last-Updated: 01-10-2021
 *           By: Adeel Bhutta
 *     Update #: 20
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
#include "game.hpp"
#include "key.hpp"
#include "score.hpp"
#include "tetris.hpp"
#include "tetromino.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <string>
#include <unistd.h>
using namespace std;

void init_game(void) { int x, y; }

int game(void) {
  static int state = INIT;
  tetromino_t *next = NULL;
  tetromino_t *current = NULL;
  tetromino_t *next_cpy = NULL;
  well_t *w;
  int x, y;
  int c;
  int arrow;
  int lives_remaining = 3;
  int score = 0;
  int lines_cleared;
  int time_elapsed = 0;
  int seconds_counter = 0;
  int random_color = 1;
  struct timespec tim = {
      0, 1000000}; // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;
  int move_counter = 0;
  int move_timeout = BASE_FALL_RATE;
  int level = 1;

  char name[20];
  cout << "Please input name: " << endl;
  cin >> name;
  cout << "Select level: 1 or 2" << endl;
  cin >> level;

  while (1) {
    switch (state) {
    case INIT: // Initialize the game, only run one time
      initscr();
      nodelay(stdscr, TRUE);  // Do not wait for characters using getch.
      noecho();               // Do not echo input characters
      getmaxyx(stdscr, y, x); // Get the screen dimensions
      w = init_well(((x / 2) - (WELL_WIDTH / 2)), 1, WELL_WIDTH, WELL_HEIGHT);
      draw_well(w);
      srand(time(NULL)); // Seed the random number generator with the time. Used
                         // in create tet.
      display_scores(w);
      state = ADD_PIECE;
      break;
    case ADD_PIECE: // Add a new piece to the game
      lines_cleared = prune_well(w, random_color);
      score = compute_score(score, lines_cleared);

      if (next) {
        current = next;
      } else {
        current = create_tetromino((w->upper_left_x + (w->width / 2)),
                                   w->upper_left_y, level);
      }

      next =
          create_tetromino((w->upper_left_x + (w->width / 2)), w->upper_left_y, level);
      mvprintw(1, w->upper_left_x + w->width + 3, "NEXT PIECE: %s             ",
               next->type_str);

      random_color = rand() % 4 + 1;

      display_tetromino(current, random_color);
      state = MOVE_PIECE;

      if (!strcmp(current->type_str, "block") ||
          !strcmp(current->type_str, "big_square") ||
          !strcmp(current->type_str, "thick_bar")) {
        move_timeout = SLOWER_FALL_RATE;
      } else if (!strcmp(current->type_str, "zigzag_l") ||
                 !strcmp(current->type_str, "zigzag_r")) {
        move_timeout = FASTER_FALL_RATE;
      } else {
        move_timeout = BASE_FALL_RATE;
      }

      if (level == 2) {
        move_timeout -= 200;
      }

      break;
    case MOVE_PIECE: // Move the current piece
      undisplay_tetromino(current);

      if ((arrow = read_escape(&c)) != NOCHAR) {
        switch (arrow) {
        case UP:
          mvprintw(10, 10, "UP            ");
          rotate_cw(current);
          break;
        case DOWN:
          mvprintw(10, 10, "DOWN          ");
          rotate_ccw(current);
          break;
        case LEFT:
          mvprintw(10, 10, "LEFT          ");
          move_tet(current, current->upper_left_x - 1, current->upper_left_y);
          break;
        case RIGHT:
          mvprintw(10, 10, "RIGHT         ");
          move_tet(current, current->upper_left_x + 1, current->upper_left_y);
          break;
        case REGCHAR:
          mvprintw(10, 10, "REGCHAR 0x%02x", c);
          if (c == ' ') {
            mvprintw(10, 10, "SPACE         ");
            move_timeout = DROP_RATE;
            break;
          }

          if (c == 'q' || c == 'x') {
            state = EXIT;
            break;
          }

          if (c == 'p') {
            state = PAUSE;
            mvprintw(10, 10, "GAME PAUSED         ");
            break;
          }
        }
      }

      if (move_counter++ >= move_timeout) {
        int result =
            move_tet(current, current->upper_left_x, current->upper_left_y + 1);
        if (result == MOVE_FAILED) {
          if (current->upper_left_y == w->upper_left_y) {
            state = GAME_OVER;
          } else {
            state = ADD_PIECE;
          }
        }

        move_counter = 0;
      }

      display_tetromino(current, random_color);
      break;
    case PAUSE:
      if ((arrow = read_escape(&c)) != NOCHAR) {
        if (arrow == REGCHAR) {
          mvprintw(10, 10, "GAME UNPAUSED", c);
          if (c == 'p') {
            state = MOVE_PIECE;
          }

          if (c == 'q' || c == 'x') {
            state = EXIT;
          }
        }
      }
      break;
    case GAME_OVER:
      if (lives_remaining <= 1) {
        lives_remaining = 0;
        mvprintw(10, 10, "GAME OVER           ");
        if (move_counter++ >= 5000) {
          state = EXIT;
          update_scores(score, name);
        }
      } else {
        lives_remaining--;
        clear_well(w);
        state = ADD_PIECE;
      }
      break;
    case EXIT:
      endwin();
      return (0);
      break;
    }

    if (state != PAUSE && state != GAME_OVER) {
      if (time_elapsed++ - 1000 > 0) {
        time_elapsed = 0;
        seconds_counter++;
      }
    }
    mvprintw(5, 10, " LIVES: %d", lives_remaining);
    mvprintw(8, 10, " SCORE: %d", score);
    mvprintw(7, 10, " TIME PASSED: %d", seconds_counter);
    mvprintw(2, 7, name);
    refresh();
    nanosleep(&tim, &tim_ret);
  }
}

/* game.cpp ends here */
