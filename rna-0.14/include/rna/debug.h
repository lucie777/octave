/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/debug.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_DEBUG_H
#define RNA_DEBUG_H

extern "C" {
    void RNA_Catch (void);
    void pp (RNA_RNA rna, RNA_TYPE type);
    void fpp (FILE *, RNA_RNA rna, RNA_TYPE type);
}

#endif /* RNA_DEBUG_H */
