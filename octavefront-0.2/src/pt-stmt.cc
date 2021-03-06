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

#if defined (__GNUG__)
#pragma implementation
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SLList.h>

#include "defun.h"
#include "error.h"
#include "ov.h"
#include "oct-lvalue.h"
#include "input.h"
#include "pager.h"
#include "pt-bp.h"
#include "pt-cmd.h"
#include "pt-id.h"
#include "pt-idx.h"
#include "pt-jump.h"
#include "pt-pr-code.h"
#include "pt-stmt.h"
#include "pt-walk.h"
#include "unwind-prot.h"
#include "utils.h"
#include "variables.h"

// If TRUE, turn off printing of results in functions (as if a
// semicolon has been appended to each statement).
static bool Vsilent_functions;

// Pointer to the current statement being executed.
tree_statement *curr_statement = 0;

// A list of commands to be executed.

tree_statement::~tree_statement (void)
{
  delete cmd;
  delete expr;
  delete comm;
}

int
tree_statement::line (void)
{
  return cmd ? cmd->line () : (expr ? expr->line () : -1);
}

int
tree_statement::column (void)
{
  return cmd ? cmd->column () : (expr ? expr->column () : -1);
}

void
tree_statement::maybe_echo_code (bool in_function_body)
{
  if (in_function_body
      && (Vecho_executing_commands & ECHO_FUNCTIONS))
    {
      tree_print_code tpc (octave_stdout, Vps4);

      accept (tpc);
    }
}

octave_value_list
tree_statement::eval (bool silent, int nargout, bool in_function_body)
{
  octave_value_list retval;

  bool pf = silent ? false : print_flag;

  if (cmd || expr)
    {
      unwind_protect_ptr (curr_statement);
      curr_statement = this;

      maybe_echo_code (in_function_body);

      if (cmd)
	cmd->eval ();
      else
	{
	  expr->set_print_flag (pf);

	  // XXX FIXME XXX -- maybe all of this should be packaged in
	  // one virtual function that returns a flag saying whether
	  // or not the expression will take care of binding ans and
	  // printing the result.

	  bool do_bind_ans = false;

	  if (expr->is_identifier ())
	    {
	      bool script_file_executed = false;

	      tree_identifier *id = static_cast<tree_identifier *> (expr);

	      id->do_lookup (script_file_executed, false);

	      do_bind_ans = id->is_function ();
	    }
	  else
	    do_bind_ans = (! (expr->is_indirect_ref ()
			      || expr->is_assignment_expression ()));

	  retval = expr->rvalue (nargout);

	  if (do_bind_ans && ! (error_state || retval.empty ()))
	    bind_ans (retval(0), pf);
	}

      unwind_protect::run ();
    }

  return retval;
}

void
tree_statement::accept (tree_walker& tw)
{
  tw.visit_statement (*this);
}

octave_value_list
tree_statement_list::eval (bool silent, int nargout)
{
  octave_value_list retval;

  if (error_state)
    return retval;

  for (Pix p = first (); p != 0; next (p))
    {
      tree_statement *elt = this->operator () (p);

      if (elt)
	{
	  bool silent_flag =
	    silent ? true : (function_body ? Vsilent_functions : false);

	  retval = elt->eval (silent_flag, nargout, function_body);

	  if (error_state)
	    break;

	  if (tree_break_command::breaking
	      || tree_continue_command::continuing)
	    break;

	  if (tree_return_command::returning)
	    break;
	}
      else
	error ("invalid statement found in statement list!");


    }

  return retval;
}

int
tree_statement_list::set_breakpoint (int line)
{
  tree_breakpoint tbp (line, tree_breakpoint::set);
  accept (tbp);
  
  return tbp.get_line ();
}

void
tree_statement_list::delete_breakpoint (int line)
{
  tree_breakpoint tbp (line, tree_breakpoint::clear); 
  accept(tbp);
}

octave_value_list
tree_statement_list::list_breakpoints (void)
{
  tree_breakpoint tbp (0, tree_breakpoint::list);
  accept (tbp);

  return tbp.get_list ();
}

void
tree_statement_list::accept (tree_walker& tw)
{
  tw.visit_statement_list (*this);
}

static int
silent_functions (void)
{
  Vsilent_functions = check_preference ("silent_functions");

  return 0;
}

void
symbols_of_pt_stmt (void)
{
  DEFVAR (silent_functions, 0.0, silent_functions,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} silent_functions\n\
If the value of @code{silent_functions} is nonzero, internal output\n\
from a function is suppressed.  Otherwise, the results of expressions\n\
within a function body that are not terminated with a semicolon will\n\
have their values printed.  The default value is 0.\n\
\n\
For example, if the function\n\
\n\
@example\n\
function f ()\n\
  2 + 2\n\
endfunction\n\
@end example\n\
\n\
@noindent\n\
is executed, Octave will either print @samp{ans = 4} or nothing\n\
depending on the value of @code{silent_functions}.\n\
@end defvr");
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
