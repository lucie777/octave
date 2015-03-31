/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/ns-begin.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_NAMESPACE_BEGIN_H
#define RNA_NAMESPACE_BEGIN_H

#ifdef RNA_HAVE_NAMESPACE

namespace RNA {

#else /* !RNA_HAVE_NAMESPACE */

// @@@ CLASSLIST @@@
# define TypeDescriptor RNA_TypeDescriptor
# define Record RNA_Record
# define AsciiSerializer RNA_AsciiSerializer
# define Array RNA_Array
# define BinarySerializer RNA_BinarySerializer
# define CompressedStream RNA_CompressedStream
# define Empty RNA_Empty
# define Error RNA_Error
# define File RNA_File
# define List RNA_List
# define Initialization RNA_Initialization
# define IntContext RNA_IntContext
# define StrContext RNA_StrContext
# define Position RNA_Position
# define Rna RNA_Rna
# define Socket RNA_Socket
# define Memory RNA_Memory
# define Stream RNA_Stream
# define Context RNA_Context
# define Serializer RNA_Serializer

// @@@ TYPELIST @@@
# define Bool RNA_Bool
# define Char RNA_Char
# define Int RNA_Int
# define Float RNA_Float
# define Ptr RNA_Ptr
# define Ref RNA_Ref
# define Str RNA_Str

# define Word RNA_Word
# define Index RNA_Index
# define Size RNA_Size
# define Format RNA_Format
# define Varg RNA_Varg
# define constStr RNA_constStr

// @@@ TYPELIST @@@
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

# define RecordAccess RNA_RecordAccess
# define ArrayAccess RNA_ArrayAccess
# define ListAccess RNA_ListAccess

# define INDEX_NOT_FOUND RNA_INDEX_NOT_FOUND

#endif /* RNA_HAVE_NAMESPACE */

# define RNA_ERROR_INSTANCE (Error::Instance ())

#endif /* RNA_NAMESPACE_BEGIN_H */

