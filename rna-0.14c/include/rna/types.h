/* -*-c++-*- */

/* Copyright (c) 1998-2003 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:47 $
 * $Source: /sys/cvs/rna/include/rna/types.h,v $
 * $Revision: 1.2 $
 */


#ifndef RNA_TYPES_H
#define RNA_TYPES_H

/******************************************************************\
 *                                                                *
 * Class Declarations                                             *
 *                                                                *
\******************************************************************/

class Rna;
class Array;
class AsciiSerializer;
class BinarySerializer;
class CompressedStream;
class Context;
class Empty;
class Error;
class File;
class Initialization;
class IntContext;
class List;
class Memory;
class Module;
class Position;
class Record;
class Serializer;
class Socket;
class StrContext;
class Stream;
class TypeDescriptor;
class Variant;
class Venue;
class Pointer;
class RecordAccess;
class ArrayAccess;
class ListAccess;

/******************************************************************\
 *                                                                *
 * Basic Types                                                    *
 *                                                                *
\******************************************************************/

typedef bool Bool;
typedef char Char;
typedef int Int;
typedef float Float;
typedef void * Ptr;
typedef Rna * Ref;
typedef char * Str;
typedef int Index;
typedef int Size;
typedef char * Format;
typedef const char * constStr;
typedef long Varg;


#endif /* RNA_TYPES_H */
