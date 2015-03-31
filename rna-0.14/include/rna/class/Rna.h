/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Rna.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_RNA_H
#define RNA_CLASS_RNA_H

class Rna
{
public:

    // Constructors/Destructors
    RNA_INLINE Rna ();
    RNA_INLINE Rna (const Rna & rna);
    RNA_INLINE Rna operator = (const Rna & rna);
    RNA_INLINE ~Rna ();

    // Instance Creation
    // @@@ TYPELIST @@@
    RNA_INLINE Rna (Bool bb);
    RNA_INLINE Rna (Char cc);
    RNA_INLINE Rna (Int ii);
    RNA_INLINE Rna (Float ff);
    RNA_INLINE Rna (Ptr pp);
    RNA_INLINE Rna (Ref rr);
    RNA_INLINE Rna (Str ss);
    RNA_INLINE Rna (Record *);
    RNA_INLINE Rna (Array *);
    RNA_INLINE Rna (List *);

    // Access - Read
    // @@@ TYPELIST @@@
    RNA_INLINE Bool readBool ();
    RNA_INLINE Char readChar ();
    RNA_INLINE Int readInt ();
    RNA_INLINE Float readFloat ();
    RNA_INLINE Ptr readPtr ();
    RNA_INLINE Ref readRef ();
    RNA_INLINE Str readStr ();
    RNA_INLINE Record * readRecord ();
    RNA_INLINE Array * readArray ();
    RNA_INLINE List * readList ();

    // Access - Write
    // @@@ TYPELIST @@@
    RNA_INLINE void writeBool (Bool);
    RNA_INLINE void writeChar (Char);
    RNA_INLINE void writeInt (Int);
    RNA_INLINE void writeFloat (Float);
    RNA_INLINE void writePtr (Ptr);
    RNA_INLINE void writeRef (Ref);
    RNA_INLINE void writeStr (Str);
    RNA_INLINE void writeRecord (Record *);
    RNA_INLINE void writeArray (Array *);
    RNA_INLINE void writeList (List *);

    // Type Checking
    RNA_INLINE Word getWord () const;
    Type getType () const;

    // Null Singleton
    static RNA_INLINE Rna Null ();
    RNA_INLINE Bool isNull () const;

    // Lifting Types
    Rna Lift (Type type, Memory * s);
    Rna Unlift ();
    
    // Comparing
    RNA_INLINE Bool operator == (Rna rna);
    RNA_INLINE Bool operator != (Rna rna);

    // Copying
    RNA_INLINE Rna clone (Memory * s, Type type = RECORD);
    Rna copy (Memory * s, Type type = RECORD);

    // Serializer
    static Rna Read (Memory * s, Stream *, Type & type);
    void Write (Stream *, Type type);

# ifdef RNA_SYNTACTIC_SUGAR
    // Instance creation
    RNA_INLINE Rna (RecordAccess);
    RNA_INLINE Rna (ArrayAccess);
    RNA_INLINE Rna (ListAccess);

    // Conversions
    // @@@ TYPELIST @@@
    RNA_INLINE operator Float ();
    RNA_INLINE operator Int ();
    RNA_INLINE operator Str ();
    RNA_INLINE operator Record * ();
    RNA_INLINE operator Array * ();
    RNA_INLINE operator List * ();
    RNA_INLINE operator TypeDescriptor * ();

    // Assignments
    // @@@ TYPELIST @@@
    RNA_INLINE Rna & operator = (Float);
    RNA_INLINE Rna & operator = (Int);
    RNA_INLINE Rna & operator = (Str);
    RNA_INLINE Rna & operator = (Record *);
    RNA_INLINE Rna & operator = (Array *);
    RNA_INLINE Rna & operator = (List *);
    RNA_INLINE Rna & operator = (TypeDescriptor *);

    RNA_INLINE Rna & operator = (RecordAccess);
    RNA_INLINE Rna & operator = (ArrayAccess);
    RNA_INLINE Rna & operator = (ListAccess);

    // Access - Composite
    RNA_INLINE RecordAccess operator () (Index);
    RNA_INLINE RecordAccess operator () (Str);
    RNA_INLINE ArrayAccess operator [] (Index);
    RNA_INLINE ListAccess operator [] (Str);
# endif /* RNA_SYNTACTIC_SUGAR */

//protected:
    
    // Members
    Word _word;
# ifdef RNA_TYPECHECK
    Type _type;
# endif /* RNA_TYPECHECK */

    // Null Singleton
    static Rna _Null;

    // Lifting Types
    static Bool is_lift (TypeDescriptor *);
    static TypeDescriptor * create_lift (char * name, char * format);
    // @@@ TYPELIST @@@
    static TypeDescriptor * _Lift_Bool;
    static TypeDescriptor * _Lift_Char;
    static TypeDescriptor * _Lift_Int;
    static TypeDescriptor * _Lift_Float;
    static TypeDescriptor * _Lift_Ptr;
    static TypeDescriptor * _Lift_Ref;
    static TypeDescriptor * _Lift_Str;
    static TypeDescriptor * _Lift_Record;
    static TypeDescriptor * _Lift_Array;
    static TypeDescriptor * _Lift_List;

    // Instance Creation
# ifndef RNA_TYPECHECK
    RNA_INLINE Rna (Word word);
# else /* RNA_TYPECHECK */
    RNA_INLINE Rna (Word word, Type type);
# endif /* RNA_TYPECHECK */

private:

    // Class Initialization
    static void Initialize ();

    // Friends
    friend class Memory;
    friend class Record;
    friend class AsciiSerializer;
    friend class Array;
    friend class List;
    friend class Initialization;
    friend class Context;
    friend class StrContext;
    friend class IntContext;

    friend class TypeDescriptor;
    friend class Position;
};

#endif /* RNA_CLASS_RNA_H */
