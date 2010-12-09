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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "asciibar.h"

// A normal progress bar, no bells or whistles
void test_progress()
{
    int i;
    struct ascii_bar_params params;

    printf("Normal:   ");
    init_ascii_bar(&params, 3.0f, 75.0f, 1.0f);
    params.len = 50;
    for(i = 0; i <= 100; i++)
    {
        set_ascii_bar(&params, (float)i);
        usleep(10000);
    }

    printf("\n");
}

// Don't show the percentage text
void test_notext()
{
    int i;
    struct ascii_bar_params params;

    printf("No text:  ");
    init_ascii_bar(&params, 3.0f, 75.0f, 1.0f);
    params.len = 50;
    params.show_txt = 0;
    for(i = 0; i <= 100; i++)
    {
        set_ascii_bar(&params, (float)i);
        usleep(10000);
    }

    printf("\n");
}

// Hide the progress bar at the end
void test_hide_progress()
{
    int i;
    struct ascii_bar_params params;

    printf("Hide:     ");
    init_ascii_bar(&params, 3.0f, 75.0f, 1.0f);
    params.len = 50;

    for(i = 0; i <= 100; i++)
    {
        set_ascii_bar(&params, (float)i);
        usleep(10000);
    }

    hide_ascii_bar(&params);
    printf("[OK]\n");
}

// Enable time estimation
void test_time_est()
{
    int i;
    struct ascii_bar_params params;

    printf("Time est: ");
    init_ascii_bar(&params, 0.0f, 100.0f, 1.0f);
    params.len = 50;
    params.show_eta = 1;
    for(i = 0; i <= 100; i++)
    {
        set_ascii_bar(&params, (float)i);
        usleep(100000);
    }

    printf("\n");
}

// Use custom characters for a unique looking progress bar
void test_progress_text()
{
    int i;
    struct ascii_bar_params params;

    printf("Custom:   ");
    init_ascii_bar(&params, 3.0f, 75.0f, 1.0f);
    params.len = 50;
    params.left_char = '<';
    params.right_char = '>';
    params.progress_char = '+';
    params.leading_char = '|';
    params.incom_char = '_';

    for(i = 0; i <= 100; i++)
    {
        set_ascii_bar(&params, (float)i);
        usleep(10000);
    }

    printf("\n");
}

// Oh I bet you know
int main()
{
    test_progress();
    test_notext();
    test_hide_progress();
    test_time_est();
    test_progress_text();

    return 0;
}
