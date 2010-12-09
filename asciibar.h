// This file is part of asciibar
// Copyright 2010 Chris Eberle <eberle1080@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __asciibar_h__
#define __asciibar_h__

#include <time.h>

// ASCII bar structures
struct ascii_bar_params
{
    // Display fields
    int len;            // The length of the bar in characters
    int show_txt;       // Whether or not to show percentage as text
    int show_eta;       // Show the estimated completion time
    char left_char;     // The character used for the left end-character
    char right_char;    // The character used for the right end-character
    char progress_char; // The character used to show progress
    char leading_char;  // The character used at the rightmost edge of progress
    char incom_char;    // The character used to show uncompleted progress

    // Progress fields
    float begin;        // The starting value for the progress bar
    float end;          // The end value for the progress bar
    float stepsize;     // The step size
    float pos;          // The current progress

    // Time fields
    int el;             // Extra length to remove upon next update
    time_t eta;         // Time until completion
    time_t epoch;       // The time that the progress bar was first shown
    time_t start_time;  // The beginning of statistical significance
    time_t last_time;   // Used for calculating ETA
    time_t next_time;   // The expected next update time
    time_t end_time;    // The time 100% was reached
    float left;         // The amount remaining
    float rate;         // The processing rate
    int broke;          // Is it "broke"

    // Internal use
    int called;
};

// ASCII bar
void init_ascii_bar(struct ascii_bar_params *params, float min, float max, float step);
void show_ascii_bar(struct ascii_bar_params *params);
void set_ascii_bar(struct ascii_bar_params *params, float val);
void step_ascii_bar(struct ascii_bar_params *params);
void hide_ascii_bar(struct ascii_bar_params *params);
void print_elapsed_time(struct ascii_bar_params *params, const char *metric_name);

#endif // __asciibar_h__
