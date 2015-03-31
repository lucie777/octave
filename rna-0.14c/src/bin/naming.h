/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/naming.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_NAMING_H
#define RNA_NAMING_H

char * make_macro_name (const char * name);
char * make_class_name (const char * name);
char * make_member_name (const char * name);

char * fix_prefix (const char * name, const char * old_prefix, const char * new_prefix);

char * make_makefile_env (const char * path);

#endif /* RNA_NAMING_H */
