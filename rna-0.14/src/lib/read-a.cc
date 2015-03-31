/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/11 08:05:04 $
 * $Source: /sys/cvs/rna/src/lib/read-a.cc,v $
 * $Revision: 1.3 $
 */

#include <ctype.h> // isalnum, isspace, isdigit, isrecord, isxdigit, isprint
#include <stdlib.h> // atol, atof
#include <string.h> // memset

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * AsciiSerializer - Transport                                 *
 *                                                                *
\******************************************************************/

void 
AsciiSerializer::read_whitespace ()
{
    repeat {
	if (isspace (_stream->peek ()))
	    _stream->next ();
	else if (_stream->peek () == ';' || _stream->peek () == '#') { 
	    // comment
	    do
		_stream->next ();
	    while (_stream->peek () != '\n' && _stream->peek () != EOF);
	} else
	    return;
    }
}

Str
AsciiSerializer::read_name ()
{
    RNA_ASSERT (isalpha (_stream->peek ()) || _stream->peek () == '_');

    _parse->start ();
    do {
	_parse->grow1 (_stream->peek ());
	_stream->next ();
    } while (isalnum (_stream->peek ()) 
	     || _stream->peek () == '_' 
	     || _stream->peek () == '-');
    return (Str) _parse->finish0 ();
}

void
AsciiSerializer::check_type (Type & expected, Type found)
{
    // check type
    if (expected == ANY) {
	expected = found;
    } else {
	if (expected != found)
	    _stream->error ("Expected type \'%c\'; found type \'%c\'!", 
			    expected, found);
    }
}

Context::Binding *
AsciiSerializer::lookup (Rna name, Type & type)
{
    Context::Binding * result = _context->lookup (name);

    if (result->type == ANY) {
	result->type = type;
    } else if (type == ANY) {
	type = result->type;
    } else {
	if (result->type != type)
	    _stream->error ("Types \'%c\' and \'%c\' do not match!", 
			    result->type, type);
    }

    return result;
}

Bool 
AsciiSerializer::read_item1 (Word * result, Type type,
			    Record * container, Bool must_define)
{
    RNA_ASSERT (type != ANY);
    return read_item (result, type, container, must_define);
}

Bool 
AsciiSerializer::read_item (Word * result, Type & type,
			   Record * container, Bool must_define)
{
    int i = 0;
    Bool positional = true;
    Memory::Marker marker = _parse->mark ();

    read_whitespace ();
    if (isalpha (_stream->peek ()) 
	|| _stream->peek () == '_') { // name
	Str name = read_name ();
	if (_stream->peek () == ':') { // label
	    _stream->next ();
	    Context::Binding * binding = lookup (name, type);
	    if (binding->resolved)
		_stream->error ("Label %s already defined!", name);
	    read_item (result, type, container, false);
	    _context->resolve (binding, *result, type);
	} else if (_stream->peek () == '=') { // component name
	    _stream->next ();
	    positional = false;
	    if (container == 0)
		_stream->error ("Component assignment not allowed here!");
	    Index xx = container->_typeinfo->lookup (name);
	    if (xx == INDEX_NOT_FOUND) { // Attribute
		List * attr = (List *) _memory->alloc (sizeof (List));
# ifdef RNA_TYPECHECK
		attr->_car._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
		attr->_car._word.ii = 0x42424242;
# endif /* RNA_DEBUG */
		attr->_tag = (Str) _memory->alloc0 (name);
		attr->_car = Rna::_Null;
		attr->_type = ANY;
		read_item (& attr->_car._word, attr->_type, 0, false);
# ifdef RNA_TYPECHECK
		attr->_car._type = attr->_type;
# endif /* RNA_TYPECHECK */
		attr->_cdr = container->_attr;
		if (strcmp (name, RNA_POSITION_TAG) != 0)
		    container->_attr = attr;
	    } else
		read_item1 (& container->_item [xx], 
			    container->_typeinfo->type (xx), 
			    0, false);
	} else { // reference
	    Context::Binding * binding = lookup (name, type);
	    if (must_define && !binding->resolved)
		_stream->error ("Undefined reference: %s", name);
	    _context->bind (binding, result, &type);
	}
    } else if (isdigit (_stream->peek ()) 
	       || _stream->peek () == '-' 
	       || _stream->peek () == '+') { // number
	Bool is_hex = false; Bool is_float = false;
	_parse->start ();
	if (_stream->peek () == '+' || _stream->peek () == '-') {
	    _parse->grow1 (_stream->peek ()); _stream->next (); }
	if (_stream->peek () == '0') {
	    _parse->grow1 (_stream->peek ()); _stream->next ();
	    if (_stream->peek () == 'X' || _stream->peek () == 'x') {
		_stream->next ();
		is_hex = true;
		while (isxdigit (_stream->peek ())) {
		    _parse->grow1 (_stream->peek ()); 
		    _stream->next ();
		}
	    }
	}
	while (isdigit (_stream->peek ())) {
	    _parse->grow1 (_stream->peek ()); 
	    _stream->next ();
	}
	if (_stream->peek () == '.') {
	    if (is_hex)
		_stream->error ("Dot '.' inside hexadecimal number!");
	    _parse->grow1 (_stream->peek ()); _stream->next ();
	    is_float = true;
	    while (isdigit (_stream->peek ())
		   || _stream->peek () == 'E' || _stream->peek () == 'e'
		   || _stream->peek () == '+' || _stream->peek () == '-') {
		_parse->grow1 (_stream->peek ()); 
		_stream->next ();
	    }
	}
	const char * number = (const char *) _parse->finish0 ();
	if (is_float) {
	    check_type (type, FLOAT);
	    result->ff = atof (number);
	} else { // integer
	    check_type (type, INT);
	    result->ii = atol (number);
	}
    } else if (_stream->peek () == '\"') { // string
	_stream->next ();
	check_type (type, STR);
	_memory->start ();
	while (_stream->peek () != '\"') {
	    if (_stream->peek () == EOF)
		_stream->error ("Unterminated string!");
	    char c;
	    if (_stream->peek () != '\\')
		c = _stream->peek ();
	    else {
		_stream->next ();
		switch (_stream->peek ()) {
		case 'a': case 'A': c = '\a'; break;
		case 'b': case 'B': c = '\b'; break;
		case 'f': case 'F': c = '\f'; break;
		case 'n': case 'N': c = '\n'; break;
		case 'r': case 'R': c = '\r'; break;
		case 't': case 'T': c = '\t'; break;
		case 'v': case 'V': c = '\v'; break;
		case 'x': 
		    _stream->next ();
		    c = 0;
		    do {
			c *= 16;
			switch (_stream->peek ()) {
			case '0' ... '9': c += _stream->peek () - '0'; break;
			case 'A' ... 'F': c += _stream->peek () - 'A' + 10; break;
			case 'a' ... 'f': c += _stream->peek () - 'a' + 10; break;
			default: _stream->error ("Invalid hex digit \'%c\'!", 
						 _stream->peek ()); break;
			}
			_stream->next ();
		    } while (isxdigit (_stream->peek ()));
		    break;
		case '0' ... '9': 
		    _stream->next ();
		    c = 0;
		    do {
			c *= 8;
			c += _stream->peek () - '0';
			_stream->next ();
		    } while (isdigit (_stream->peek ()));
		    break;
		case '\n': continue;
		default: c = _stream->peek ();
		}
	    }
	    _memory->grow1 (c);
	    _stream->next ();
	}
	_stream->next ();
	result->ss = (Str) _memory->finish0 ();
    } else if (_stream->peek () == '(') {
	Position * pos = _stream->getPosition (_memory);
	_stream->next ();
	check_type (type, RECORD);
	read_whitespace ();
	if (_stream->peek () == ')') {
	    _stream->next ();
	    result->record = 0;
	} else {
	    TypeDescriptor * typeinfo;
	    read_item1 ((Word *) & typeinfo, RECORD, 0, true);
	    Size size = typeinfo->_item [TypeDescriptor::IDX_SIZE].ii;
	    result->record = (Record *) _memory->alloc
		(sizeof (Record) + size * sizeof (Word));
#ifdef RNA_DEBUG
	    memset (result->record, 0, 
		    sizeof (Record) + size * sizeof (Word));
#endif /* RNA_DEBUG */
	    result->record->_typeinfo = typeinfo;
	    Str item_type = typeinfo->_item [TypeDescriptor::IDX_FORMAT].ss;
	    repeat {
		read_whitespace ();
		if (_stream->peek () == ')')
		    break;
		if (read_item1 (& result->record->_item [i], 
				(Type) item_type [i], 
				result->record, false))
		    i++;
		if (i > typeinfo->_item [TypeDescriptor::IDX_SIZE].ii)
		    _stream->error ("Too many items inside Record %s!", 
				    typeinfo->_item [TypeDescriptor::IDX_NAME].ss);
	    }
	    _stream->next ();
	    List * attr = (List *) _memory->alloc (sizeof (List));
# ifdef RNA_TYPECHECK
	    attr->_car._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
	    attr->_car._word.ii = 0x42424242;
# endif /* RNA_DEBUG */
	    attr->_tag = (Str) _memory->alloc0 (RNA_POSITION_TAG);
	    attr->_type = RECORD;
	    attr->_car = pos;
	    attr->_cdr = result->record->_attr;
	    result->record->_attr = attr;
	}
    } else if (_stream->peek () == '[') {
	_stream->next ();
	check_type (type, ARRAY);
	struct Node 
	{
	    Word item;
	    Node * next;
	} * head = 0, * * ref = & head;
	Type item_type = ANY;
	repeat {
	    read_whitespace ();
	    if (_stream->peek () == ']')
		break;
	    Node * curr = (Node *) _parse->alloc (sizeof (Node));
	    curr->next = 0;
	    *ref = curr;
	    ref = & (*ref)->next;
	    read_item (& curr->item, item_type, 0, false);
	    i++;
	}
	_stream->next ();
	if (i > 0) {
	    if (item_type == ANY) {
#if 0
		_stream->error ("Undefined item type!");
#else
		item_type = RECORD;
#endif
	    }
	    result->array = (Array *) _memory->alloc 
		(sizeof (Array) + i * sizeof (Rna));
	    result->array->_length = i;
	    result->array->_type = item_type;
	    Node * curr;
	    for (curr = head, i = 0; curr; curr = curr->next, i++)
		_context->relocate (& curr->item, & result->array->_item [i]);
	} else
	    result->array = 0;
    } else { // Error
	if (_stream->peek () == EOF)
	    if (type == ANY) {
		* result = Rna::_Null._word;
		return positional;
	    } else
		_stream->error ("Unexpected end of file!");
	else if (isprint (_stream->peek ()))
	    _stream->error ("Unexpected character \'%c\'!", 
			    _stream->peek ());
	else
	    _stream->error ("Unexpected character \'\\x%02x\'!", 
			    _stream->peek ());
    }
  
    _parse->rewind (marker);
    return positional;
}

Rna 
AsciiSerializer::read (Type type)
{
    Rna result = Rna::_Null;

    read_item (& result._word, type, 0, true);

# ifdef RNA_TYPECHECK
    result._type = type;
# endif /* RNA_TYPECHECK */

    return result;
}
