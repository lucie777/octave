/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/inline/clone.inline.cc,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

/******************************************************************\
 *                                                                *
 * Cloning RNA                                                    *
 *                                                                *
\******************************************************************/

RNA_INLINE Record *
Record::clone (Memory * s)
{
    s->startRecord (_typeinfo);
    
    Size size = _typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
    
    for (int i = 0; i < size; i++) {
# ifndef RNA_TYPECHECK
	s->grow (Rna (_item [i]));
# else /* RNA_TYPECHECK */
	s->grow (Rna (_item [i], (Type) _typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss [i]));
# endif /* RNA_TYPECHECK */
    }
    
    Record * result = s->finishRecord ();
    
    result->_attr = _attr->clone (s);

    return result;
}

RNA_INLINE Array *
Array::clone (Memory * s)
{
    s->startArray (_length, _type);

    for (int i = 0; i < _length; i++) {
# ifndef RNA_TYPECHECK
	s->grow (Rna (_item [i]));
# else /* RNA_TYPECHECK */
	s->grow (Rna (_item [i], _type));
# endif /* RNA_TYPECHECK */
    }
    
    return s->finishArray ();
}

RNA_INLINE List *
List::clone (Memory * s)
{
    if (this == 0) {
	return this;
    } else {
	List * result = (List *) s->alloc (sizeof (List));
	
	result->_tag = s->alloc0 (_tag);
	result->_car = _car;
	result->_cdr = _cdr->clone (s);
	
	return result;
    }
}

RNA_INLINE Rna 
Rna::clone (Memory * s, Type type)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == type);
# endif /* RNA_TYPECHECK */

    switch (type) {
    case RECORD:
	return _word.record->clone (s);
    case ARRAY:
	return _word.array->clone (s);
    case LIST:
	return _word.list->clone (s);
    case STR:
	return s->alloc0 (_word.ss);
    default:
	return SELF;
    }
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
