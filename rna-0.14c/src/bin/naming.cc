/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/naming.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rna/rna.h>

#include "naming.h"

char *
make_macro_name (const char * name)
{
    char * result = (char *) malloc (strlen (name) + 1);

    const char * src = name;
    char * dst = result;

    while (*src) {
	if (isalnum (*src))
	    *(dst++) = toupper (*(src++));
	else {
	    *(dst++) = '_';
	    do src++; while (*src && !isalnum (*src));
	}
    }

    *dst = '\0';
    return result;
}

char *
make_class_name (const char * name)
{
    char * result = (char *) malloc (strlen (name) + 1);

    const char * src = name;
    char * dst = result;

    bool upper = true;
    while (*src) {
	if (isalnum (*src)) {
	    if (upper)
		*(dst++) = toupper (*(src++));
	    else
		*(dst++) = *(src++);
	    upper = false;
	} else {
	    upper = true;
	    do src++; while (*src && !isalnum (*src));
	}
    }

    *dst = '\0';
    return result;
}

char *
make_member_name (const char * name)
{
    char * result = (char *) malloc (strlen (name) + 1);

    const char * src = name;
    char * dst = result;

    bool upper = false;
    while (*src) {
	if (isalnum (*src)) {
	    if (upper)
		*(dst++) = toupper (*(src++));
	    else
		*(dst++) = *(src++);
	    upper = false;
	} else {
	    upper = true;
	    do src++; while (*src && !isalnum (*src));
	}
    }

    *dst = '\0';
    return result;
}

char *
fix_prefix (const char * name, 
	    const char * old_prefix, 
	    const char * new_prefix)
{
    const char * word_name;

    int len1 = strlen (old_prefix);
    if (strncmp (name, old_prefix, len1) == 0)
	word_name = name + len1;
    else
	word_name = name;

    int len2 = strlen (word_name);
    int len3 = strlen (new_prefix);
    char * result;
    if (len3 > 0) {
	result = (char *) malloc (len2 + len3 + 2);
	
	strcpy (result, new_prefix);
	result [len3] = '-';
	strcpy (result + len3 + 1, word_name);
    } else 
	result = strdup (word_name);
    
    return result;
}

char * 
make_makefile_env (const char * value)
{
    if (value == 0)
	return 0;

    char * result = 0;
    size_t result_length = 0;
    FILE * f = open_memstream (&result, &result_length);

    for (int i = 0; value [i]; i++) {
	if (value [i] != '$') {
	    fputc (value [i], f);
	} else {
	    i++;
	    const char * var_start = value + i;
	    int var_len = 0;
	    if (value [i] == '{') {
		var_start++;
		i++;
		while (isalnum (value [i]) || value [i] == '_') {
		    i++;
		    var_len++;
		}
		RNA_ASSERT (value [i] == '}');
	    } else {
		while (isalnum (value [i]) || value [i] == '_') {
		    i++;
		    var_len++;
		}
		i--;
	    }
	    fprintf (f, "$(%.*s)", var_len, var_start);
	}
    }

    fclose (f);
    return result;
}
