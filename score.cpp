#include "score.hpp"
#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int compute_score(int previous_score, int lines_cleared) {
  return previous_score + lines_cleared * lines_cleared;
}

void display_scores(well_t *well) {
  ifstream highscores;
  highscores.open("highscores.txt");

  if (highscores.is_open()) {
    string name_line;
    int line_count = 0;
    mvprintw(3, well->upper_left_x + well->width + 3, "HIGH SCORES:     ");

    while (getline(highscores, name_line)) {
      string score;
      if (getline(highscores, score)) {
        stringstream ss;
        ss << name_line << ": " << score;
        mvprintw(4 + line_count, well->upper_left_x + well->width + 3,
                 ss.str().c_str());
        line_count++;
      }
    }
  }
}

void update_scores(int new_score, char *name) {
  ifstream highscores;
  highscores.open("highscores.txt");

  if (highscores.is_open()) {
    string name_line;
    string score;
    vector<int> scores;
    vector<string> names;
    while (getline(highscores, name_line)) {
      if (getline(highscores, score)) {
        scores.push_back(strtol(score.c_str(), NULL, 10));
        names.push_back(name_line);
      }
    }

    highscores.close();

    stringstream result;

    int score_size = scores.size();
    for (int i = 0; i < score_size; i++) {
      if (scores.at(i) >= new_score) {
        result << names.at(i) << endl << scores.at(i) << endl;
        if (i == scores.size() - 1) {
          result << name << endl << new_score << endl;
        }
      } else {
        result << name << endl << new_score << endl;
        int new_size = score_size + 1;
        
        if (new_size > 5) {
          for (int j = i; j < 4; j++) {
            result << names.at(j) << endl << scores.at(j) << endl;
          }
        } else {
          for (int j = i; j < scores.size(); j++) {
            result << names.at(j) << endl << scores.at(j) << endl;
          }
        }

        break;
      }
    }

    if (score_size == 0) {
      result << name << endl << new_score << endl;
    }

    ofstream updated_highscores;
    updated_highscores.open("highscores.txt");

    updated_highscores << result.str();

    updated_highscores.close();
  } else {
    ofstream updated_highscores;
    updated_highscores.open("highscores.txt");

    updated_highscores << name << endl << new_score << endl;
  }
}