/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Empty.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_EMPTY_H
#define RNA_CLASS_EMPTY_H

class Empty : public Record
{
public:

    // Constants
    static const Size SIZE = 0;

    // Instance Creation
    static Empty * Create (Memory * s);

    // TypeInfo
    static TypeDescriptor * TypeInfo ();

    // Access

    // Class Initialization
    static void Initialize ();

protected:

    // TypeInfo
    static TypeDescriptor * _TypeInfo;
};

#endif /* RNA_CLASS_EMPTY_H */
