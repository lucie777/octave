/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/debug.h,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_DEBUG_H
#define RNA_DEBUG_H

extern "C" {
    void RNA_Catch (void);
    void pp (RNA_WORD rna, RNA_TYPE type);
    void fpp (FILE *, RNA_WORD rna, RNA_TYPE type);
    void RNA_TryCatch (RNA_WORD item);
    void RNA_Watch (RNA_WORD item);
}

#endif /* RNA_DEBUG_H */
