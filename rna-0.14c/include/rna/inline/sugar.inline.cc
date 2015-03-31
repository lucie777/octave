/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/11 08:05:04 $
 * $Source: /sys/cvs/rna/include/rna/inline/sugar.inline.cc,v $
 * $Revision: 1.3 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

# ifdef RNA_SYNTACTIC_SUGAR

/******************************************************************\
 *                                                                *
 * Syntactic Sugar - Rna                         @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE 
Rna::operator Float ()
{
    return readFloat ();
}

RNA_INLINE 
Rna::operator Int ()
{
    return readInt ();
}

RNA_INLINE 
Rna::operator Str ()
{
    return readStr ();
}

RNA_INLINE 
Rna::operator Record * ()
{
    return readRecord ();
}

RNA_INLINE 
Rna::operator Array * ()
{
    return readArray ();
}

RNA_INLINE 
Rna::operator List * ()
{
    return readList ();
}

RNA_INLINE 
Rna::operator TypeDescriptor * ()
{
    return TypeDescriptor::Convert (readRecord ());
}

RNA_INLINE Rna &
Rna::operator = (Float ff)
{
    writeFloat (ff);

    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (Int ii)
{
    writeInt (ii);

    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (Str ss)
{
    writeStr (ss);

    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (Record * record)
{
    writeRecord (record);
    
    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (Array * array)
{
    writeArray (array);
    
    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (List * list)
{
    writeList (list);
    
    return SELF;
}

RNA_INLINE Rna &
Rna::operator = (TypeDescriptor * typedescriptor)
{
    writeRecord (typedescriptor);
    
    return SELF;
}


/******************************************************************\
 *                                                                *
 * Syntactic Sugar - Access Constructors                          *
 *                                                                *
\******************************************************************/

RNA_INLINE 
RecordAccess::RecordAccess (Record * target1, Index index1)
    : target (target1),
      index (index1)
{}

RNA_INLINE 
RecordAccess::RecordAccess (const RecordAccess & aa)
    : target (aa.target),
      index (aa.index)
{}

RNA_INLINE 
ArrayAccess::ArrayAccess (Array * target1, Index index1)
    : target (target1),
      index (index1)
{}

RNA_INLINE 
ArrayAccess::ArrayAccess (const ArrayAccess & ba)
    : target (ba.target),
      index (ba.index)
{}

RNA_INLINE 
ListAccess::ListAccess (Record * target1, Str name1)
    : target (target1),
      name (name1)
{}

RNA_INLINE 
ListAccess::ListAccess (const ListAccess & ga)
    : target (ga.target),
      name (ga.name)
{}

/******************************************************************\
 *                                                                *
 * Syntactic Sugar - Record Access                @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE RecordAccess
Rna::operator () (Index xx)
{
    return RecordAccess (readRecord (), xx);
}

RNA_INLINE RecordAccess
Rna::operator () (Str name)
{
    Record * temp = readRecord ();
    Index xx = temp->getTypeInfo ()->lookup (name);
    return RecordAccess (temp, xx);
}

RNA_INLINE 
RecordAccess::operator Rna ()
{
    return target->get (index);
}

RNA_INLINE 
RecordAccess::operator Float ()
{
    return target->get (index).readFloat ();
}

RNA_INLINE 
RecordAccess::operator Int ()
{
    return target->get (index).readInt ();
}

RNA_INLINE 
RecordAccess::operator Str ()
{
    return target->get (index).readStr ();
}

RNA_INLINE 
RecordAccess::operator Record * ()
{
    return target->get (index).readRecord ();
}

RNA_INLINE 
RecordAccess::operator Array * ()
{
    return target->get (index).readArray ();
}

RNA_INLINE 
RecordAccess::operator List * ()
{
    return target->get (index).readList ();
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Rna rna)
{
    target->set (index, rna);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Float ff)
{
    target->set (index, ff);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Int ii)
{
    target->set (index, ii);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Str ss)
{
    target->set (index, ss);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Record * record)
{
    target->set (index, record);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (Array * array)
{
    target->set (index, array);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (List * list)
{
    target->set (index, list);
    return SELF;
}

RNA_INLINE RecordAccess &
RecordAccess::operator = (TypeDescriptor * typedescriptor)
{
    target->set (index, typedescriptor);
    return SELF;
}

RNA_INLINE RecordAccess 
RecordAccess::operator () (Index xx)
{
    return RecordAccess (target->get (index), xx);
}

RNA_INLINE RecordAccess 
RecordAccess::operator () (Str name)
{
    Record * temp = target->get (index).readRecord ();
    Int xx = temp->getTypeInfo ()->lookup (name);
    return RecordAccess (temp, xx);
}

RNA_INLINE ArrayAccess 
RecordAccess::operator [] (Index xx)
{
    return ArrayAccess (target->get (index).readArray (), xx);
}

RNA_INLINE ListAccess 
RecordAccess::operator [] (Str name)
{
    return ListAccess (target->get (index).readRecord (), name);
}


/******************************************************************\
 *                                                                *
 * Syntactic Sugar - Array Access                 @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE ArrayAccess
Rna::operator [] (Index xx)
{
    return ArrayAccess (readArray (), xx);
}

RNA_INLINE 
ArrayAccess::operator Rna ()
{
    return target->at (index);
}

RNA_INLINE 
ArrayAccess::operator Float ()
{
    return target->at (index).readFloat ();
}

RNA_INLINE 
ArrayAccess::operator Int ()
{
    return target->at (index).readInt ();
}

RNA_INLINE 
ArrayAccess::operator Str ()
{
    return target->at (index).readStr ();
}

RNA_INLINE 
ArrayAccess::operator Record * ()
{
    return target->at (index).readRecord ();
}

RNA_INLINE 
ArrayAccess::operator Array * ()
{
    return target->at (index).readArray ();
}

RNA_INLINE 
ArrayAccess::operator List * ()
{
    return target->at (index).readList ();
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Rna rna)
{
    target->atPut (index, rna);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Float ff)
{
    target->atPut (index, ff);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Int ii)
{
    target->atPut (index, ii);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Str ss)
{
    target->atPut (index, ss);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Record * record)
{
    target->atPut (index, record);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (Array * array)
{
    target->atPut (index, array);
    return SELF;
}

RNA_INLINE ArrayAccess &
ArrayAccess::operator = (List * list)
{
    target->atPut (index, list);
    return SELF;
}

RNA_INLINE RecordAccess 
ArrayAccess::operator () (Index xx)
{
    return RecordAccess (target->at (index).readRecord (), xx);
}

RNA_INLINE RecordAccess 
ArrayAccess::operator () (Str name)
{
    Record * temp = target->at (index).readRecord ();
    Int xx = temp->getTypeInfo ()->lookup (name);
    return RecordAccess (temp, xx);
}

RNA_INLINE ArrayAccess 
ArrayAccess::operator [] (Index xx)
{
    return ArrayAccess (target->at (index).readArray (), xx);
}

RNA_INLINE ListAccess 
ArrayAccess::operator [] (Str name)
{
    return ListAccess (target->at (index).readRecord (), name);
}


/******************************************************************\
 *                                                                *
 * Syntactic Sugar - List Access                @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE ListAccess
Rna::operator [] (Str name)
{
    return ListAccess (readRecord (), name);
}

RNA_INLINE 
ListAccess::operator Rna ()
{
    List * list = target->getAttr ()->find (name);
    if (list)
	return list->getCar ();
    else
	return Rna::Null ();
}

RNA_INLINE 
ListAccess::operator Float ()
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    return list->getCar ().readFloat ();
}

RNA_INLINE 
ListAccess::operator Int ()
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    return list->getCar ().readInt ();
}

RNA_INLINE 
ListAccess::operator Str ()
{
    List * list = target->getAttr ()->find (name);
    if (list)
	return list->getCar ().readStr ();
    else
	return Rna::Null ();
}

RNA_INLINE 
ListAccess::operator Record * ()
{
    List * list = target->getAttr ()->find (name);
    if (list)
	return list->getCar ().readRecord ();
    else
	return Rna::Null ();
}

RNA_INLINE 
ListAccess::operator Array * ()
{
    List * list = target->getAttr ()->find (name);
    if (list)
	return list->getCar ().readArray ();
    else
	return Rna::Null ();
}

RNA_INLINE 
ListAccess::operator List * ()
{
    List * list = target->getAttr ()->find (name);
    if (list)
	return list->getCar ().readList ();
    else
	return Rna::Null ();
}

#define MEMORY (Memory::Main ())

RNA_INLINE ListAccess &
ListAccess::operator = (Rna rna)
{
    Record * record = rna.readRecord ();
    if (record) {
	List * list = target->getAttr ()->find (name);
	if (list)
	    list->setCar (rna);
	else {
	    list = List::Create (MEMORY, name, RECORD, record, 
				 target->getAttr ());
	    target->setAttr (list);
	}
	list->setType (RECORD);
    } else {
	List * attr = target->getAttr ();
	List::remove (name, attr);
	target->setAttr (attr);
    }
    return SELF;
}

RNA_INLINE ListAccess &
ListAccess::operator = (Int ii)
{
    List * list = target->getAttr ()->find (name);
    if (list)
	list->setCar (Rna (ii));
    else {
	list = List::Create (MEMORY, name, INT, Rna (ii), 
			     target->getAttr ());
	target->setAttr (list);
    }
    list->setType (INT);
    return SELF;
}

RNA_INLINE ListAccess &
ListAccess::operator = (Record * record)
{
    if (record) {
	List * list = target->getAttr ()->find (name);
	if (list)
	    list->setCar (record);
	else {
	    list = List::Create (MEMORY, name, RECORD, record, 
				 target->getAttr ());
	    target->setAttr (list);
	}
	list->setType (RECORD);
    } else {
	List * attr = target->getAttr ();
	List::remove (name, attr);
	target->setAttr (attr);
    }
    return SELF;
}

RNA_INLINE ListAccess &
ListAccess::operator = (Array * array)
{
    if (array) {
	List * list = target->getAttr ()->find (name);
	if (list)
	    list->setCar (array);
	else {
	    list = List::Create (MEMORY, name, ARRAY, array, 
				 target->getAttr ());
	    target->setAttr (list);
	}
	list->setType (ARRAY);
    } else {
	List * attr = target->getAttr ();
	List::remove (name, attr);
	target->setAttr (attr);
    }
    return SELF;
}

RNA_INLINE RecordAccess 
ListAccess::operator () (Index xx)
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    return RecordAccess (list->getCar ().readRecord (), xx);
}

RNA_INLINE RecordAccess 
ListAccess::operator () (Str name1)
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    Record * temp = list->getCar ().readRecord ();
    Int xx = temp->getTypeInfo ()->lookup (name1);
    return RecordAccess (temp, xx);
}

RNA_INLINE ArrayAccess 
ListAccess::operator [] (Index xx)
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    return ArrayAccess (list->getCar ().readArray (), xx);
}

RNA_INLINE ListAccess 
ListAccess::operator [] (Str name1)
{
    List * list = target->getAttr ()->find (name);
    RNA_ASSERT (list);
    return ListAccess (list->getCar ().readRecord (), name1);
}

/******************************************************************\
 *                                                                *
 * Syntactic Sugar - Rna                         @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE
Rna::Rna (RecordAccess aa)
# ifdef RNA_TYPECHECK
    : _type (ANY)
# endif /* RNA_TYPECHECK */
{
    SELF = aa.target->get (aa.index);
}

RNA_INLINE
Rna::Rna (ArrayAccess ba)
# ifdef RNA_TYPECHECK
    : _type (ANY)
# endif /* RNA_TYPECHECK */
{
    SELF = ba.target->at (ba.index);
}

RNA_INLINE
Rna::Rna (ListAccess ga)
# ifdef RNA_TYPECHECK
    : _type (ANY)
# endif /* RNA_TYPECHECK */
{
    List * list = ga.target->getAttr ()->find (ga.name);
    if (list)
	SELF = list->getCar ();
    else
	SELF = Rna::Null ();
}

RNA_INLINE Rna & 
Rna::operator = (RecordAccess aa)
{
    SELF = aa.target->get (aa.index);
    return SELF;
}

RNA_INLINE Rna & 
Rna::operator = (ArrayAccess ba)
{
    SELF = ba.target->at (ba.index);
    return SELF;
}

RNA_INLINE Rna & 
Rna::operator = (ListAccess ga)
{
    List * list = ga.target->getAttr ()->find (ga.name);
    if (list)
	SELF = list->getCar ();
    else
	SELF = Rna::Null ();

    return SELF;
}


# endif /* RNA_SYNTACTIC_SUGAR */

#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
