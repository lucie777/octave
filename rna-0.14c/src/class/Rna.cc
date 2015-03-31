/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:48 $
 * $Source: /sys/cvs/rna/src/class/Rna.cc,v $
 * $Revision: 1.3 $
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
}
