/* -*-c++-*- */

/* Copyright (c) 1998-2003 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:47 $
 * $Source: /sys/cvs/rna/include/rna/ns-begin.h,v $
 * $Revision: 1.2 $
 */


#ifndef RNA_NAMESPACE_BEGIN_H
#define RNA_NAMESPACE_BEGIN_H

#ifdef RNA_HAVE_NAMESPACE

namespace RNA {

#else /* !RNA_HAVE_NAMESPACE */

# define Rna RNA_Rna
# define Array RNA_Array
# define AsciiSerializer RNA_AsciiSerializer
# define BinarySerializer RNA_BinarySerializer
# define CompressedStream RNA_CompressedStream
# define Context RNA_Context
# define Empty RNA_Empty
# define Error RNA_Error
# define File RNA_File
# define Initialization RNA_Initialization
# define IntContext RNA_IntContext
# define List RNA_List
# define Memory RNA_Memory
# define Module RNA_Module
# define Position RNA_Position
# define Record RNA_Record
# define Serializer RNA_Serializer
# define Socket RNA_Socket
# define StrContext RNA_StrContext
# define Stream RNA_Stream
# define TypeDescriptor RNA_TypeDescriptor
# define Variant RNA_Variant
# define Venue RNA_Venue
# define Pointer RNA_Pointer
# define RecordAccess RNA_RecordAccess
# define ArrayAccess RNA_ArrayAccess
# define ListAccess RNA_ListAccess


# define Bool RNA_Bool
# define Char RNA_Char
# define Int RNA_Int
# define Float RNA_Float
# define Ptr RNA_Ptr
# define Ref RNA_Ref
# define Str RNA_Str
# define Index RNA_Index
# define Size RNA_Size
# define Format RNA_Format
# define constStr RNA_constStr
# define Varg RNA_Varg


# define STOP RNA_STOP
# define ANY RNA_ANY
# define BOOL RNA_BOOL
# define CHAR RNA_CHAR
# define INT RNA_INT
# define FLOAT RNA_FLOAT
# define PTR RNA_PTR
# define REF RNA_REF
# define STR RNA_STR
# define RECORD RNA_RECORD
# define ARRAY RNA_ARRAY
# define LIST RNA_LIST


# define Word RNA_Word

# define RecordAccess RNA_RecordAccess
# define ArrayAccess RNA_ArrayAccess
# define ListAccess RNA_ListAccess

# define INDEX_NOT_FOUND RNA_INDEX_NOT_FOUND

#endif /* RNA_HAVE_NAMESPACE */

# define RNA_ERROR_INSTANCE (Error::Instance ())

#endif /* RNA_NAMESPACE_BEGIN_H */

