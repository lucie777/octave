/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/config.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rna/rna.h>

#include "config.h"

char * 
expand_environment (const char * value)
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
	    char * var_name = strndup (var_start, var_len);
	    char * var_value = getenv (var_name);
	    if (var_value)
		fputs (var_value, f);
	    else { 
		fprintf (stderr, "Undefined Environment Variable $%s\n", var_name);
		exit (EXIT_FAILURE);
	    }
	    free (var_name);
	}
    }

    fclose (f);
    return result;
}

char *
read_attribute (RNA::Rna item, char * name, char * default_value = "")
{
    RNA::List * list (item.readRecord ()->getAttr ()->find (name));
    if (list)
	return list->getCar ().readStr ();
    else {
	item.readRecord ()->setAttr 
	    (RNA::List::Create (RNA::Memory::Main (), 
				name, RNA::STR, default_value, 
				item.readRecord ()->getAttr ()));
	return default_value;
    }
}

