/* -*-c++-*- */

/* Copyright (c) 1998-2003 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:47 $
 * $Source: /sys/cvs/rna/include/rna/word.h,v $
 * $Revision: 1.2 $
 */


#ifndef RNA_WORD_H
#define RNA_WORD_H

/******************************************************************\
 *                                                                *
 * Rna Word                                                       *
 *                                                                *
\******************************************************************/

union Word {

    Bool bb;
    Char cc;
    Int ii;
    Float ff;
    Ptr pp;
    Ref rr;
    Str ss;
    Record * record;
    Array * array;
    List * list;
    Varg varg;

};

static inline bool operator == (Word b1, Word b2) { return b1.ii == b2.ii; }
static inline bool operator != (Word b1, Word b2) { return b1.ii != b2.ii; }


/******************************************************************\
 *                                                                *
 * Type Strings                                                   *
 *                                                                *
\******************************************************************/

enum Type {

    STOP='\0',
    ANY='0',
    BOOL='b',
    CHAR='c',
    INT='i',
    FLOAT='f',
    PTR='p',
    REF='r',
    STR='s',
    RECORD='A',
    ARRAY='B',
    LIST='C'

};


#endif /* RNA_WORD_H */
