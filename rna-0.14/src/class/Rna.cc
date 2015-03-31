/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/07/16 20:18:50 $
 * $Source: /sys/cvs/rna/src/class/Rna.cc,v $
 * $Revision: 1.2 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Access - Type                                                  *
 *                                                                *
\******************************************************************/

Type 
Rna::getType () const
{ 
# ifdef RNA_TYPECHECK
    return _type;
# else /* !RNA_TYPECHECK */
    return ANY;
# endif /* RNA_TYPECHECK */
}


/******************************************************************\
 *                                                                *
 * Null Singleton                                                 *
 *                                                                *
\******************************************************************/

Rna Rna::_Null;

/******************************************************************\
 *                                                                *
 * Serializer                                                  *
 *                                                                *
\******************************************************************/

Rna 
Rna::Read (Memory * s, Stream * stream, Type & type)
{
    Serializer * serializer = Serializer::Create (stream);
    serializer->set_memory (s);

    Rna result = serializer->read (type);

    serializer->set_stream (0);
    serializer->set_memory (0);
    delete serializer;

    return result;
}

void 
Rna::Write (Stream * stream, Type type)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == type);
# endif /* RNA_TYPECHECK */

    Serializer * serializer = new AsciiSerializer;
    serializer->set_stream (stream);
    serializer->set_memory (new Memory);
    serializer->set_context (new StrContext);

    serializer->write (SELF, type);

    serializer->set_stream (0);
    delete serializer;
}


/******************************************************************\
 *                                                                *
 * Lifting Types                                                  *
 *                                                                *
\******************************************************************/

Rna 
Rna::Lift (Type type, Memory * s)
{
    TypeDescriptor * typedescriptor = 0;
    switch (type) {
    // @@@ TYPELIST @@@
    case BOOL: typedescriptor = _Lift_Bool;
    Case CHAR: typedescriptor = _Lift_Char;
    Case INT: typedescriptor = _Lift_Int;
    Case FLOAT: typedescriptor = _Lift_Float;
    Case PTR: typedescriptor = _Lift_Ptr;
    Case REF: typedescriptor = _Lift_Ref;
    Case STR: typedescriptor = _Lift_Str;
    Case RECORD: typedescriptor = _Lift_Record;
    Case ARRAY: typedescriptor = _Lift_Array;
    Case LIST: typedescriptor = _Lift_List;
    Default: RNA_SHOULD_NOT_REACH_HERE;
    }

    Record * result = Record::Create (s, typedescriptor);

    result->set (0, SELF);

    return result;
}

Bool
Rna::is_lift (TypeDescriptor * typeinfo)
{
    // @@@ TYPELIST @@@
    return typeinfo == _Lift_Bool
	|| typeinfo == _Lift_Char
	|| typeinfo == _Lift_Int
	|| typeinfo == _Lift_Float
	|| typeinfo == _Lift_Ptr
	|| typeinfo == _Lift_Ref
	|| typeinfo == _Lift_Str
	|| typeinfo == _Lift_Record
	|| typeinfo == _Lift_Array
	|| typeinfo == _Lift_List;
}

Rna 
Rna::Unlift ()
{
    Rna result = SELF;

    if (is_lift (TypeDescriptor::Convert (result.readRecord ()->getTypeInfo ())))
	return readRecord ()->get (0).Unlift ();
    else
	return SELF;
}

// @@@ TYPELIST @@@
TypeDescriptor * Rna::_Lift_Bool = 0;
TypeDescriptor * Rna::_Lift_Char = 0;
TypeDescriptor * Rna::_Lift_Int = 0;
TypeDescriptor * Rna::_Lift_Float = 0;
TypeDescriptor * Rna::_Lift_Ptr = 0;
TypeDescriptor * Rna::_Lift_Ref = 0;
TypeDescriptor * Rna::_Lift_Str = 0;
TypeDescriptor * Rna::_Lift_Record = 0;
TypeDescriptor * Rna::_Lift_Array = 0;
TypeDescriptor * Rna::_Lift_List = 0;

TypeDescriptor *
Rna::create_lift (char * name, char * format)
{
    RNA_ASSERT (strlen (format) == 1);

    Array * array = Array::Create (Memory::Main (), 1, STR);
    array->atPut (0, "value");

    TypeDescriptor * result = TypeDescriptor::Create (Memory::Main (),
				    name, 1, format, array);

    return result;
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
Rna::Initialize ()
{
# ifdef RNA_TYPECHECK
    _Null._type = ANY;
# endif /* RNA_TYPECHECK */

    _Null._word.record = 0;

    // @@@ TYPELIST @@@
    _Lift_Bool = create_lift ("Bool", "b");
    _Lift_Char = create_lift ("Char", "c");
    _Lift_Int = create_lift ("Int", "i");
    _Lift_Float = create_lift ("Float", "f");
    _Lift_Ptr = create_lift ("Ptr", "p");
    _Lift_Ptr = create_lift ("Ref", "r");
    _Lift_Str = create_lift ("Str", "s");
    _Lift_Record = create_lift ("Record", "A");
    _Lift_Array = create_lift ("Array", "B");
    _Lift_List = create_lift ("List", "C");
}
