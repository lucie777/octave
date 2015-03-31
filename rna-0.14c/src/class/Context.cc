/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/08/21 11:10:21 $
 * $Source: /sys/cvs/rna/src/class/Context.cc,v $
 * $Revision: 1.3 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

Context::Context ()
    : _bindings (0),
      _bindingspace (new Memory ()),
      _parent (StrContext::_System),
      _patchspace (new Memory ())
{}

Context::Context (Context * parent)
    : _bindings (0),
      _bindingspace (new Memory ()),
      _parent (parent),
      _patchspace (new Memory ())
{}

Context::~Context ()
{
    if (_patchspace)
	delete _patchspace;

    delete _bindingspace;
}


/******************************************************************\
 *                                                                *
 * Handle Management                                              *
 *                                                                *
\******************************************************************/

Type
Context::handle_type () const
{
    return ANY;
}

Bool 
Context::compare_handle (Rna handle1, Rna handle2) const
{
    return handle1 == handle2;
}

Rna 
Context::copy_handle (Rna handle) const
{
    return handle;
}


/******************************************************************\
 *                                                                *
 * Dictionary                                                     *
 *                                                                *
\******************************************************************/

Bool
Context::includes (Rna handle)
{
    for (Binding * result = _bindings; result; result = result->next)
	if (compare_handle (result->handle, handle))
	    return true;
    return false;
}

void
Context::enter (Rna handle, Rna definition, Type type)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (handle._type == STR && definition._type == type);
# endif /* RNA_TYPECHECK */
    
    Binding * binding = (Binding *) 
	_bindingspace->alloc (sizeof (Binding));
# ifdef RNA_TYPECHECK
    binding->handle._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
    binding->handle._word.ii = 0x42424242;
# endif /* RNA_DEBUG */

    binding->handle = copy_handle (handle);
    binding->resolved = true;
    binding->definition = definition._word;
    binding->type = type;
    binding->patch = 0;
    binding->depth_nesting = -1;
    binding->side_nesting = -1;
    binding->ref_count = 0;
    binding->next = _bindings;
    _bindings = binding;
}

void
Context::remove (Rna handle)
{
    for (Binding * * ref = & _bindings; *ref; ref = & (*ref)->next)
	if (compare_handle ((*ref)->handle, handle)) {
	    (*ref) = (*ref)->next;
	    // @@@ memory leak
	    break;
	}
}


/******************************************************************\
 *                                                                *
 * Symbol Table                                                   *
 *                                                                *
\******************************************************************/

Context::Binding *
Context::lookup (Rna handle, Bool allocate)
{
    Binding * result;

    for (result = _bindings; result; result = result->next)
	if (compare_handle (result->handle, handle))
	    return result;

    RNA_ASSERT (result == 0);

    if (_parent) {
	result = _parent->lookup (handle, false);

	if (result && allocate)
	    if (!result->resolved) {
		Binding * new_result = (Binding *) 
		    _bindingspace->alloc (sizeof (Binding));
# ifdef RNA_TYPECHECK
		new_result->handle._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
		new_result->handle._word.ii = 0x42424242;
# endif /* RNA_DEBUG */
		* new_result = * result;
		
		new_result->next = _bindings;
		_bindings = new_result;

		return new_result;
	    }
    }
    
    if (result == 0 && allocate) {    
	result = (Binding *) _bindingspace->alloc (sizeof (Binding));
# ifdef RNA_TYPECHECK
	result->handle._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
	result->handle._word.ii = 0x42424242;
# endif /* RNA_DEBUG */

	result->handle = copy_handle (handle);
	result->resolved = false;
	result->definition = Rna::_Null._word;
	result->type = ANY;
	result->patch = 0;
	result->depth_nesting = -1;
	result->side_nesting = -1;
	result->ref_count = 0;
	result->next = _bindings;

	_bindings = result;
    }

    return result;
}

Context::Binding *
Context::reverse_lookup (Rna definition, Type type, Bool allocate)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (definition._type == type 
		|| definition._type == ANY || type == ANY);
# endif /* RNA_TYPECHECK */

    Binding * result;

    for (result = _bindings; result; result = result->next)
	if (result->definition == definition._word)
	    break;

    if (result == 0 && _parent) {
	result = _parent->reverse_lookup (definition, type, false);

	if (result && allocate)
	    if (!result->resolved) {
		Binding * new_result = (Binding *) 
		    _bindingspace->alloc (sizeof (Binding));
# ifdef RNA_TYPECHECK
		new_result->handle._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
		new_result->handle._word.ii = 0x42424242;
# endif /* RNA_DEBUG */
		* new_result = * result;
		
		new_result->next = _bindings;
		_bindings = new_result;

		return new_result;
	    }
    }

    if (result == 0 && allocate) {
	result = (Binding *) _bindingspace->alloc (sizeof (Binding));
# ifdef RNA_TYPECHECK
	result->handle._type = ANY;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
	result->handle._word.ii = 0x42424242;
# endif /* RNA_DEBUG */

	result->handle = create_handle (definition._word);
	result->resolved = false;
	result->definition = definition._word;
	result->type = type;
	result->patch = 0;
	result->depth_nesting = -1;
	result->side_nesting = -1;
	result->ref_count = 0;
	result->next = _bindings;

	_bindings = result;
    }

    if (result) {
	if (result->type == ANY)
	    result->type = type;
# ifdef RNA_TYPECHECK
	RNA_ASSERT (type == result->type || type == ANY);
# endif /* RNA_TYPECHECK */
    }

    return result;
}

void
Context::resolve (Binding * binding, Word definition, Type type)
{
    RNA_ASSERT (!binding->resolved);
    RNA_ASSERT (Rna::_Null._word == binding->definition
		|| definition == binding->definition);

    binding->resolved = true;
    binding->definition = definition;

    if (binding->type == ANY)
	binding->type = type;
    else if (type == ANY)
	type = binding->type;
    RNA_ASSERT (binding->type == type);

    if (binding->patch) {
	// catch up deferred patches
	for (Patch * patch = binding->patch; patch; patch = patch->next) {
	    * patch->use = definition;
	    if (patch->type)
		* patch->type = type;
	}
	binding->patch = 0;
    }
}

void
Context::bind (Binding * binding, Word * use, Type * type)
{
    Patch * patch = (Patch *) _patchspace->alloc (sizeof (Patch));
    patch->use = use;
    patch->type = type;
    patch->next = binding->patch;
    binding->patch = patch;

    if (binding->resolved) {
	// patch imediately
	* use = binding->definition;
        if (patch->type)
            * patch->type = *type;
    }
}

void 
Context::relocate (Word * from, Word * to, Type * from_type, Type * to_type)
{
    // relocate item
    * to = * from;

    // adjust patches
    for (Binding * binding = _bindings; binding; binding = binding->next)
	for (Patch * patch = binding->patch; patch; patch = patch->next)
	    if (patch->use == from) {
		patch->use = to;
                if (patch->type == from_type)
                    patch->type = to_type;
            }
}

void 
Context::clear_patches ()
{
    for (Binding * binding = _bindings; binding; binding = binding->next)
	binding->patch = 0;
    
    if (_patchspace)
	delete _patchspace;
    _patchspace = new Memory ();
}


/******************************************************************\
 *                                                                *
 * Context                                                      *
 *                                                                *
\******************************************************************/

Context *
Context::getParent ()
{
    return _parent;
}

Rna
Context::operator [] (Rna handle)
{
    Binding * binding = lookup (handle);
    
    RNA_ASSERT (binding->resolved);
    
# ifndef RNA_TYPECHECK
    return Rna (binding->definition);
# else /* RNA_TYPECHECK */
    return Rna (binding->definition, binding->type);
# endif /* RNA_TYPECHECK */
}

Rna 
Context::findTypeDescriptorByName (constStr name)
{
    for (Binding * binding = _bindings; binding; binding = binding->next)
	if (binding->resolved && binding->type == RECORD
	    && binding->definition != Rna::_Null._word) {
	    Word word = binding->definition;
	    if (word.record->_typeinfo == TypeDescriptor::_TypeInfo)
		if (strcmp(word.record->_item [TypeDescriptor::IDX_NAME].ss,
			   name) == 0) {
# ifndef RNA_TYPECHECK
		    return Rna (word);
# else /* RNA_TYPECHECK */
		    return Rna (word, binding->type);
# endif /* RNA_TYPECHECK */
		}
	}

    if (_parent)
	return _parent->findTypeDescriptorByName (name);
    else
	return Rna::_Null;
}


/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

StrContext * 
Context::strcontext ()
{
    RNA_SHOULD_NOT_REACH_HERE;

    return 0;
}

IntContext * 
Context::intcontext ()
{
    RNA_SHOULD_NOT_REACH_HERE;

    return 0;
}
