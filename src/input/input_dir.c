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
 * @defgroup input 
 * @author Konrad Rieck (konrad@mlsec.org)
 * @{
 */

#include "config.h"
#include "common.h"
#include "util.h"
#include "input.h"
#include "murmur.h"

/* Local functions */
static char *load_file(char *path, char *name, int *size);

/* Local variables */
static DIR *dir = NULL;
static char *path = NULL;

/**
 * Opens a directory for reading files. 
 * @param p Directory name
 * @return number of regular files
 */
int input_dir_open(char *p) 
{
    assert(p);    
    struct dirent *dp;
    path = p;

    /* Open directory */
    dir = opendir(path);
    if (!dir) {
        error("Could not open directory '%s'", path);
        return 0;
    }

    /* Count files */
    int num_files = 0;
    while (dir && (dp = readdir(dir)) != NULL) {
        if (dp->d_type == DT_REG)
            num_files++;
    }
    rewinddir(dir);
    return num_files;
}

/**
 * Reads a block of files into memory.
 * @param strs Array for file data
 * @param sizes Array of file sizes
 * @param names Array of file names
 * @param len Length of block
 * @return number of read files
 */
int input_dir_read(char **strs, int *sizes, char **names, int len)
{
    assert(strs && sizes && names);
    int i, j = 0;
 
    /* Determine maximum path length and allocate buffer */
    int maxlen = fpathconf(dirfd(dir), _PC_NAME_MAX);
    
    /* Load block of files */
#pragma omp parallel for
    for (i = 0; i < len; i++) {
        struct dirent *buf, *dp;
        buf = malloc(offsetof(struct dirent, d_name) + maxlen + 1);
    
        /* Read directory entry to local buffer */
        readdir_r(dir, (struct dirent *) buf, &dp);
        if (!dp) 
            goto skip;

        /* Skip all entries except for regular files */
        if (dp->d_type != DT_REG) 
            goto skip;

#pragma omp critical
        {
            names[j] = strdup(dp->d_name);
            strs[j] = load_file(path, dp->d_name, sizes + j);
            j++;
        }
skip:
        free(buf);	
    }
    return j;
}

/**
 * Closes an open directory.
 */
void input_dir_close()
{
    closedir(dir);
}

/**
 * Loads a file into a byte array. The array is allocated 
 * and need to be free'd later by the caller.
 * @param path Path to file
 * @param name File name or NULL
 * @param size Pointer to file size
 * @return file data
 */
static char *load_file(char *path, char *name, int *size)
{
    assert(path);
    long read;
    char *x = NULL, file[512];
    struct stat st;

#pragma omp critical (snprintf)
    {
        /* snprintf is not necessary thread-safe. good to know. */
        if (name)
            snprintf(file, 512, "%s/%s", path, name);
        else
            snprintf(file, 512, "%s", path);
    }

    /* Open file */
    FILE *fptr = fopen(file, "r");
    if (!fptr) {
        error("Could not open file '%s'", file);
        return NULL;
    }

    /* Allocate memory */
    stat(file, &st);
    *size = st.st_size;
    if (!(x = malloc((*size + 1) * sizeof(char)))) {
        error("Could not allocate memory for file data");
        return NULL;
    }

    /* Read data */
    read = fread(x, sizeof(char), *size, fptr);
    fclose(fptr);
    if (*size != read)
        warning("Could not read all data from file '%s'", file);

    return x;
}

/** 
 * Converts a file name to a label. The label is computed from the 
 * file's suffix; either directly if the suffix is a number or 
 * indirectly by hashing.
 * @param desc Description (file name) 
 * @return label value.
 */
float input_dir_desc2label(char *desc)
{
    char *endptr;
    char *name = desc + strlen(desc) - 1;

    /* Determine dot in file name */
    while (name != desc && *name != '.')
        name--; 

    /* Place pointer before '.' */
    if (name != desc)
        name++;

    /* Test direct conversion */
    float f = strtof(name, &endptr);
    
    /* Compute hash value */
    if (!endptr || strlen(endptr) > 0) 
        f = MurmurHash64B(name, strlen(name), 0xc0d3bab3) % 0xffff;
    
    return f;
} 

/** @} */
