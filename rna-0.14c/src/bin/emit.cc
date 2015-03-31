/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/emit.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <rna/rna.h>

#include "emit.h"
#include "config.h"

FILE *
my_fopen (const char * filename, const char * mode)
{
    char * my_filename = expand_environment (filename);
    FILE * f = fopen (my_filename, mode);
    
    if (f == 0) {
	fprintf (stderr, "%s: Cannot open file \"%s\": %s\n",
		 program_invocation_short_name, my_filename,
		 strerror (errno));
	exit (EXIT_FAILURE);
    }

    free (my_filename);

    return f;
}

void
emit_indent (FILE * f, int indent)
{
    while (indent >= 8) {
	fputc ('\t', f);
	indent -= 8;
    }
    while (indent > 0) {
	fputc (' ', f);
	indent --;
    }
}

void
print_indent (FILE * f, int indent, const char * format, ...)
{
    char * buffer = 0;

    va_list ap;
    va_start (ap, format);
    vasprintf (& buffer, format, ap);
    va_end (ap);
    
    emit_indent (f, indent);
    bool indent_pending = false;
    for (char * p = buffer; *p; p++) {
	switch (*p) {
	case '\n': 
	    fputc (*p, f);
	    indent_pending = true;
	    break;
	default: 
	    if (indent_pending) {
		emit_indent (f, indent);
		indent_pending = false;
	    }
	    fputc (*p, f);
	}
    }

    free (buffer);
}

void
print_c_header (FILE * f)
{
    fprintf (f, "/* -*-c++-*- */\n");
    fprintf (f, "\n");
    fprintf (f, "/*\n");
    fprintf (f, " * %cAuthor%c\n", '$', '$');
    fprintf (f, " * %cDate%c\n", '$', '$');
    fprintf (f, " * %cSource%c\n", '$', '$');
    fprintf (f, " * %cRevision%c\n", '$', '$');
    fprintf (f, " */\n");
    fprintf (f, "\n");
    fprintf (f, "/* This code has been generated automatically. */\n");
    fprintf (f, "/* Do not edit by hand. */\n");
    fprintf (f, "\n");
    fprintf (f, "/* Copyright (c) 1998-2002 Gordon Cichon et al. */\n");
    fprintf (f, "/* Distributed under the GNU GPL. */\n");
    fprintf (f, "\n");
}

void
print_c_comment_block (FILE * f, const char * format, ...)
{
    char * message;

    va_list ap;
    va_start (ap, format);
    vasprintf (& message, format, ap);
    va_end (ap);

    fprintf (f, "\n");
    fprintf (f, "/******************************************************************\\\n");
    fprintf (f, " *                                                                *\n");
    fprintf (f, " * %-62s *\n", message);
    fprintf (f, " *                                                                *\n");
    fprintf (f, "\\******************************************************************/\n");
    fprintf (f, "\n");

    free (message);
}

void
print_shell_header (FILE * f, const char * mode)
{
    fprintf (f, "# -*-%s-*-\n", mode);
    fprintf (f, "\n");
    fprintf (f, "#\n");
    fprintf (f, "# %cAuthor%c\n", '$', '$');
    fprintf (f, "# %cDate%c\n", '$', '$');
    fprintf (f, "# %cSource%c\n", '$', '$');
    fprintf (f, "# %cRevision%c\n", '$', '$');
    fprintf (f, "#\n");
    fprintf (f, "\n");
    fprintf (f, "# This code has been generated automatically.\n");
    fprintf (f, "# Do not edit by hand.\n");
    fprintf (f, "\n");
    fprintf (f, "# Copyright (c) 1998-2002 Gordon Cichon et al.\n");
    fprintf (f, "# Distributed under the GNU GPL.\n");
    fprintf (f, "\n");
}

void
print_shell_comment_block (FILE * f, const char * message)
{
    fprintf (f, "\n");
    fprintf (f, "##################################################################\n");
    fprintf (f, "#                                                                #\n");
    fprintf (f, "# %-62s #\n", message);
    fprintf (f, "#                                                                #\n");
    fprintf (f, "##################################################################\n");
    fprintf (f, "\n");
}
