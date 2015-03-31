/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Position.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_POSITION_H
#define RNA_CLASS_POSITION_H

class Position : public Record
{
public:

    // Constants
    static const Index IDX_FILENAME = 0;
    static const Index IDX_ROW = 1;
    static const Index IDX_COLUMN = 2;
    static const Size SIZE = 3;

    // Instance Creation
    static Position * Create (Memory * s, Rna filename, Rna row, Rna column);
    
    // TypeInfo
    static TypeDescriptor * TypeInfo ();

    // Access - Read
    Str get_filename ();
    Int get_row ();
    Int get_column ();

    // Access - Write
    void set_filename (Rna);
    void set_row (Rna);
    void set_column (Rna);
    
    // Class Initialization
    static void Initialize ();

protected:
    
    // TypeInfo
    static TypeDescriptor * _TypeInfo;

    // Members
    Str _filename;
    Int _row;
    Int _column;
};

#endif /* RNA_CLASS_POSITION_H */
