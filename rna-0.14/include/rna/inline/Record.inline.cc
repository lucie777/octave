/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/inline/Record.inline.cc,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

RNA_INLINE Record *
Record::Create (Memory * s, TypeDescriptor * typeinfo, List * attr)
{
    Record * result;

    Index size = typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
    result = (Record *) s->alloc
	(sizeof (Record) + size * sizeof (Rna));
    
    result->_typeinfo = typeinfo;
    result->_attr = attr;

    return result;
}


/******************************************************************\
 *                                                                *
 * Access - typeinfo                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE TypeDescriptor *
Record::getTypeInfo ()
{
    return _typeinfo;
}

RNA_INLINE void 
Record::setTypeInfo (TypeDescriptor * typeinfo)
{
    _typeinfo = typeinfo;
}


/******************************************************************\
 *                                                                *
 * Access - attr                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE List * 
Record::getAttr ()
{
    return _attr;
}

RNA_INLINE void 
Record::setAttr (List * attr)
{
    _attr = attr;
}


/******************************************************************\
 *                                                                *
 * Access - main                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE Rna
Record::get (Index xx)
{
    RNA_ASSERT (0 <= xx && xx < _typeinfo->_item [TypeDescriptor::IDX_SIZE].ii);

# ifndef RNA_TYPECHECK
    return Rna (_item [xx]);
# else /* RNA_TYPECHECK */
    return Rna (_item [xx], (Type) _typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss [xx]);
# endif /* RNA_TYPECHECK */
}

RNA_INLINE void
Record::set (Index xx, Rna value)
{
    RNA_ASSERT (0 <= xx && xx < _typeinfo->_item [TypeDescriptor::IDX_SIZE].ii);
# ifdef RNA_TYPECHECK
    RNA_ASSERT (value._type == _typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss [xx]
		|| (value._type == ANY && value._word.ii == 0));
# endif /* !RNA_TYPECHECK */
    _item [xx] = value._word;
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
