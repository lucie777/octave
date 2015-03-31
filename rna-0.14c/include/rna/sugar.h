/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/sugar.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_SUGAR_H
#define RNA_SUGAR_H

# ifdef RNA_SYNTACTIC_SUGAR

/******************************************************************\
 *                                                                *
 * Class Declarations                                             *
 *                                                                *
\******************************************************************/

class RecordAccess;
class ArrayAccess;
class ListAccess;
 

/******************************************************************\
 *                                                                *
 * Record Access Proxy                                            *
 *                                                                *
\******************************************************************/

class RecordAccess
{
public:

    // Copying
    RNA_INLINE RecordAccess (const RecordAccess &);

    // @@@ TYPELIST @@@
    RNA_INLINE operator Rna ();
    RNA_INLINE operator Float ();
    RNA_INLINE operator Int ();
    RNA_INLINE operator Str ();
    RNA_INLINE operator Record * ();
    RNA_INLINE operator Array * ();
    RNA_INLINE operator List * ();

    // @@@ TYPELIST @@@
    RNA_INLINE RecordAccess & operator = (Rna);
    RNA_INLINE RecordAccess & operator = (Float);
    RNA_INLINE RecordAccess & operator = (Int);
    RNA_INLINE RecordAccess & operator = (Str);
    RNA_INLINE RecordAccess & operator = (Record *);
    RNA_INLINE RecordAccess & operator = (Array *);
    RNA_INLINE RecordAccess & operator = (List *);
    RNA_INLINE RecordAccess & operator = (TypeDescriptor *);

    RNA_INLINE RecordAccess operator () (Index);
    RNA_INLINE RecordAccess operator () (Str);
    RNA_INLINE ArrayAccess operator [] (Index);
    RNA_INLINE ListAccess operator [] (Str);

protected:

    // Instance Creation
    RNA_INLINE RecordAccess (Record * target, Index index);
    friend class Rna;
    friend class Record;
    friend class ArrayAccess;
    friend class ListAccess;

private:

    // Members
    Record * target;
    Index index;
    
    // do not copy!
    RNA_INLINE const RecordAccess & operator = (const RecordAccess &);
};


/******************************************************************\
 *                                                                *
 * Array Access Proxy                                             *
 *                                                                *
\******************************************************************/

class ArrayAccess
{
public:
    // Copying
    RNA_INLINE ArrayAccess(const ArrayAccess&);
    
    // @@@ TYPELIST @@@
    RNA_INLINE operator Rna ();
    RNA_INLINE operator Float ();
    RNA_INLINE operator Int ();
    RNA_INLINE operator Str ();
    RNA_INLINE operator Record * ();
    RNA_INLINE operator Array * ();
    RNA_INLINE operator List * ();
    
    // @@@ TYPELIST @@@
    RNA_INLINE ArrayAccess & operator = (Rna);
    RNA_INLINE ArrayAccess & operator = (Float);
    RNA_INLINE ArrayAccess & operator = (Int);
    RNA_INLINE ArrayAccess & operator = (Str);
    RNA_INLINE ArrayAccess & operator = (Record *);
    RNA_INLINE ArrayAccess & operator = (Array *);
    RNA_INLINE ArrayAccess & operator = (List *);

    RNA_INLINE RecordAccess operator () (Index);
    RNA_INLINE RecordAccess operator () (Str);
    RNA_INLINE ArrayAccess operator [] (Index);
    RNA_INLINE ListAccess operator [] (Str);

protected:

    // Instance Creation
    RNA_INLINE ArrayAccess (Array * target, Index index);
    friend class Rna;
    friend class RecordAccess;
    friend class ListAccess;

private:
    
    // Members
    Array * target;
    Index index;

    // do not copy!
    RNA_INLINE const ArrayAccess & operator = (const ArrayAccess &);
};


/******************************************************************\
 *                                                                *
 * List Access Proxy                                              *
 *                                                                *
\******************************************************************/

class ListAccess
{
public:
    // Copying 
    RNA_INLINE ListAccess (const ListAccess &);

    // @@@ TYPELIST @@@
    RNA_INLINE operator Rna ();
    RNA_INLINE operator Float ();
    RNA_INLINE operator Int ();
    RNA_INLINE operator Str ();
    RNA_INLINE operator Record * ();
    RNA_INLINE operator Array * ();
    RNA_INLINE operator List * ();

    // @@@ TYPELIST @@@
    RNA_INLINE ListAccess & operator = (Rna);
    RNA_INLINE ListAccess & operator = (Float);
    RNA_INLINE ListAccess & operator = (Int);
    RNA_INLINE ListAccess & operator = (Str);
    RNA_INLINE ListAccess & operator = (Record *);
    RNA_INLINE ListAccess & operator = (Array *);
    RNA_INLINE ListAccess & operator = (List *);

    RNA_INLINE RecordAccess operator () (Index);
    RNA_INLINE RecordAccess operator () (Str);
    RNA_INLINE ArrayAccess operator [] (Index);
    RNA_INLINE ListAccess operator [] (Str);

    RNA_INLINE void clear ();

protected:

    // Instance Creation
    RNA_INLINE ListAccess (Record * target, Str name);
    friend class Rna;
    friend class RecordAccess;
    friend class ArrayAccess;

private:

    // Members
    Record * target;
    Str name;

    // do not copy!
    RNA_INLINE const ListAccess & operator = (const ListAccess &);
};

# endif /* RNA_SYNTACTIC_SUGAR */

#endif /* RNA_SUGAR_H */
