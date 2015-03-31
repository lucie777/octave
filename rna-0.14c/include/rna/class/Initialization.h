/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Initialization.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_INITIALIZATION_H
#define RNA_CLASS_INITIALIZATION_H

class Initialization
{
public:
    static void Initialize ();

private:
    static Bool Initialized;
};

#endif /* RNA_CLASS_INITIALIZATION_H */

