/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/macros.h,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_MACROS_H
#define RNA_MACROS_H

/******************************************************************\
 *                                                                *
 * Preprocessor Operations                                        *
 *                                                                *
\******************************************************************/

#define CONC(A,B) A ## B
#define CONC3(A,B,C) A ## B ## C

#define MKSTRING(A) # A

/******************************************************************\
 *                                                                *
 * Useful Declarations                                            *
 *                                                                *
\******************************************************************/

#define SELF (*this)

#define Case break;case
#define Default break;default

#define repeat while (true)


/******************************************************************\
 *                                                                *
 * Assertions                                                     *
 *                                                                *
\******************************************************************/

#ifdef RNA_DEBUG

// macros
# define RNA_SHOULD_NOT_REACH_HERE do {					\
    RNA_ERROR_INSTANCE->ShouldNotReachHere (__FILE__, __LINE__);	\
} while(0)
# define RNA_ASSERT(EXPR) do {						     \
    if ( !(EXPR) )							     \
	RNA_ERROR_INSTANCE->AssertFail (MKSTRING(EXPR), __FILE__, __LINE__); \
} while (0)
# define RNA_WATCH(ITEM) do { 			\
    RNA_ERROR_INSTANCE->Watch ((ITEM));		\
} while (0)
# define RNA_USE(ITEM) do {			\
    RNA_ERROR_INSTANCE->Use ((ITEM));		\
} while (0)

#else /* !RNA_DEBUG */
# define RNA_SHOULD_NOT_REACH_HERE do {} while(0)
# define RNA_ASSERT(EXPR) do {} while(0)
# define RNA_WATCH(ITEM) do {} while (0)
# define RNA_USE(ITEM) do {} while (0)
#endif /* RNA_DEBUG */

/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

#define RNA_ERROR(TEXT) do {						\
    RNA_ERROR_INSTANCE->Message (__FILE__, __LINE__, -1, "%s", (TEXT));	\
} while (0)


#endif /* RNA_MACROS_H */
