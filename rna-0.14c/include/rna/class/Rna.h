/* -*-c++-*- */

/* Copyright (c) 1998-2003 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:48 $
 * $Source: /sys/cvs/rna/include/rna/class/Rna.h,v $
 * $Revision: 1.2 $
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
    RNA_INLINE Rna (Bool bb);
    RNA_INLINE Rna (Char cc);
    RNA_INLINE Rna (Int ii);
    RNA_INLINE Rna (Float ff);
    RNA_INLINE Rna (Ptr pp);
    RNA_INLINE Rna (Ref rr);
    RNA_INLINE Rna (Str ss);
    RNA_INLINE Rna (Record * record);
    RNA_INLINE Rna (Array * array);
    RNA_INLINE Rna (List * list);
    RNA_INLINE Rna (Varg varg);


    // Access - Read
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
    RNA_INLINE Varg readVarg ();


    // Access - Write
    RNA_INLINE void writeBool (Bool bb);
    RNA_INLINE void writeChar (Char cc);
    RNA_INLINE void writeInt (Int ii);
    RNA_INLINE void writeFloat (Float ff);
    RNA_INLINE void writePtr (Ptr pp);
    RNA_INLINE void writeRef (Ref rr);
    RNA_INLINE void writeStr (Str ss);
    RNA_INLINE void writeRecord (Record * record);
    RNA_INLINE void writeArray (Array * array);
    RNA_INLINE void writeList (List * list);
    RNA_INLINE void writeVarg (Varg varg);


    // Type Checking
    RNA_INLINE Word getWord () const;
    Type getType () const;

    // Null Singleton
    static RNA_INLINE Rna Null ();
    RNA_INLINE Bool isNull () const;

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
    friend class Array;
    friend class AsciiSerializer;
    friend class BinarySerializer;
    friend class CompressedStream;
    friend class Context;
    friend class Empty;
    friend class Error;
    friend class File;
    friend class Initialization;
    friend class IntContext;
    friend class List;
    friend class Memory;
    friend class Module;
    friend class Position;
    friend class Record;
    friend class Serializer;
    friend class Socket;
    friend class StrContext;
    friend class Stream;
    friend class TypeDescriptor;
    friend class Variant;
    friend class Venue;
    friend class Pointer;
    friend class RecordAccess;
    friend class ArrayAccess;
    friend class ListAccess;

};

#endif /* RNA_CLASS_RNA_H */
