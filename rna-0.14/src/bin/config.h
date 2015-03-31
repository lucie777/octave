/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/config.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CONFIG_H
#define RNA_CONFIG_H

char * expand_environment (const char * value);

char * read_attribute (RNA::Rna item, char * name, char * default_value = "");

#endif /* RNA_CONFIG_H */
