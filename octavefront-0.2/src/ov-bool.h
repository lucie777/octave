/*

Copyright (C) 1996, 1997 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#if !defined (octave_bool_h)
#define octave_bool_h 1

#if defined (__GNUG__)
#pragma interface
#endif

#include <cstdlib>

#include <iostream>
#include <string>

#include "lo-utils.h"
#include "mx-base.h"
#include "oct-alloc.h"
#include "str-vec.h"

#include "ov-base.h"
#include "ov-base-scalar.h"
#include "ov-typeinfo.h"

class Octave_map;
class octave_value_list;

class tree_walker;

// Real scalar values.

class
octave_bool : public octave_base_scalar<bool>
{
public:

  octave_bool (void)
    : octave_base_scalar<bool> (false) { }

  octave_bool (bool b)
    : octave_base_scalar<bool> (b) { }

  octave_bool (const octave_bool& s)
    : octave_base_scalar<bool> (s) { }

  ~octave_bool (void) { }

  octave_value *clone (void) { return new octave_bool (*this); }

  type_conv_fcn numeric_conversion_function (void) const;

  octave_value do_index_op (const octave_value_list& idx);

  idx_vector index_vector (void) const { return idx_vector (scalar); }

  bool is_real_scalar (void) const { return true; }

  bool is_bool_type (void) const { return true; }

  bool is_real_type (void) const { return true; }

  bool valid_as_scalar_index (void) const { return scalar; }

  bool valid_as_zero_index (void) const { return ! scalar; }

  bool is_true (void) const { return scalar; }

  double double_value (bool = false) const { return scalar; }

  double scalar_value (bool = false) const { return scalar; }

  Matrix matrix_value (bool = false) const
    { return Matrix (1, 1, scalar); }

  Complex complex_value (bool = false) const { return scalar; }

  ComplexMatrix complex_matrix_value (bool = false) const
    { return  ComplexMatrix (1, 1, Complex (scalar)); }

  bool bool_value (void) const { return scalar; }

  boolMatrix bool_matrix_value (void) const
    { return boolMatrix (1, 1, scalar); }

  octave_value convert_to_str (void) const;

private:

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA

  DECLARE_OCTAVE_ALLOCATOR
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
