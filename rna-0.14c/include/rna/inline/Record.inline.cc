/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/inline/Record.inline.cc,v $
 * $Revision: 1.2 $
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

    Word w; w.record = result; RNA_USE(w);
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
    Word w; w.record = this; RNA_USE(w);
    return _typeinfo;
}

RNA_INLINE void 
Record::setTypeInfo (TypeDescriptor * typeinfo)
{
    Word w; w.record = this; RNA_USE(w);
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
    Word w; w.record = this; RNA_USE(w);
    return _attr;
}

RNA_INLINE void 
Record::setAttr (List * attr)
{
    Word w; w.record = this; RNA_USE(w);
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
    Word w; w.record = this; RNA_USE(w);
    RNA_USE(_item [xx]);

# ifndef RNA_TYPECHECK
    return Rna (_item [xx]);
# else /* RNA_TYPECHECK */
    return Rna (_item [xx], (Type) _typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss [xx]);
# endif /* RNA_TYPECHECK */
}

RNA_INLINE void
Record::set (Index xx, Rna value)
{
    Word w; w.record = this; RNA_USE(w);
    RNA_USE(value._word);

    RNA_ASSERT (0 <= xx && xx < _typeinfo->_item [TypeDescriptor::IDX_SIZE].ii);
# ifdef RNA_TYPECHECK
    RNA_ASSERT (value._type == _typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss [xx]
		|| (value._type == ANY && value._word.ii == 0));
# endif /* !RNA_TYPECHECK */
    _item [xx] = value._word;
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
