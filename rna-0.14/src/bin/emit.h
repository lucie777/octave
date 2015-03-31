/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/emit.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_EMIT_H
#define RNA_EMIT_H

#include <stdio.h>

FILE * my_fopen (const char * filename, const char * mode);
void print_indent (FILE * f, int indent, const char * format, ...);

void print_c_header (FILE * f);
void print_c_comment_block (FILE * f, const char * format, ...);

void print_shell_header (FILE * f, const char * mode);
void print_shell_comment_block (FILE * f, const char * message);

#endif /* RNA_EMIT_H */
