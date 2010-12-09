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
#include <string.h>
#include <math.h>

#include "asciibar.h"

// Initialize the ASCII progress bar
void init_ascii_bar(struct ascii_bar_params *params, float min, float max, float step)
{
    if(min > max)
    {
        // I'm way too nice
        float temp = min;
        min = max;
        max = temp;
    }

    // Set some reasonable defaults
    params->len = 80;
    params->show_txt = 1;
    params->left_char     = '[';
    params->right_char    = ']';
    params->progress_char = '=';
    params->leading_char  = '>';
    params->incom_char    = '.';
    params->begin    = min;
    params->end      = max;
    params->stepsize = step;
    params->pos      = min;
    params->called   = 0;
    params->show_eta = 0;

    // ETA calculation parameters
    params->el = 0;
    params->eta = (time_t)-1;
    params->epoch = (time_t)0;
    params->start_time = (time_t)0;
    params->last_time  = (time_t)0;
    params->next_time  = (time_t)0;
    params->end_time   = (time_t)0;
    params->left       = max - min;
    params->rate       = 0.0;
    params->broke      = 0;
}

// Show the ASCII progress bar
void show_ascii_bar(struct ascii_bar_params *params)
{
    set_ascii_bar(params, params->pos);
}

// Just because
static float __max(float a, float b)
{
    if(a > b)
        return a;

    return b;
}

// Also just because
static time_t __tmin(float a, float b)
{
    if(a < b)
        return (time_t)a;

    return (time_t)b;
}

// Calculate the ETA, and format a string that displays said ETA
static void calc_time_remaining(struct ascii_bar_params *params, float val, time_t now, char *time_buf)
{
    float amt = val - params->pos;

    params->left -= amt;

    if(now < params->next_time && amt == 0)
        return;

    time_t sdiff = now - params->start_time;

    if(sdiff <= 0)
        params->eta = -1;
    else
    {
        params->rate = ((params->rate * (float)(params->last_time - params->start_time)) + amt) / (float)sdiff;
        params->last_time = now;
        params->eta = (time_t)(params->left / params->rate);

        if(params->start_time < params->last_time - params->eta)
            params->start_time = params->last_time - params->eta;
    }

    if(params->broke && (params->last_time - params->start_time) < 20)
        params->start_time = params->last_time - 20;

    if(params->last_time - params->start_time > 20)
        params->broke = 1;

    params->next_time = now + __tmin((amt / __max(params->rate, 0.0001)), 5.0);

    char tmp_buf[64];
    time_buf[0] = '\0';

    if(params->eta != (time_t)-1)
    {
        int hours   = params->eta / 3600;
        int minutes = (params->eta - (hours * 3600)) / 60;
        int seconds = params->eta - (hours * 3600) - (minutes * 60);

        sprintf(time_buf, "eta:");

        if(hours == 1)
        {
            strcat(time_buf, " 1 hr");
        }
        else if(hours > 1)
        {
            snprintf(tmp_buf, 64, " %d hrs", hours);
            strncat(time_buf, tmp_buf, 64);
        }

        if(minutes == 1)
            strcat(time_buf, " 1 min");
        else if(minutes == 0 && hours > 0)
            strcat(time_buf, " 0 mins");
        else if(minutes > 1)
        {
            snprintf(tmp_buf, 64, " %d mins", minutes);
            strncat(time_buf, tmp_buf, 64);
        }

        if(hours == 0)
        {
            if(seconds == 1)
                strcat(time_buf, " 1 sec");
            else
            {
                snprintf(tmp_buf, 64, " %d secs", seconds);
                strncat(time_buf, tmp_buf, 64);
            }
        }

        strcat(time_buf, " ");

        if(hours == 0 && minutes == 0 && seconds == 0)
            time_buf[0] = '\0';
    }
}

// Set the ASCII progress bar to a specific position
void set_ascii_bar(struct ascii_bar_params *params, float val)
{
    char   time_buf[128];
    time_t now = time(NULL);

    int i;

    if(params->called == 1)
    {
        for(i = 0; i < params->el; i++)
            printf("\b");

        for(i = 0; i < params->el; i++)
            printf(" ");

        for(i = 0; i < params->el; i++)
            printf("\b");

        for(i = 0; i < params->len; i++)
            printf("\b");
    }
    else
    {
        params->epoch = now;
        params->start_time = now;
        params->last_time  = now;
    }

    params->called = 1;

    float range = params->end - params->begin;
    float mypos = val - params->begin;

    float percentage;

    if(mypos < 0.0)
        percentage = 0.0f;
    else if(mypos >= range)
        percentage = 1.0f;
    else if(range == 0.0f)
        return;
    else
        percentage = mypos / range;

    if(params->show_eta)
        calc_time_remaining(params, val, now, time_buf);

    int width = params->len - 3;

    // Show the percentage text
    if(params->show_txt)
    {
        width -= 8;
        printf(" %5.1f%% ", percentage * 100.0);
    }

    int complete  = (int)(percentage * (float)width);
    int remaining = width - complete;

    // Show the progress bar
    printf("%c", params->left_char);

    for(i = 0; i < complete; i++)
    {
        if(i == complete - 1 && remaining > 0)
            printf("%c", params->leading_char);
        else
            printf("%c", params->progress_char);
    }

    for(i = 0; i < remaining; i++)
        printf("%c", params->incom_char);

    printf("%c ", params->right_char);

    if(params->show_eta)
    {
        params->el = strlen(time_buf);
        printf("%s", time_buf);
        params->end_time = now;
    }

    fflush(stdout);
    params->pos = val;
}

// Add step size to the current position and redraw
void step_ascii_bar(struct ascii_bar_params *params)
{
    set_ascii_bar(params, params->pos + params->stepsize);
}

// Hide the progress bar
void hide_ascii_bar(struct ascii_bar_params *params)
{
    int i;

    if(params->called == 1)
        for(i = 0; i < params->len; i++)
            printf("\b");

    for(i = 0; i < params->len; i++)
        printf(" ");

    for(i = 0; i < params->len; i++)
        printf("\b");
}

// Print the amount of time that the algorithm took
void print_elapsed_time(struct ascii_bar_params *params, const char *metric_name)
{
    time_t elapsed = params->end_time - params->epoch;
    float  sz   = params->pos - params->begin;

    int hours   = elapsed / 3600;
    int minutes = (elapsed - (hours * 3600)) / 60;
    int seconds = elapsed - (hours * 3600) - (minutes * 60);

    printf("Total time:");

    if(hours == 1)
        printf(" 1 hour");
    else if(hours > 1)
        printf(" %d hours", hours);

    if(minutes == 1)
        printf(" 1 minute");
    else if(minutes > 1)
        printf(" %d minutes", minutes);
    else if(hours > 0 && minutes == 0 && seconds > 0)
        printf(" 0 minutes");

    if(seconds == 1)
        printf(" 1 second");
    else if(seconds > 1)
        printf(" %d seconds", seconds);

    printf("\nAverage rate: %.3f seconds per %s\n", (float)elapsed / sz, metric_name);
}
