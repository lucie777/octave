/*

Copyright (C) 2000 John W. Eaton

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined (USE_READLINE)

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "oct-rl-edit.h"

int
octave_rl_screen_height (void)
{
  int rows, cols;
  rl_get_screen_size (&rows, &cols);
  return rows;
}

int
octave_rl_screen_width (void)
{
  int rows, cols;
  rl_get_screen_size (&rows, &cols);
  return cols;
}

void
octave_rl_enable_paren_matching (int val)
{
  rl_variable_bind ("blink-matching-paren", val ? "1" : "0");
}

void
octave_rl_clear_screen (void)
{
  extern void _rl_clear_screen ();

  _rl_clear_screen ();
}

void
octave_rl_resize_terminal (void)
{
  rl_resize_terminal ();
}

void
octave_rl_restore_terminal_state ()
{
  if (rl_deprep_term_function)
    rl_deprep_term_function ();
}

void
octave_rl_insert_text (const char *s)
{
  rl_insert_text (s);
}

void
octave_rl_newline (void)
{
  rl_newline (1, '\n');
}

void
octave_rl_clear_undo_list (void)
{
  if (rl_undo_list)
    {
      rl_free_undo_list ();

      rl_undo_list = 0;
    }
}

void
octave_rl_set_name (const char *n)
{
  static char *nm = 0;

  if (nm)
    {
      free (nm);
      nm = 0;
    }

  nm = malloc (strlen (n + 1));

  strcpy (nm, n);

  rl_readline_name = nm;

  /* Since we've already called rl_initialize, we need to re-read the
     init file to take advantage of the conditional parsing feature
     based on rl_readline_name; */

  rl_re_read_init_file (0, 0);
}

char *
octave_rl_readline (const char *prompt)
{
  return readline (prompt);
}

void
octave_rl_set_input_stream (FILE *f)
{
  rl_instream = f;
}

FILE *
octave_rl_get_input_stream (void)
{
  return rl_instream;
}

void
octave_rl_set_output_stream (FILE *f)
{
  rl_outstream = f;
}

FILE *
octave_rl_get_output_stream (void)
{
  return rl_outstream;
}

void
octave_rl_read_init_file (const char *f)
{
  if (f && *f)
    rl_read_init_file (f);
  else
    rl_re_read_init_file (0, 0);
}

void
octave_rl_set_basic_quote_characters (const char *s)
{
  static char *ss = 0;

  if (ss)
    {
      free (ss);
      ss = 0;
    }

  ss = malloc (strlen (s) + 1);

  strcpy (ss, s);

  rl_basic_quote_characters = ss;
}

void
octave_rl_set_completion_append_character (char c)
{
  rl_completion_append_character = c;
}

void
octave_rl_set_completion_function (rl_attempted_completion_fcn_ptr f)
{
  rl_attempted_completion_function = f;
}

void
octave_rl_set_startup_hook (rl_startup_hook_fcn_ptr f)
{
  void **fp = (void **) (&rl_startup_hook);
  *fp = (void *) f;
}

rl_startup_hook_fcn_ptr
octave_rl_get_startup_hook (void)
{
  return (rl_startup_hook_fcn_ptr) rl_startup_hook;
}

void
octave_rl_set_event_hook (rl_event_hook_fcn_ptr f)
{
  void **fp = (void **) (&rl_event_hook);
  *fp = (void *) f;
}

rl_event_hook_fcn_ptr
octave_rl_get_event_hook (void)
{
  return (rl_event_hook_fcn_ptr) rl_event_hook;
}

char **
octave_rl_completion_matches (const char *text, rl_completer_fcn_ptr f)
{
  return rl_completion_matches (text, f);
}

char
octave_rl_prompt_start_ignore (void)
{
  return RL_PROMPT_START_IGNORE;
}

char
octave_rl_prompt_end_ignore (void)
{
  return RL_PROMPT_END_IGNORE;
}

void
octave_rl_add_defun (const char *name, rl_fcn_ptr f, char key)
{
  rl_add_defun (name, f, key);
}

void
octave_rl_set_terminal_name (const char *term)
{
  rl_terminal_name = (char *) term;
}

void
octave_rl_initialize (void)
{
  rl_initialize ();
}

int
octave_rl_history_search_forward (int count, int ignore)
{
  return rl_history_search_forward (count, ignore);
}

int
octave_rl_history_search_backward (int count, int ignore)
{
  return rl_history_search_backward (count, ignore);
}

char
octave_rl_ctrl (char c)
{
  return CTRL (c);
}

char
octave_rl_meta (char c)
{
  return META (c);
}

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
