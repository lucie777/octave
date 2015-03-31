/* -*-c++-*- */

/* Copyright (c) 1998-2003 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:47 $
 * $Source: /sys/cvs/rna/include/rna/rna.h,v $
 * $Revision: 1.2 $
 */


#ifndef RNA_H
#define RNA_H

/******************************************************************\
 *                                                                *
 * Setup                                                          *
 *                                                                *
\******************************************************************/

// determine macro settings

// possible macros:
// - RNA_DEBUG:        Enables assertions
// - RNA_TYPECHECK:    Enables strict type checking
// - RNA_TRACE:        Enables Rna tracing
// - RNA_USE_INLINE:   Implement common functions inline

// - RNA_HAVE_NAMESPACE: Use C++ namespaces
// - RNA_HAVE_LIBC:      Use standard C-library functionality
// - RNA_HAVE_OBSTACK:   Use GNU obstacks for memory management
// - RNA_HAVE_ZLIB:      Support zlib-compressed binary streams

// - RNA_IMPLEMENTATION: Define when including from RNA implementation source typeinfo

// - RNA_SYNTACTIC_SUGAR:   Turn on sophisticated accessor functions
// - RNA_AUSTERE:        Turn off any unncessary stuff

#define RNA_DEBUG
#define RNA_TYPECHECK
//#define RNA_TRACE
//#define RNA_USE_INLINE
#define RNA_HAVE_NAMESPACE
#define RNA_HAVE_LIBC
#define RNA_HAVE_OBSTACK
#define RNA_HAVE_ZLIB
#define RNA_SYNTACTIC_SUGAR

#ifdef RNA_USE_INLINE
# define RNA_INLINE inline
# define RNA_DEFINE_INLINE_FUNCTIONS_NOW
#else /* !RNA_USE_INLINE */
# define RNA_INLINE 
# undef RNA_DEFINE_INLINE_FUNCTIONS_NOW
#endif /* RNA_USE_INLINE */

#define RNA_INSIDE_HEADER

/******************************************************************\
 *                                                                *
 * Includes                                                       *
 *                                                                *
\******************************************************************/

// include system include files
#ifdef RNA_HAVE_OBSTACK
# include <obstack.h>
#endif /* RNA_HAVE_OBSTACK */
#ifdef RNA_HAVE_LIBC
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif /* RNA_HAVE_LIBC */
#ifdef RNA_HAVE_ZLIB
# include <zlib.h>
#endif /* RNA_HAVE_ZLIB */

#include <rna/ns-begin.h>

// include RNA word include files
#include <rna/macros.h>
#include <rna/types.h>
#include <rna/word.h>
#include <rna/sugar.h>


/******************************************************************\
 *                                                                *
 * Class Definitions                                              *
 *                                                                *
\******************************************************************/

#include <rna/class/Rna.h>
#include <rna/class/Array.h>
#include <rna/class/Context.h>
#include   <rna/class/IntContext.h>
#include   <rna/class/StrContext.h>
#include <rna/class/Error.h>
#include <rna/class/Initialization.h>
#include <rna/class/List.h>
#include <rna/class/Memory.h>
#include <rna/class/Record.h>
#include   <rna/class/Empty.h>
#include   <rna/class/Module.h>
#include   <rna/class/Position.h>
#include   <rna/class/TypeDescriptor.h>
#include   <rna/class/Variant.h>
#include <rna/class/Serializer.h>
#include   <rna/class/AsciiSerializer.h>
#include   <rna/class/BinarySerializer.h>
#include <rna/class/Stream.h>
#include   <rna/class/CompressedStream.h>
#include   <rna/class/File.h>
#include   <rna/class/Socket.h>
#include <rna/class/Venue.h>
#include <rna/class/Pointer.h>
#include   <rna/class/RecordAccess.h>
#include   <rna/class/ArrayAccess.h>
#include   <rna/class/ListAccess.h>


/******************************************************************\
 *                                                                *
 * Inline Implementations                                         *
 *                                                                *
\******************************************************************/

#ifdef RNA_USE_INLINE
# include <rna/inline/Rna.inline.cc>
# include <rna/inline/Array.inline.cc>
# include <rna/inline/List.inline.cc>
# include <rna/inline/Memory.inline.cc>
# include <rna/inline/Record.inline.cc>
# include <rna/inline/Venue.inline.cc>

# include <rna/inline/sugar.inline.cc>
#endif /* RNA_USE_INLINE */

/******************************************************************\
 *                                                                *
 * Includes                                                       *
 *                                                                *
\******************************************************************/

#include <rna/ns-end.h>
#include <rna/debug.h>

#undef RNA_INSIDE_HEADER
#ifdef RNA_USE_INLINE
# undef RNA_DEFINE_INLINE_FUNCTIONS_NOW
#else /* !RNA_USE_INLINE */
# define RNA_DEFINE_INLINE_FUNCTIONS_NOW
#endif /* RNA_USE_INLINE */

#endif /* RNA_H */
