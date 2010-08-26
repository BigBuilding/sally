/*
 * Sally - A Tool for Embedding Strings in Vector Spaces
 * Copyright (C) 2010 Konrad Rieck (konrad@mlsec.org)
 * --
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.  This program is distributed without any
 * warranty. See the GNU General Public License for more details. 
 */
 
/**
 * @defgroup util Utility functions
 * The module contains utility functions for Sally.
 * @author Konrad Rieck (konrad@mlsec.org)
 * @{
 */

#include "config.h"
#include "common.h" 
#include "util.h"

/* External variable */
extern int verbose;
/* Global variable */
static double time_start = -1;

/**
 * Print a formated info message with timestamp. 
 * @param v Verbosity level of message
 * @param m Format string
 */
void info_msg(int v, char *m, ...)
{
    va_list ap;
    char s[256] = { " " };

    if (time_start == -1)
        time_start = time_stamp();

    if (v > verbose)
        return;

    va_start(ap, m);
    vsnprintf(s, 256, m, ap);
    va_end(ap);
    
    fprintf(stderr, "[%7.1f] %s\n", time_stamp() - time_start, s);
    fflush(stderr);
}


/**
 * Print a formated error/warning message. See the macros error and 
 * warning in util.h
 * @param p Prefix string, e.g. "Error"
 * @param f Function name
 * @param m Format string
 */
void err_msg(char *p, const char *f, char *m, ...) 
{
    va_list ap;
    char s[256] = { " " };
    
    va_start(ap, m);
    vsnprintf(s, 256, m, ap);
    va_end(ap);

    fprintf(stderr, "%s: %s (", p, s);
    if (errno)
        fprintf(stderr, "%s, ", strerror(errno));
    fprintf(stderr, "%s)\n", f);
    errno = 0;
}


/**
 * Return a timestamp of the real time
 * @return time stamp
 */
double time_stamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}
 

/** @} */
