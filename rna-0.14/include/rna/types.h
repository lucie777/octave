/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/types.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_TYPES_H
#define RNA_TYPES_H

/******************************************************************\
 *                                                                *
 * Class Declarations                                             *
 *                                                                *
\******************************************************************/

// @@@ CLASSLIST @@@
class TypeDescriptor;
class Record;
class AsciiSerializer;
class Array;
class BinarySerializer;
class CompressedStream;
class Empty;
class Error;
class File;
class List;
class Initialization;
class IntContext;
class StrContext;
class Position;
class Rna;
class Socket;
class Memory;
class Stream;
class Context;
class Serializer;
class Venue;


/******************************************************************\
 *                                                                *
 * Basic Types                                                    *
 *                                                                *
\******************************************************************/

// @@@ TYPELIST @@@
typedef bool Bool;
typedef char Char;
typedef float Float;
typedef int Int;
typedef void * Ptr;
typedef Rna * Ref;
typedef char * Str;

typedef Int Index;
typedef Int Size;
typedef Str Format;
typedef const char * constStr;
typedef int Varg;


#endif /* RNA_TYPES_H */
