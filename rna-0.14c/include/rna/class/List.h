/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/List.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_LIST_H
#define RNA_CLASS_LIST_H

class List 
{
public:

    // New/Delete
    RNA_INLINE void * operator new (size_t, Memory &);
    RNA_INLINE void * operator new (size_t, Memory *);
    RNA_INLINE void operator delete (void *, size_t, Memory &);
    RNA_INLINE void operator delete (void *, size_t, Memory *);

    // Instance Creation
    RNA_INLINE static List * Create (Memory * s, Str tag, Type type, Rna car, List * cdr);

    // Access - Tag
    RNA_INLINE Str getTag ();
    RNA_INLINE void setTag (Str);

    // Access - Type
    RNA_INLINE Type getType ();
    RNA_INLINE void setType (Type);

    // Access - CAR
    RNA_INLINE Rna getCar ();
    RNA_INLINE void setCar (Rna);

    // Access - CDR
    RNA_INLINE List * getCdr ();
    RNA_INLINE void setCdr (List *);

    // Copying
    RNA_INLINE List * clone (Memory * s);
    List * copy (Memory * s);
 
    // Linked List
    List * find (Str tag);
    static void add (List * item, List * & list);
    static List * remove (Str tag, List * & list);
    void walk (Bool (* function) (List * item,
				  Rna & user_data),
	       Rna & user_data);

# ifdef RNA_SYNTACTIC_SUGAR
    // Access - Composite
    RNA_INLINE RecordAccess operator [] (Str);
# endif /* RNA_SYNTACTIC_SUGAR */

protected:

    // Members
    Str _tag;
    Type _type;
    Rna _car;
    List * _cdr;

private:

    // Friends
    friend class AsciiSerializer;
    friend class Record;
    friend class TypeDescriptor;
};

#endif /* RNA_CLASS_LIST_H */
