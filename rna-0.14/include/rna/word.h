/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/word.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_WORD_H
#define RNA_WORD_H

/******************************************************************\
 *                                                                *
 * Rna Word                                                       *
 *                                                                *
\******************************************************************/

union Word {

    Varg  varg;

    // @@@ TYPELIST @@@
    // Simple
    Bool  bb; // a boolean value
    Char  cc; // a boolean value
    Int   ii; // a signed integer value
    Float ff; // a floating point value
    Ptr   pp; // a generic pointer value
    Ref   rr; // a pointer to an Rna

    Str   ss; // a null terminated string
	
    // Composite
    class Record * record; // Record configuation
    class Array  * array;  // Array configuation
    class List * list; // List configuation
};

static inline bool operator == (Word b1, Word b2) { return b1.ii == b2.ii; }
static inline bool operator != (Word b1, Word b2) { return b1.ii != b2.ii; }


/******************************************************************\
 *                                                                *
 * Type Strings                                                   *
 *                                                                *
\******************************************************************/

enum Type {

    // Special
    STOP='\0',
    ANY='0',
    
    // @@@ TYPELIST @@@
    // Simple
    BOOL ='b',
    CHAR ='c',
    INT  ='i',
    FLOAT='f',
    PTR  ='p',
    REF  ='r',
    STR  ='s',
    
    // Composite
    RECORD='A',
    ARRAY ='B',
    LIST='C'
};


#endif /* RNA_WORD_H */
