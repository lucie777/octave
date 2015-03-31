/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/ns-end.h,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_NAMESPACE_END_H
#define RNA_NAMESPACE_END_H

# undef RNA_ERROR_INSTANCE

#ifdef RNA_HAVE_NAMESPACE

} /* namespace RNA */

# ifdef RNA_IMPLEMENTATION
using namespace RNA;
# endif /* RNA_IMPLEMENTATION */

# define RNA_RNA RNA::Rna
# define RNA_WORD RNA::Word
# define RNA_TYPE RNA::Type
# define RNA_ERROR_INSTANCE (RNA::Error::Instance ())

#else /* !RNA_HAVE_NAMESPACE */

# ifndef RNA_IMPLEMENTATION

// @@@ CLASSLIST @@@
#   undef TypeDescriptor
#   undef Record
#   undef AsciiSerializer
#   undef Array
#   undef BinarySerializer
#   undef CompressedStream
#   undef Empty
#   undef Error
#   undef File
#   undef List
#   undef Initialization
#   undef IntContext
#   undef StrContext
#   undef Position
#   undef Rna
#   undef Socket
#   undef Memory
#   undef Stream
#   undef Context
#   undef Serializer

// @@@ TYPELIST @@@
#   undef Bool
#   undef Char
#   undef Int
#   undef Float
#   undef Ptr
#   undef Ref
#   undef Str

#   undef Word
#   undef Index
#   undef Size
#   undef Format
#   undef Varg
#   undef constStr

// @@@ TYPELIST @@@
#   undef STOP
#   undef ANY
#   undef BOOL
#   undef CHAR
#   undef INT
#   undef FLOAT
#   undef PTR
#   undef REF
#   undef STR
#   undef RECORD
#   undef ARRAY
#   undef LIST

#   undef RecordAccess
#   undef ArrayAccess
#   undef ListAccess

#   undef INDEX_NOT_FOUND

# endif /* RNA_IMPLEMENTATION */

# define RNA_RNA RNA_Rna
# define RNA_TYPE RNA_Type
# define RNA_ERROR_INSTANCE (RNA_Error::Instance ())

#endif /* RNA_HAVE_NAMESPACE */

#endif /* RNA_NAMESPACE_END_H */

