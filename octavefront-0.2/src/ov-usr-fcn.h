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

#if !defined (octave_user_function_h)
#define octave_user_function_h 1

#if defined (__GNUG__)
#pragma interface
#endif

#include <ctime>

#include <string>

#include "comment-list.h"
#include "oct-obj.h"
#include "ov-fcn.h"
#include "ov-typeinfo.h"

class string_vector;

class octave_value;
class tree_parameter_list;
class tree_statement_list;
class tree_va_return_list;
class tree_walker;
class symbol_table;
class symbol_record;

// Builtin functions.

class
octave_user_function : public octave_function
{
public:

  octave_user_function (tree_parameter_list *pl = 0,
			tree_parameter_list *rl = 0,
			tree_statement_list *cl = 0,
			symbol_table *st = 0);

  ~octave_user_function (void);

  octave_function *function_value (bool) { return this; }

  octave_user_function *define_param_list (tree_parameter_list *t);

  octave_user_function *define_ret_list (tree_parameter_list *t);

  void stash_fcn_file_name (void);

  void stash_leading_comment (octave_comment_list *lc) { lead_comm = lc; }

  void stash_trailing_comment (octave_comment_list *tc) { trail_comm = tc; }

  void mark_fcn_file_up_to_date (const octave_time& t) { t_checked = t; }

  void stash_fcn_file_time (const octave_time& t)
    {
      t_parsed = t;
      mark_fcn_file_up_to_date (t);
    }

  void stash_symtab_ptr (symbol_record *sr)
    { symtab_entry = sr; }

  std::string fcn_file_name (void) const
    { return file_name; }

  octave_time time_parsed (void) const
    { return t_parsed; }

  octave_time time_checked (void) const
    { return t_checked; }

  void mark_as_system_fcn_file (void);

  bool is_system_fcn_file (void) const
    { return system_fcn_file; }

  bool takes_varargs (void) const;

  void octave_va_start (void)
    { curr_va_arg_number = num_named_args; }

  octave_value octave_va_arg (void);

  octave_value_list octave_all_va_args (void);

  bool takes_var_return (void) const;

  void octave_vr_val (const octave_value& val);

  void stash_function_name (const std::string& s);

  std::string function_name (void)
    { return fcn_name; }

  void clear_args_passed (void)
    { args_passed = octave_value_list (); }

  octave_value_list
  do_multi_index_op (int nargout, const octave_value_list& args);

  void traceback_error (void);

  tree_parameter_list *parameter_list (void) { return param_list; }

  tree_parameter_list *return_list (void) { return ret_list; }

  tree_statement_list *body (void) { return cmd_list; }

  octave_comment_list *leading_comment (void) { return lead_comm; }

  octave_comment_list *trailing_comment (void) { return trail_comm; }

  void accept (tree_walker& tw);

private:

  octave_user_function (void);

  octave_user_function (const octave_user_function& m);

  // List of arguments for this function.  These are local variables.
  tree_parameter_list *param_list;

  // List of parameters we return.  These are also local variables in
  // this function.
  tree_parameter_list *ret_list;

  // The list of commands that make up the body of this function.
  tree_statement_list *cmd_list;

  // The local symbol table for this function.
  symbol_table *sym_tab;

  // The comments preceding the FUNCTION token.
  octave_comment_list *lead_comm;

  // The comments preceding the ENDFUNCTION token.
  octave_comment_list *trail_comm;

  // The name of the file we parsed
  std::string file_name;

  // The name of the function.
  std::string fcn_name;

  // The time the file was parsed.
  octave_time t_parsed;

  // The time the file was last checked to see if it needs to be
  // parsed again.
  octave_time t_checked;

  // True if this function came from a file that is considered to be a
  // system function.  This affects whether we check the time stamp
  // on the file to see if it has changed.
  bool system_fcn_file;

  // Used to keep track of recursion depth.
  int call_depth;

  // The number of arguments that have names.
  int num_named_args;

  // The values that were passed as arguments.
  octave_value_list args_passed;

  // The number of arguments passed in.
  int num_args_passed;

  // Used to keep track of the current offset into the list of va_args.
  int curr_va_arg_number;

  // The list of return values when an unspecified number can be
  // returned.
  tree_va_return_list *vr_list;

  // The symbol record for this function.
  symbol_record *symtab_entry;

  // The symbol record for argn in the local symbol table.
  symbol_record *argn_sr;

  // The symbol record for nargin in the local symbol table.
  symbol_record *nargin_sr;

  // The symbol record for nargout in the local symbol table.
  symbol_record *nargout_sr;

  void print_code_function_header (void);

  void print_code_function_trailer (void);

  void install_automatic_vars (void);

  void bind_automatic_vars (const string_vector& arg_names, int nargin,
			    int nargout);

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA

  DECLARE_OCTAVE_ALLOCATOR

  friend class compiler_air;
  friend class tree_dump;
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
