
/*  A Bison parser, made from parse.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ADD_EQ	257
#define	SUB_EQ	258
#define	MUL_EQ	259
#define	DIV_EQ	260
#define	LEFTDIV_EQ	261
#define	EMUL_EQ	262
#define	EDIV_EQ	263
#define	ELEFTDIV_EQ	264
#define	AND_EQ	265
#define	OR_EQ	266
#define	LSHIFT_EQ	267
#define	RSHIFT_EQ	268
#define	LSHIFT	269
#define	RSHIFT	270
#define	EXPR_AND_AND	271
#define	EXPR_OR_OR	272
#define	EXPR_AND	273
#define	EXPR_OR	274
#define	EXPR_NOT	275
#define	EXPR_LT	276
#define	EXPR_LE	277
#define	EXPR_EQ	278
#define	EXPR_NE	279
#define	EXPR_GE	280
#define	EXPR_GT	281
#define	LEFTDIV	282
#define	EMUL	283
#define	EDIV	284
#define	ELEFTDIV	285
#define	EPLUS	286
#define	EMINUS	287
#define	QUOTE	288
#define	TRANSPOSE	289
#define	PLUS_PLUS	290
#define	MINUS_MINUS	291
#define	POW	292
#define	EPOW	293
#define	NUM	294
#define	IMAG_NUM	295
#define	STRUCT_ELT	296
#define	NAME	297
#define	END	298
#define	PLOT	299
#define	TEXT	300
#define	STYLE	301
#define	AXES_TAG	302
#define	FOR	303
#define	WHILE	304
#define	DO	305
#define	UNTIL	306
#define	IF	307
#define	ELSEIF	308
#define	ELSE	309
#define	SWITCH	310
#define	CASE	311
#define	OTHERWISE	312
#define	BREAK	313
#define	CONTINUE	314
#define	FUNC_RET	315
#define	UNWIND	316
#define	CLEANUP	317
#define	TRY	318
#define	CATCH	319
#define	GLOBAL	320
#define	STATIC	321
#define	END_OF_INPUT	322
#define	LEXICAL_ERROR	323
#define	FCN	324
#define	ELLIPSIS	325
#define	ALL_VA_ARGS	326
#define	USING	327
#define	TITLE	328
#define	WITH	329
#define	AXES	330
#define	COLON	331
#define	OPEN_BRACE	332
#define	CLOSE_BRACE	333
#define	CLEAR	334
#define	UNARY	335

#line 27 "parse.y"

#define YYDEBUG 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <cstdio>

#ifdef YYBYACC
#include <cstdlib>
#endif

#include <strstream.h>

#include "Matrix.h"
#include "cmd-edit.h"
#include "cmd-hist.h"
#include "file-ops.h"
#include "file-stat.h"
#include "oct-time.h"

#include "comment-list.h"
#include "defun.h"
#include "dynamic-ld.h"
#include "error.h"
#include "input.h"
#include "lex.h"
#include "oct-hist.h"
#include "ov-usr-fcn.h"
#include "toplev.h"
#include "pager.h"
#include "parse.h"
#include "pt-all.h"
#include "symtab.h"
#include "token.h"
#include "unwind-prot.h"
#include "utils.h"
#include "variables.h"

// TRUE means we print 
static bool Vdefault_eval_print_flag = true;

// If TRUE, generate a warning for the assignment in things like
//
//   octave> if (a = 2 < n)
//
// but not
//
//   octave> if ((a = 2) < n)
//
static bool Vwarn_assign_as_truth_value;

// If TRUE, generate a warning for variable swich labels.
static bool Vwarn_variable_switch_label;

// If TRUE, generate warning if declared function name disagrees with
// the name of the file in which it is defined.
static bool Vwarn_function_name_clash;

// TRUE means warn about function files that have time stamps in the future.
bool Vwarn_future_time_stamp;

// If TRUE, generate warning if a statement in a function is not
// terminated with a semicolon.  Useful for checking functions that
// should only produce output using explicit printing statements.
static bool Vwarn_missing_semicolon;

// Temporary symbol table pointer used to cope with bogus function syntax.
symbol_table *tmp_local_sym_tab = 0;

// The current input line number.
int input_line_number = 0;

// The column of the current token.
int current_input_column = 1;

// Buffer for help text snagged from function files.
std::string help_buf;

// Buffer for comments appearing before a function statement.
static std::string fcn_comment_header;

// TRUE means we are using readline.
// (--no-line-editing)
bool line_editing = true;

// TRUE means we printed messages about reading startup files.
bool reading_startup_message_printed = false;

// TRUE means input is coming from startup file.
bool input_from_startup_file = false;

// TRUE means that input is coming from a file that was named on
// the command line.
bool input_from_command_line_file = true;

// TRUE means that we are in the process of evaluating a function
// body.  The parser might be called in that case if we are looking at
// an eval() statement.
bool evaluating_function_body = false;

// Forward declarations for some functions defined at the bottom of
// the file.

// Generic error messages.
static void
yyerror (const char *s);

// Error mesages for mismatched end tokens.
static void
end_error (const char *type, token::end_tok_type ettype, int l, int c);

// Check to see that end tokens are properly matched.
static bool
end_token_ok (token *tok, token::end_tok_type expected);

// Maybe print a warning if an assignment expression is used as the
// test in a logical expression.
static void
maybe_warn_assign_as_truth_value (tree_expression *expr);

// Maybe print a warning about switch labels that aren't constants.
static void
maybe_warn_variable_switch_label (tree_expression *expr);

// Create a plot command.
static tree_plot_command *
make_plot_command (token *tok, plot_limits *range, subplot_list *list);

// Finish building a range.
static tree_expression *
finish_colon_expression (tree_colon_expression *e);

// Build a constant.
static tree_constant *
make_constant (int op, token *tok_val);

// Build a binary expression.
static tree_expression *
make_binary_op (int op, tree_expression *op1, token *tok_val,
		tree_expression *op2);

// Build a boolean expression.
static tree_expression *
make_boolean_op (int op, tree_expression *op1, token *tok_val,
		 tree_expression *op2);

// Build a prefix expression.
static tree_expression *
make_prefix_op (int op, tree_expression *op1, token *tok_val);

// Build a postfix expression.
static tree_expression *
make_postfix_op (int op, tree_expression *op1, token *tok_val);

// Build an unwind-protect command.
static tree_command *
make_unwind_command (token *unwind_tok, tree_statement_list *body,
		     tree_statement_list *cleanup, token *end_tok,
		     octave_comment_list *lc, octave_comment_list *mc);

// Build a try-catch command.
static tree_command *
make_try_command (token *try_tok, tree_statement_list *body,
		  tree_statement_list *cleanup, token *end_tok,
		  octave_comment_list *lc, octave_comment_list *mc);

// Build a while command.
static tree_command *
make_while_command (token *while_tok, tree_expression *expr,
		    tree_statement_list *body, token *end_tok,
		    octave_comment_list *lc);

// Build a do-until command.
static tree_command *
make_do_until_command (token *do_tok, tree_statement_list *body,
		       tree_expression *expr, octave_comment_list *lc);

// Build a for command.
static tree_command *
make_for_command (token *for_tok, tree_argument_list *lhs,
		  tree_expression *expr, tree_statement_list *body,
		  token *end_tok, octave_comment_list *lc);

// Build a break command.
static tree_command *
make_break_command (token *break_tok);

// Build a continue command.
static tree_command *
make_continue_command (token *continue_tok);

// Build a return command.
static tree_command *
make_return_command (token *return_tok);

// Start an if command.
static tree_if_command_list *
start_if_command (tree_expression *expr, tree_statement_list *list);

// Finish an if command.
static tree_if_command *
finish_if_command (token *if_tok, tree_if_command_list *list,
		   token *end_tok, octave_comment_list *lc);

// Build an elseif clause.
static tree_if_clause *
make_elseif_clause (tree_expression *expr, tree_statement_list *list,
		    octave_comment_list *lc);

// Finish a switch command.
static tree_switch_command *
finish_switch_command (token *switch_tok, tree_expression *expr,
		       tree_switch_case_list *list, token *end_tok,
		       octave_comment_list *lc);

// Build a switch case.
static tree_switch_case *
make_switch_case (tree_expression *expr, tree_statement_list *list,
		  octave_comment_list *lc);

// Build an assignment to a variable.
static tree_expression *
make_assign_op (int op, tree_argument_list *lhs, token *eq_tok,
		tree_expression *rhs);

// Begin defining a function.
static octave_user_function *
start_function (tree_parameter_list *param_list, tree_statement_list *body);

// Do most of the work for defining a function.
static octave_user_function *
frob_function (tree_identifier *id, octave_user_function *fcn);

// Finish defining a function.
static octave_user_function *
finish_function (tree_identifier *id, octave_user_function *fcn,
		 octave_comment_list *lc);

// Finish defining a function a different way.
static octave_user_function *
finish_function (tree_parameter_list *ret_list,
		 octave_user_function *fcn, octave_comment_list *lc);

// Reset state after parsing function.
static void
recover_from_parsing_function (void);

// Make an index expression.
static tree_index_expression *
make_index_expression (tree_expression *expr, tree_argument_list *args);

// Make an indirect reference expression.
static tree_indirect_ref *
make_indirect_ref (tree_expression *expr, const std::string&);

// Make a declaration command.
static tree_decl_command *
make_decl_command (int tok, token *tok_val, tree_decl_init_list *lst);

// Finish building a matrix list.
static tree_expression *
finish_matrix (tree_matrix *m);

// Finish building a cell list.
static tree_expression *
finish_cell (tree_cell *c);

// Maybe print a warning.  Duh.
static void
maybe_warn_missing_semi (tree_statement_list *);

// Set the print flag for a statement based on the separator type.
static void
set_stmt_print_flag (tree_statement_list *, char, bool);

#define ABORT_PARSE \
  do \
    { \
      global_command = 0; \
      yyerrok; \
      if (interactive || forced_interactive) \
	YYACCEPT; \
      else \
	YYABORT; \
    } \
  while (0)


#line 321 "parse.y"
typedef union
{
  // The type of the basic tokens returned by the lexer.
  token *tok_val;

  // Comment strings that we need to deal with mid-rule.
  octave_comment_list *comment_type;

  // Types for the nonterminals we generate.
  char sep_type;
  tree *tree_type;
  tree_matrix *tree_matrix_type;
  tree_cell *tree_cell_type;
  tree_expression *tree_expression_type;
  tree_constant *tree_constant_type;
  tree_identifier *tree_identifier_type;
  tree_index_expression *tree_index_expression_type;
  tree_colon_expression *tree_colon_expression_type;
  tree_argument_list *tree_argument_list_type;
  tree_parameter_list *tree_parameter_list_type;
  tree_command *tree_command_type;
  tree_if_command *tree_if_command_type;
  tree_if_clause *tree_if_clause_type;
  tree_if_command_list *tree_if_command_list_type;
  tree_switch_command *tree_switch_command_type;
  tree_switch_case *tree_switch_case_type;
  tree_switch_case_list *tree_switch_case_list_type;
  tree_decl_elt *tree_decl_elt_type;
  tree_decl_init_list *tree_decl_init_list_type;
  tree_decl_command *tree_decl_command_type;
  tree_statement *tree_statement_type;
  tree_statement_list *tree_statement_list_type;
  tree_plot_command *tree_plot_command_type;
  subplot *subplot_type;
  subplot_list *subplot_list_type;
  plot_limits *plot_limits_type;
  plot_range *plot_range_type;
  subplot_using *subplot_using_type;
  subplot_style *subplot_style_type;
  subplot_axes *subplot_axes_type;
  octave_user_function *octave_user_function_type;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		398
#define	YYFLAG		-32768
#define	YYNTBASE	98

#define YYTRANSLATE(x) ((unsigned)(x) <= 335 ? yytranslate[x] : 185)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    89,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    91,
    97,     7,     6,    88,     5,    92,     8,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     4,    87,     2,
     3,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    94,     2,    95,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    93,     2,    96,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    90
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     7,     9,    11,    13,    16,    19,    22,
    24,    28,    29,    31,    34,    36,    40,    42,    44,    47,
    49,    51,    53,    55,    56,    59,    63,    68,    70,    73,
    75,    79,    82,    86,    90,    92,    95,    97,   101,   103,
   106,   108,   110,   112,   114,   118,   120,   122,   124,   126,
   130,   134,   138,   139,   141,   145,   150,   155,   158,   161,
   164,   167,   172,   174,   176,   179,   182,   185,   188,   191,
   195,   199,   203,   207,   211,   215,   219,   223,   227,   231,
   235,   239,   241,   243,   247,   249,   253,   257,   261,   265,
   269,   273,   277,   281,   285,   289,   293,   297,   299,   304,
   308,   312,   316,   320,   324,   328,   332,   336,   340,   344,
   348,   352,   356,   359,   361,   364,   366,   368,   370,   372,
   374,   376,   378,   380,   382,   384,   387,   390,   392,   395,
   397,   401,   403,   405,   410,   412,   415,   419,   422,   429,
   434,   441,   443,   446,   448,   451,   458,   463,   470,   477,
   486,   488,   490,   492,   502,   512,   513,   514,   515,   516,
   518,   520,   523,   527,   530,   535,   538,   542,   545,   549,
   553,   556,   560,   564,   570,   572,   576,   579,   583,   586,
   590,   594,   598,   603,   606,   608,   612,   614,   616,   618,
   621,   624,   628,   630,   633,   637,   643,   648,   653,   657,
   660,   662,   666,   668,   671,   673,   675,   677,   679,   682,
   685,   688,   691,   694,   696,   699,   702,   706,   709,   712,
   716,   721,   722,   724,   726,   728,   730,   733,   736,   737,
   739,   741,   743,   745,   748,   751,   754,   755
};

static const short yyrhs[] = {    99,
     0,    74,     0,   100,   180,     0,   180,     0,    89,     0,
   100,     0,   100,    89,     0,   100,    74,     0,   101,   182,
     0,   105,     0,   101,   181,   105,     0,     0,   103,     0,
   104,   184,     0,   105,     0,   104,   183,   105,     0,   130,
     0,   131,     0,    51,    86,     0,    49,     0,    46,     0,
    47,     0,    52,     0,     0,    94,    95,     0,    94,    87,
    95,     0,    94,   108,   110,    95,     0,   111,     0,   111,
    87,     0,   115,     0,   111,    87,   115,     0,    93,    96,
     0,    93,    87,    96,     0,    93,   113,    96,     0,   114,
     0,   114,    87,     0,   115,     0,   114,    87,   115,     0,
   118,     0,   118,    88,     0,   106,     0,   107,     0,   109,
     0,   112,     0,    91,   130,    97,     0,     4,     0,   130,
     0,   117,     0,    78,     0,   118,    88,   117,     0,   118,
    88,   130,     0,   118,    88,    78,     0,     0,   116,     0,
   120,    91,    97,     0,   120,    91,   118,    97,     0,   120,
    93,   118,    96,     0,   120,    42,     0,   120,    43,     0,
   120,    40,     0,   120,    41,     0,   120,    92,   119,    48,
     0,   120,     0,   122,     0,    42,   121,     0,    43,   121,
     0,    27,   121,     0,     6,   121,     0,     5,   121,     0,
   121,    44,   121,     0,   121,    45,   121,     0,   121,     6,
   121,     0,   121,     5,   121,     0,   121,     7,   121,     0,
   121,     8,   121,     0,   121,    38,   121,     0,   121,    39,
   121,     0,   121,    35,   121,     0,   121,    36,   121,     0,
   121,    34,   121,     0,   121,    37,   121,     0,   124,     0,
   121,     0,   124,     4,   121,     0,   123,     0,   125,    21,
   125,     0,   125,    22,   125,     0,   125,    28,   125,     0,
   125,    29,   125,     0,   125,    30,   125,     0,   125,    32,
   125,     0,   125,    33,   125,     0,   125,    31,   125,     0,
   125,    25,   125,     0,   125,    26,   125,     0,   125,    23,
   125,     0,   125,    24,   125,     0,   125,     0,    94,   108,
   118,    85,     0,   126,     3,   130,     0,   126,     9,   130,
     0,   126,    10,   130,     0,   126,    11,   130,     0,   126,
    12,   130,     0,   126,    13,   130,     0,   126,    19,   130,
     0,   126,    20,   130,     0,   126,    14,   130,     0,   126,
    15,   130,     0,   126,    16,   130,     0,   126,    17,   130,
     0,   126,    18,   130,     0,   106,   129,     0,    52,     0,
   129,    52,     0,   125,     0,   128,     0,   127,     0,   132,
     0,   135,     0,   146,     0,   147,     0,   148,     0,   162,
     0,   168,     0,    72,   133,     0,    73,   133,     0,   134,
     0,   133,   134,     0,   106,     0,   106,     3,   130,     0,
   136,     0,   141,     0,    59,   179,   137,    50,     0,   138,
     0,   138,   140,     0,   130,   184,   102,     0,   138,   139,
     0,    60,   179,   184,   130,   184,   102,     0,    61,   179,
   184,   102,     0,    62,   179,   130,   184,   142,    50,     0,
   143,     0,   143,   145,     0,   144,     0,   143,   144,     0,
    63,   179,   184,   130,   184,   103,     0,    64,   179,   184,
   102,     0,    56,   179,   130,   184,   102,    50,     0,    57,
   179,   184,   102,    58,   130,     0,    55,   179,   126,     3,
   130,   184,   102,    50,     0,    65,     0,    66,     0,    67,
     0,    68,   179,   184,   102,    69,   179,   184,   102,    50,
     0,    70,   179,   184,   102,    71,   179,   184,   102,    50,
     0,     0,     0,     0,     0,    91,     0,    97,     0,   153,
   154,     0,   153,    77,   154,     0,   156,   154,     0,   156,
    88,    77,   154,     0,   153,   106,     0,   156,    88,   106,
     0,   153,     1,     0,   156,    88,     1,     0,    94,   151,
   150,     0,   157,   160,     0,   157,    77,   160,     0,   157,
   159,   160,     0,   157,   159,    88,    77,   160,     0,   106,
     0,   159,    88,   106,     0,   149,    95,     0,    76,   179,
   149,     0,   161,   164,     0,   161,   106,   163,     0,   161,
   158,   163,     0,   149,     3,   164,     0,   106,   150,   152,
   165,     0,   155,   166,     0,   166,     0,   184,   102,   167,
     0,    50,     0,    74,     0,    51,     0,    51,   169,     0,
    51,   171,     0,    51,   169,   171,     0,   170,     0,   170,
   170,     0,   170,   170,   170,     0,    84,   130,    83,   130,
    85,     0,    84,    83,   130,    85,     0,    84,   130,    83,
    85,     0,    84,    83,    85,     0,    84,    85,     0,   172,
     0,   171,    88,   172,     0,   130,     0,   130,   173,     0,
   175,     0,   177,     0,   178,     0,   174,     0,   173,   175,
     0,   173,   177,     0,   173,   178,     0,   173,   174,     0,
    82,    54,     0,   176,     0,   176,   130,     0,    79,   130,
     0,   176,    83,   130,     0,    80,   130,     0,    81,    53,
     0,    81,    53,   130,     0,    81,    53,   130,   130,     0,
     0,    75,     0,     1,     0,    88,     0,    87,     0,   181,
    88,     0,   181,    87,     0,     0,   181,     0,    88,     0,
    87,     0,    89,     0,   183,    88,     0,   183,    87,     0,
   183,    89,     0,     0,   183,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   459,   465,   471,   473,   477,   479,   481,   483,   487,   494,
   496,   504,   506,   510,   517,   522,   530,   537,   544,   560,
   567,   569,   571,   576,   580,   582,   584,   591,   593,   597,
   599,   606,   608,   610,   614,   616,   620,   622,   630,   632,
   636,   638,   640,   642,   644,   648,   655,   657,   659,   665,
   670,   675,   684,   688,   690,   692,   694,   696,   698,   700,
   702,   704,   708,   710,   712,   714,   716,   718,   720,   724,
   726,   728,   730,   732,   734,   736,   738,   740,   742,   744,
   746,   750,   754,   756,   763,   765,   767,   769,   771,   773,
   775,   777,   779,   781,   783,   785,   787,   794,   796,   803,
   805,   807,   809,   811,   813,   815,   817,   819,   821,   823,
   825,   827,   831,   835,   840,   848,   850,   852,   860,   862,
   864,   866,   868,   870,   872,   880,   882,   886,   888,   895,
   897,   905,   907,   915,   922,   924,   931,   933,   940,   944,
   954,   961,   963,   970,   972,   979,   983,   993,   998,  1003,
  1014,  1019,  1024,  1035,  1041,  1053,  1057,  1061,  1065,  1073,
  1077,  1081,  1086,  1093,  1099,  1108,  1110,  1115,  1121,  1133,
  1136,  1141,  1148,  1153,  1161,  1163,  1170,  1177,  1181,  1187,
  1193,  1201,  1205,  1212,  1214,  1218,  1222,  1232,  1243,  1248,
  1253,  1258,  1265,  1267,  1269,  1273,  1275,  1277,  1279,  1281,
  1285,  1287,  1294,  1296,  1300,  1305,  1310,  1315,  1320,  1328,
  1336,  1344,  1354,  1361,  1366,  1373,  1378,  1382,  1386,  1388,
  1390,  1398,  1402,  1404,  1407,  1409,  1411,  1413,  1417,  1419,
  1423,  1425,  1427,  1429,  1431,  1433,  1437,  1439
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","'='","':'",
"'-'","'+'","'*'","'/'","ADD_EQ","SUB_EQ","MUL_EQ","DIV_EQ","LEFTDIV_EQ","EMUL_EQ",
"EDIV_EQ","ELEFTDIV_EQ","AND_EQ","OR_EQ","LSHIFT_EQ","RSHIFT_EQ","LSHIFT","RSHIFT",
"EXPR_AND_AND","EXPR_OR_OR","EXPR_AND","EXPR_OR","EXPR_NOT","EXPR_LT","EXPR_LE",
"EXPR_EQ","EXPR_NE","EXPR_GE","EXPR_GT","LEFTDIV","EMUL","EDIV","ELEFTDIV","EPLUS",
"EMINUS","QUOTE","TRANSPOSE","PLUS_PLUS","MINUS_MINUS","POW","EPOW","NUM","IMAG_NUM",
"STRUCT_ELT","NAME","END","PLOT","TEXT","STYLE","AXES_TAG","FOR","WHILE","DO",
"UNTIL","IF","ELSEIF","ELSE","SWITCH","CASE","OTHERWISE","BREAK","CONTINUE",
"FUNC_RET","UNWIND","CLEANUP","TRY","CATCH","GLOBAL","STATIC","END_OF_INPUT",
"LEXICAL_ERROR","FCN","ELLIPSIS","ALL_VA_ARGS","USING","TITLE","WITH","AXES",
"COLON","OPEN_BRACE","CLOSE_BRACE","CLEAR","';'","','","'\\n'","UNARY","'('",
"'.'","'{'","'['","']'","'}'","')'","input","input1","simple_list","simple_list1",
"opt_list","list","list1","statement","identifier","constant","in_matrix_or_assign_lhs",
"matrix","matrix_rows","matrix_rows1","cell","cell_rows","cell_rows1","cell_or_matrix_row",
"primary_expr","magic_colon","arg_list","parsing_indir","postfix_expr","prefix_expr",
"binary_expr","colon_expr","colon_expr1","simple_expr","assign_lhs","assign_expr",
"word_list_cmd","word_list","expression","command","declaration","decl1","decl2",
"select_command","if_command","if_cmd_list","if_cmd_list1","elseif_clause","else_clause",
"switch_command","case_list","case_list1","switch_case","default_case","loop_command",
"jump_command","except_command","global_symtab","local_symtab","in_return_list",
"parsed_fcn_name","param_list_beg","param_list_end","param_list","param_list1",
"return_list_beg","return_list","return_list1","return_list_end","function_beg",
"function","function1","function2","function3","function4","function_end","plot_command",
"ranges","ranges1","plot_command1","plot_command2","plot_options","axes","using",
"using1","title","style","stash_comment","parse_error","sep_no_nl","opt_sep_no_nl",
"sep","opt_sep", NULL
};
#endif

static const short yyr1[] = {     0,
    98,    98,    98,    98,    99,    99,    99,    99,   100,   101,
   101,   102,   102,   103,   104,   104,   105,   105,   105,   106,
   107,   107,   107,   108,   109,   109,   109,   110,   110,   111,
   111,   112,   112,   112,   113,   113,   114,   114,   115,   115,
   116,   116,   116,   116,   116,   117,   118,   118,   118,   118,
   118,   118,   119,   120,   120,   120,   120,   120,   120,   120,
   120,   120,   121,   121,   121,   121,   121,   121,   121,   122,
   122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
   122,   123,   124,   124,   125,   125,   125,   125,   125,   125,
   125,   125,   125,   125,   125,   125,   125,   126,   126,   127,
   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
   127,   127,   128,   129,   129,   130,   130,   130,   131,   131,
   131,   131,   131,   131,   131,   132,   132,   133,   133,   134,
   134,   135,   135,   136,   137,   137,   138,   138,   139,   140,
   141,   142,   142,   143,   143,   144,   145,   146,   146,   146,
   147,   147,   147,   148,   148,   149,   150,   151,   152,   153,
   154,   155,   155,   155,   155,   156,   156,   156,   156,   157,
   158,   158,   158,   158,   159,   159,   160,   161,   162,   162,
   162,   163,   164,   165,   165,   166,   167,   167,   168,   168,
   168,   168,   169,   169,   169,   170,   170,   170,   170,   170,
   171,   171,   172,   172,   173,   173,   173,   173,   173,   173,
   173,   173,   174,   175,   175,   176,   176,   177,   178,   178,
   178,   179,   180,   180,   181,   181,   181,   181,   182,   182,
   183,   183,   183,   183,   183,   183,   184,   184
};

static const short yyr2[] = {     0,
     1,     1,     2,     1,     1,     1,     2,     2,     2,     1,
     3,     0,     1,     2,     1,     3,     1,     1,     2,     1,
     1,     1,     1,     0,     2,     3,     4,     1,     2,     1,
     3,     2,     3,     3,     1,     2,     1,     3,     1,     2,
     1,     1,     1,     1,     3,     1,     1,     1,     1,     3,
     3,     3,     0,     1,     3,     4,     4,     2,     2,     2,
     2,     4,     1,     1,     2,     2,     2,     2,     2,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     1,     1,     3,     1,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     1,     4,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     2,     1,     2,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     2,     1,     2,     1,
     3,     1,     1,     4,     1,     2,     3,     2,     6,     4,
     6,     1,     2,     1,     2,     6,     4,     6,     6,     8,
     1,     1,     1,     9,     9,     0,     0,     0,     0,     1,
     1,     2,     3,     2,     4,     2,     3,     2,     3,     3,
     2,     3,     3,     5,     1,     3,     2,     3,     2,     3,
     3,     3,     4,     2,     1,     3,     1,     1,     1,     2,
     2,     3,     1,     2,     3,     5,     4,     4,     3,     2,
     1,     3,     1,     2,     1,     1,     1,     1,     2,     2,
     2,     2,     2,     1,     2,     2,     3,     2,     2,     3,
     4,     0,     1,     1,     1,     1,     2,     2,     0,     1,
     1,     1,     1,     2,     2,     2,     0,     1
};

static const short yydefact[] = {     0,
   224,     0,     0,     0,     0,     0,    21,    22,    20,   189,
    23,   222,   222,   222,   222,   222,   151,   152,   153,   222,
   222,     0,     0,     2,   223,   222,     5,     0,     0,    24,
     1,     0,   229,    10,    41,    42,    43,    44,    54,    63,
    83,    64,    85,    82,   116,     0,   118,   117,    17,    18,
   119,   120,   132,   133,   121,   122,   123,     0,   124,   125,
     4,    24,    41,    69,    68,    67,    65,    66,     0,    19,
   203,   190,   193,   191,   201,     0,     0,   237,     0,     0,
   237,   237,   130,   126,   128,   127,   156,     0,    46,    49,
     0,    32,     0,    35,    37,    48,    39,    47,     0,    25,
     0,     8,     7,     3,   226,   225,   230,     9,   114,   113,
    60,    61,    58,    59,     0,    53,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   158,   157,   156,   156,   179,
     0,     0,   200,     0,     0,     0,     0,     0,   204,   208,
   205,   214,   206,   207,   192,   194,     0,    98,     0,   237,
   232,   231,   233,   238,    12,   237,     0,   135,   237,    12,
    12,     0,   129,   178,    45,    33,    34,    36,    40,    26,
     0,    28,    30,    39,   228,   227,    11,   115,    55,     0,
     0,     0,    73,    72,    74,    75,    80,    78,    79,    81,
    76,    77,    70,    71,    84,    86,    87,    96,    97,    94,
    95,    88,    89,    90,    93,    91,    92,   100,   101,   102,
   103,   104,   105,   108,   109,   110,   111,   112,   106,   107,
   157,     0,   159,   180,   156,   175,     0,   156,   171,   181,
   199,     0,     0,   216,   218,   219,   213,   212,   209,   210,
   211,     0,   215,   195,   202,     0,    12,   235,   234,   236,
     0,    13,   237,    15,    12,   134,   222,   222,   138,   136,
     0,     0,     0,   131,    38,    52,    50,    51,    27,    29,
    99,     0,    56,    62,    57,   170,     0,   237,   172,   177,
     0,   173,   197,   198,     0,   220,   217,   237,     0,     0,
   238,    14,   137,   237,   237,   222,     0,   142,   144,   222,
   222,    31,   157,   182,   160,     0,   237,     0,   183,   185,
    12,   156,   176,   196,   221,    12,   148,   149,    16,     0,
    12,   237,   141,   222,   145,   143,   237,   237,   168,     0,
   161,   166,   162,   184,     0,   164,     0,   174,     0,   237,
   140,     0,   237,    12,    12,   163,   169,     0,   167,   187,
   188,   186,   150,    12,   237,    12,     0,     0,   165,   139,
     0,   147,   154,   155,   146,     0,     0,     0
};

static const short yydefgoto[] = {   396,
    31,    32,    33,   281,   282,   283,   284,    35,    36,   101,
    37,   201,   202,    38,    93,    94,   203,    39,    96,    97,
   211,    40,    41,    42,    43,    44,    45,    46,    47,    48,
   110,    49,    50,    51,    84,    85,    52,    53,   187,   188,
   289,   290,    54,   327,   328,   329,   356,    55,    56,    57,
   257,   253,   251,   308,   336,   363,   337,   338,   158,   159,
   258,   259,    58,    59,   254,   160,   339,   340,   382,    60,
    72,    73,    74,    75,   169,   170,   171,   172,   173,   174,
    76,    61,   107,   108,   184,   341
};

static const short yypact[] = {   363,
-32768,   305,   305,   305,   305,   305,-32768,-32768,-32768,   725,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    -6,    -6,-32768,-32768,-32768,-32768,   833,   444,   -55,
-32768,    24,   -15,-32768,    -4,-32768,-32768,-32768,-32768,   -14,
   158,-32768,-32768,    47,   937,   968,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -18,-32768,-32768,
-32768,   -55,-32768,    36,    36,    36,    36,    36,   739,-32768,
    89,   833,   -30,   -22,-32768,   833,   833,    20,   833,   833,
    20,    20,    55,    -6,-32768,    -6,-32768,   -26,-32768,-32768,
   -28,-32768,   -21,     5,-32768,-32768,    -3,-32768,    -1,-32768,
   499,-32768,-32768,-32768,-32768,-32768,   615,-32768,-32768,    48,
-32768,-32768,-32768,-32768,   436,-32768,   499,   305,   305,   305,
   305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
   305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
   305,   305,   833,   833,   833,   833,   833,   833,   833,   833,
   833,   833,   833,   833,   833,-32768,   112,   -16,-32768,-32768,
   499,   751,-32768,    66,   833,   833,    98,   101,    89,-32768,
-32768,   807,-32768,-32768,   -22,   -30,   833,   550,   170,    20,
-32768,-32768,-32768,   117,   670,    20,   111,    44,    20,   670,
   670,   833,-32768,-32768,-32768,-32768,-32768,   499,   507,-32768,
    85,    91,-32768,   -46,-32768,-32768,-32768,-32768,-32768,   -53,
   133,   -51,    52,    52,    36,    36,    36,    36,    36,    36,
    52,    52,    36,    36,   158,-32768,-32768,   269,   269,   154,
   154,   122,   122,   122,   122,   122,   122,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   186,-32768,-32768,-32768,-32768,    96,   110,-32768,-32768,
-32768,   114,   820,-32768,-32768,   833,-32768,-32768,-32768,-32768,
-32768,   833,-32768,-32768,-32768,   833,   670,-32768,-32768,-32768,
   149,-32768,    20,-32768,   670,-32768,-32768,-32768,-32768,-32768,
   145,   140,   142,-32768,-32768,-32768,-32768,-32768,-32768,   499,
-32768,   507,-32768,-32768,-32768,-32768,    -6,    71,-32768,-32768,
   -13,-32768,-32768,-32768,   126,   833,-32768,    20,   164,   833,
   560,-32768,-32768,    20,    20,-32768,   165,    83,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    13,    20,   -50,-32768,-32768,
   670,-32768,-32768,-32768,-32768,   670,-32768,-32768,-32768,   833,
   670,    20,-32768,-32768,-32768,-32768,    20,    20,-32768,   132,
-32768,-32768,-32768,-32768,    14,-32768,    -9,-32768,   180,    20,
-32768,   833,    20,   670,   670,-32768,-32768,   132,-32768,-32768,
-32768,-32768,-32768,   670,    20,   670,   181,   182,-32768,-32768,
   670,-32768,-32768,-32768,-32768,   233,   236,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,  -174,  -154,-32768,    34,    -2,-32768,   178,
-32768,-32768,-32768,-32768,-32768,-32768,   -19,-32768,  -190,   -48,
-32768,-32768,   258,-32768,-32768,-32768,   797,   166,-32768,-32768,
-32768,    73,-32768,-32768,   218,   -29,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   -85,-32768,-32768,-32768,-32768,
   -64,    -7,-32768,-32768,-32768,  -308,-32768,-32768,-32768,-32768,
-32768,  -236,-32768,-32768,    87,   -60,-32768,   -89,-32768,-32768,
-32768,   -62,   177,    75,-32768,    84,    86,-32768,    88,    90,
    -8,   222,-32768,-32768,   -27,   -32
};


#define	YYLAST		988


static const short yytable[] = {    63,
    63,    63,    63,    63,    77,    78,    79,    80,   297,    95,
   176,    81,    82,   359,   377,   292,   293,    87,   309,    83,
    83,   312,   194,    -6,     1,   111,   112,   113,   114,   366,
     9,    99,     9,    34,   302,     9,   302,   365,   301,   100,
   380,   199,     9,   303,   305,   185,   361,   109,   190,   191,
   130,   376,   204,    69,   193,   157,   193,   192,   120,   121,
   255,     9,     9,   342,   381,   177,   210,   196,   212,   389,
   195,   105,   106,    63,   197,   156,   115,   116,   117,   128,
   129,    83,    71,    83,   199,   122,   123,   124,   125,   360,
   378,   198,   252,   200,   252,   128,   129,   102,    25,   208,
    88,    98,   319,   287,   288,   368,   181,   182,   183,   361,
   323,   297,   103,   274,  -156,    63,    63,    63,    63,    63,
    63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
    63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
   207,   164,   131,   132,    71,   326,   354,   277,   263,   180,
   266,   186,   189,   285,   267,   256,   291,   181,   182,   183,
   286,   335,   118,   119,   120,   121,   367,   165,   166,   167,
   168,   369,   276,    98,   131,   132,   371,   300,   295,   299,
   304,   137,   138,   139,   140,   141,   142,    98,   307,    98,
   310,   122,   123,   124,   125,   126,   127,   311,   313,   387,
   388,   128,   129,   278,   279,   280,   320,   326,   330,   390,
   344,   392,   331,   347,   353,   238,   239,   240,   241,   242,
   243,   244,   245,   246,   247,   248,   249,   250,   361,   383,
   393,   394,   397,    98,   262,   398,   395,   264,   265,   161,
    86,   179,   355,   306,   273,   260,   334,   364,   175,    71,
   322,   275,   268,   104,   269,   321,   270,     0,   271,    64,
    65,    66,    67,    68,   294,     0,     0,     0,     0,     0,
    98,   298,     0,     0,     0,     0,     0,     0,   324,   325,
   332,     0,     0,     0,     0,   346,     0,     0,     0,   131,
   132,   350,   351,   135,   136,     0,   137,   138,   139,   140,
   141,   142,     0,     0,   333,     0,     0,     0,   343,     2,
     3,     0,     0,     0,     0,     0,     0,   352,     0,   372,
     0,   357,   358,     0,   374,   375,     0,     0,     0,     0,
     0,     4,     0,   362,     0,   315,     0,   384,   316,     0,
   386,     0,     0,     0,   317,   373,     5,     6,   318,     0,
     7,     8,   391,     9,   349,     0,    11,     0,     0,     0,
     0,     0,   379,     1,     0,     0,     0,     2,     3,     0,
     0,     0,    98,     0,   298,   213,   214,   215,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   345,     4,
     0,     0,   348,     0,     0,    28,     0,    29,    62,     0,
     0,     0,     0,     0,     5,     6,     0,     0,     7,     8,
     0,     9,     0,    10,    11,     0,     0,    12,    13,    14,
     0,    15,   370,     0,    16,     0,     0,    17,    18,    19,
    20,     0,    21,     0,    22,    23,    24,    25,    26,    89,
     2,     3,     0,     0,   385,     0,     0,    89,     2,     3,
     0,    27,     0,    28,     0,    29,    30,     0,     0,     0,
     0,     0,     4,     0,     0,     0,     0,     0,     0,     0,
     4,     0,     0,     0,     0,     0,     0,     5,     6,     0,
     0,     7,     8,     0,     9,     5,     6,    11,     0,     7,
     8,     0,     9,     0,     0,    11,     0,     0,     0,     0,
     0,     0,    89,     2,     3,     0,     0,     0,     0,     0,
    89,     2,     3,    90,     0,     0,     0,     0,     0,     0,
     0,    90,     0,     0,     0,     4,    28,     0,    29,    30,
    91,     0,   209,     4,    28,     0,    29,    30,     0,    92,
     5,     6,     0,     0,     7,     8,     0,     9,     5,     6,
    11,     0,     7,     8,     0,     9,     0,     0,    11,     0,
     0,     0,     0,     0,     2,     3,     0,     0,     0,     0,
   131,   132,   133,   134,   135,   136,    90,   137,   138,   139,
   140,   141,   142,     0,   296,     0,     4,     0,     0,    28,
     0,    29,    30,     0,     0,     0,     0,    28,     0,    29,
    30,     5,     6,     0,     0,     7,     8,     0,     9,     0,
    10,    11,     0,     0,    12,    13,    14,     0,    15,     2,
     3,    16,     0,     0,    17,    18,    19,    20,     0,    21,
     0,    22,    23,     0,     0,    26,     0,     0,     0,     0,
     0,     4,     0,     0,     0,     0,   278,   279,   280,     0,
    28,     0,    29,    30,     0,     0,     5,     6,     0,     0,
     7,     8,     0,     9,     0,    10,    11,     0,     0,    12,
    13,    14,     0,    15,     2,     3,    16,     0,     0,    17,
    18,    19,    20,     0,    21,     0,    22,    23,     0,     0,
    26,     0,     0,     0,     0,     0,     4,     0,     0,     0,
     0,   205,   206,     0,     0,    28,     0,    29,    30,     0,
     0,     5,     6,     0,     0,     7,     8,     0,     9,     0,
    10,    11,     0,     0,    12,    13,    14,     0,    15,     2,
     3,    16,     0,     0,    17,    18,    19,    20,     0,    21,
     0,    22,    23,     2,     3,    26,     0,     0,     0,     0,
     0,     4,     0,     0,     0,     2,     3,     0,     0,     0,
    28,     0,    29,    30,     0,     4,     5,     6,     0,     0,
     7,     8,     0,     9,     0,     0,    11,     4,     0,     0,
     5,     6,     0,     0,     7,     8,     0,     9,     0,     0,
    11,     0,     5,     6,     0,     0,     7,     8,     0,     9,
     0,     0,    11,     0,     0,     0,     0,     0,    69,     0,
    70,     2,     3,     0,     0,    28,     0,    29,    30,     0,
     0,   162,     0,   163,     2,     3,     0,     0,     0,    28,
     0,    29,    30,     4,     0,   261,     0,     2,     3,     0,
     0,    28,     0,    29,    30,     0,     4,     0,     5,     6,
     0,     0,     7,     8,     0,     9,     0,     0,    11,     4,
     0,     5,     6,     0,     0,     7,     8,     0,     9,     0,
     0,    11,   178,     0,     5,     6,     0,     0,     7,     8,
     0,     9,     0,     0,    11,     0,     0,     0,     0,   272,
     0,     0,     0,     0,     0,     0,     0,    28,     0,    29,
    30,     0,     0,     0,   314,     0,     0,     0,     0,     0,
    28,     0,    29,    30,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    28,     0,    29,    30,   226,   227,   228,
   229,   230,   231,   232,   233,   234,   235,   236,   237,   -98,
     0,     0,     0,     0,     0,   -98,   -98,   -98,   -98,   -98,
   -98,   -98,   -98,   -98,   -98,   -98,   -98,   131,   132,   133,
   134,   135,   136,     0,   137,   138,   139,   140,   141,   142,
   143,     0,     0,     0,     0,     0,   144,   145,   146,   147,
   148,   149,   150,   151,   152,   153,   154,   155
};

static const short yycheck[] = {     2,
     3,     4,     5,     6,    13,    14,    15,    16,   199,    29,
    73,    20,    21,     1,     1,   190,   191,    26,   255,    22,
    23,   258,    87,     0,     1,    40,    41,    42,    43,   338,
    49,    87,    49,     0,    88,    49,    88,    88,    85,    95,
    50,    88,    49,    97,    96,    78,    97,    52,    81,    82,
     4,   360,   101,    84,    84,    58,    86,     3,     7,     8,
    77,    49,    49,    77,    74,    88,   115,    96,   117,   378,
    97,    87,    88,    76,    96,    94,    91,    92,    93,    44,
    45,    84,    10,    86,    88,    34,    35,    36,    37,    77,
    77,    87,   157,    95,   159,    44,    45,    74,    75,    52,
    28,    29,   277,    60,    61,   342,    87,    88,    89,    97,
   285,   302,    89,   176,     3,   118,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
   107,    69,    21,    22,    72,    63,    64,   180,    83,    77,
    53,    79,    80,   186,    54,   158,   189,    87,    88,    89,
    50,    91,     5,     6,     7,     8,   341,    79,    80,    81,
    82,   346,     3,   101,    21,    22,   351,    87,   198,    95,
    48,    28,    29,    30,    31,    32,    33,   115,     3,   117,
    95,    34,    35,    36,    37,    38,    39,    88,    85,   374,
   375,    44,    45,    87,    88,    89,    58,    63,    69,   384,
    85,   386,    71,    50,    50,   143,   144,   145,   146,   147,
   148,   149,   150,   151,   152,   153,   154,   155,    97,    50,
    50,    50,     0,   161,   162,     0,   391,   165,   166,    62,
    23,    76,   328,   251,   172,   159,   307,   337,    72,   177,
   283,   177,   169,    32,   169,   283,   169,    -1,   169,     2,
     3,     4,     5,     6,   192,    -1,    -1,    -1,    -1,    -1,
   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,   287,   288,
   300,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    21,
    22,   324,   325,    25,    26,    -1,    28,    29,    30,    31,
    32,    33,    -1,    -1,   307,    -1,    -1,    -1,   311,     5,
     6,    -1,    -1,    -1,    -1,    -1,    -1,   326,    -1,   352,
    -1,   330,   331,    -1,   357,   358,    -1,    -1,    -1,    -1,
    -1,    27,    -1,   336,    -1,   263,    -1,   370,   266,    -1,
   373,    -1,    -1,    -1,   272,   354,    42,    43,   276,    -1,
    46,    47,   385,    49,   321,    -1,    52,    -1,    -1,    -1,
    -1,    -1,   365,     1,    -1,    -1,    -1,     5,     6,    -1,
    -1,    -1,   300,    -1,   302,   118,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,   129,   130,   316,    27,
    -1,    -1,   320,    -1,    -1,    91,    -1,    93,    94,    -1,
    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    46,    47,
    -1,    49,    -1,    51,    52,    -1,    -1,    55,    56,    57,
    -1,    59,   350,    -1,    62,    -1,    -1,    65,    66,    67,
    68,    -1,    70,    -1,    72,    73,    74,    75,    76,     4,
     5,     6,    -1,    -1,   372,    -1,    -1,     4,     5,     6,
    -1,    89,    -1,    91,    -1,    93,    94,    -1,    -1,    -1,
    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    27,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,
    -1,    46,    47,    -1,    49,    42,    43,    52,    -1,    46,
    47,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
    -1,    -1,     4,     5,     6,    -1,    -1,    -1,    -1,    -1,
     4,     5,     6,    78,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    78,    -1,    -1,    -1,    27,    91,    -1,    93,    94,
    87,    -1,    97,    27,    91,    -1,    93,    94,    -1,    96,
    42,    43,    -1,    -1,    46,    47,    -1,    49,    42,    43,
    52,    -1,    46,    47,    -1,    49,    -1,    -1,    52,    -1,
    -1,    -1,    -1,    -1,     5,     6,    -1,    -1,    -1,    -1,
    21,    22,    23,    24,    25,    26,    78,    28,    29,    30,
    31,    32,    33,    -1,    78,    -1,    27,    -1,    -1,    91,
    -1,    93,    94,    -1,    -1,    -1,    -1,    91,    -1,    93,
    94,    42,    43,    -1,    -1,    46,    47,    -1,    49,    -1,
    51,    52,    -1,    -1,    55,    56,    57,    -1,    59,     5,
     6,    62,    -1,    -1,    65,    66,    67,    68,    -1,    70,
    -1,    72,    73,    -1,    -1,    76,    -1,    -1,    -1,    -1,
    -1,    27,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
    91,    -1,    93,    94,    -1,    -1,    42,    43,    -1,    -1,
    46,    47,    -1,    49,    -1,    51,    52,    -1,    -1,    55,
    56,    57,    -1,    59,     5,     6,    62,    -1,    -1,    65,
    66,    67,    68,    -1,    70,    -1,    72,    73,    -1,    -1,
    76,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
    -1,    87,    88,    -1,    -1,    91,    -1,    93,    94,    -1,
    -1,    42,    43,    -1,    -1,    46,    47,    -1,    49,    -1,
    51,    52,    -1,    -1,    55,    56,    57,    -1,    59,     5,
     6,    62,    -1,    -1,    65,    66,    67,    68,    -1,    70,
    -1,    72,    73,     5,     6,    76,    -1,    -1,    -1,    -1,
    -1,    27,    -1,    -1,    -1,     5,     6,    -1,    -1,    -1,
    91,    -1,    93,    94,    -1,    27,    42,    43,    -1,    -1,
    46,    47,    -1,    49,    -1,    -1,    52,    27,    -1,    -1,
    42,    43,    -1,    -1,    46,    47,    -1,    49,    -1,    -1,
    52,    -1,    42,    43,    -1,    -1,    46,    47,    -1,    49,
    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    84,    -1,
    86,     5,     6,    -1,    -1,    91,    -1,    93,    94,    -1,
    -1,    83,    -1,    85,     5,     6,    -1,    -1,    -1,    91,
    -1,    93,    94,    27,    -1,    85,    -1,     5,     6,    -1,
    -1,    91,    -1,    93,    94,    -1,    27,    -1,    42,    43,
    -1,    -1,    46,    47,    -1,    49,    -1,    -1,    52,    27,
    -1,    42,    43,    -1,    -1,    46,    47,    -1,    49,    -1,
    -1,    52,    76,    -1,    42,    43,    -1,    -1,    46,    47,
    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    83,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    93,
    94,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
    91,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    91,    -1,    93,    94,   131,   132,   133,
   134,   135,   136,   137,   138,   139,   140,   141,   142,     3,
    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
     3,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 460 "parse.y"
{
		    global_command = yyvsp[0].tree_statement_list_type;
		    promptflag = 1;
		    YYACCEPT;
		  ;
    break;}
case 2:
#line 466 "parse.y"
{
		    global_command = 0;
		    promptflag = 1;
		    YYABORT;
		  ;
    break;}
case 3:
#line 472 "parse.y"
{ ABORT_PARSE; ;
    break;}
case 4:
#line 474 "parse.y"
{ ABORT_PARSE; ;
    break;}
case 5:
#line 478 "parse.y"
{ yyval.tree_statement_list_type = 0; ;
    break;}
case 6:
#line 480 "parse.y"
{ yyval.tree_statement_list_type = yyvsp[0].tree_statement_list_type; ;
    break;}
case 7:
#line 482 "parse.y"
{ yyval.tree_statement_list_type = yyvsp[-1].tree_statement_list_type; ;
    break;}
case 8:
#line 484 "parse.y"
{ yyval.tree_statement_list_type = yyvsp[-1].tree_statement_list_type; ;
    break;}
case 9:
#line 488 "parse.y"
{
		    set_stmt_print_flag (yyvsp[-1].tree_statement_list_type, yyvsp[0].sep_type, false);
		    yyval.tree_statement_list_type = yyvsp[-1].tree_statement_list_type;
		  ;
    break;}
case 10:
#line 495 "parse.y"
{ yyval.tree_statement_list_type = new tree_statement_list (yyvsp[0].tree_statement_type); ;
    break;}
case 11:
#line 497 "parse.y"
{
		    set_stmt_print_flag (yyvsp[-2].tree_statement_list_type, yyvsp[-1].sep_type, false);
		    yyvsp[-2].tree_statement_list_type->append (yyvsp[0].tree_statement_type);
		    yyval.tree_statement_list_type = yyvsp[-2].tree_statement_list_type;
		  ;
    break;}
case 12:
#line 505 "parse.y"
{ yyval.tree_statement_list_type = new tree_statement_list (); ;
    break;}
case 13:
#line 507 "parse.y"
{ yyval.tree_statement_list_type = yyvsp[0].tree_statement_list_type; ;
    break;}
case 14:
#line 511 "parse.y"
{
		    set_stmt_print_flag (yyvsp[-1].tree_statement_list_type, yyvsp[0].sep_type, true);
		    yyval.tree_statement_list_type = yyvsp[-1].tree_statement_list_type;
		  ;
    break;}
case 15:
#line 518 "parse.y"
{
		    lexer_flags.beginning_of_function = false;
		    yyval.tree_statement_list_type = new tree_statement_list (yyvsp[0].tree_statement_type);
		  ;
    break;}
case 16:
#line 523 "parse.y"
{
		    set_stmt_print_flag (yyvsp[-2].tree_statement_list_type, yyvsp[-1].sep_type, true);
		    yyvsp[-2].tree_statement_list_type->append (yyvsp[0].tree_statement_type);
		    yyval.tree_statement_list_type = yyvsp[-2].tree_statement_list_type;
		  ;
    break;}
case 17:
#line 531 "parse.y"
{
		    octave_comment_list *comment
		      = octave_comment_buffer::get_comment ();

		    yyval.tree_statement_type = new tree_statement (yyvsp[0].tree_expression_type, comment);
		  ;
    break;}
case 18:
#line 538 "parse.y"
{
		    octave_comment_list *comment
		      = octave_comment_buffer::get_comment ();

		    yyval.tree_statement_type = new tree_statement (yyvsp[0].tree_command_type, comment);
		  ;
    break;}
case 19:
#line 545 "parse.y"
{
		    symbol_record *sr = lookup_by_name ("clearplot", 0);
		    tree_identifier *id = new tree_identifier (sr);

		    octave_comment_list *comment
		      = octave_comment_buffer::get_comment ();

		    yyval.tree_statement_type = new tree_statement (id, comment);
		  ;
    break;}
case 20:
#line 561 "parse.y"
{
		    yyval.tree_identifier_type = new tree_identifier
		      (yyvsp[0].tok_val->sym_rec (), yyvsp[0].tok_val->line (), yyvsp[0].tok_val->column ());
		  ;
    break;}
case 21:
#line 568 "parse.y"
{ yyval.tree_constant_type = make_constant (NUM, yyvsp[0].tok_val); ;
    break;}
case 22:
#line 570 "parse.y"
{ yyval.tree_constant_type = make_constant (IMAG_NUM, yyvsp[0].tok_val); ;
    break;}
case 23:
#line 572 "parse.y"
{ yyval.tree_constant_type = make_constant (TEXT, yyvsp[0].tok_val); ;
    break;}
case 24:
#line 577 "parse.y"
{ lexer_flags.looking_at_matrix_or_assign_lhs = true; ;
    break;}
case 25:
#line 581 "parse.y"
{ yyval.tree_expression_type = new tree_constant (octave_value (Matrix ())); ;
    break;}
case 26:
#line 583 "parse.y"
{ yyval.tree_expression_type = new tree_constant (octave_value (Matrix ())); ;
    break;}
case 27:
#line 585 "parse.y"
{
		    yyval.tree_expression_type = finish_matrix (yyvsp[-1].tree_matrix_type);
		    lexer_flags.looking_at_matrix_or_assign_lhs = false;
		  ;
    break;}
case 28:
#line 592 "parse.y"
{ yyval.tree_matrix_type = yyvsp[0].tree_matrix_type; ;
    break;}
case 29:
#line 594 "parse.y"
{ yyval.tree_matrix_type = yyvsp[-1].tree_matrix_type; ;
    break;}
case 30:
#line 598 "parse.y"
{ yyval.tree_matrix_type = new tree_matrix (yyvsp[0].tree_argument_list_type); ;
    break;}
case 31:
#line 600 "parse.y"
{
		    yyvsp[-2].tree_matrix_type->append (yyvsp[0].tree_argument_list_type);
		    yyval.tree_matrix_type = yyvsp[-2].tree_matrix_type;
		  ;
    break;}
case 32:
#line 607 "parse.y"
{ yyval.tree_expression_type = new tree_constant (octave_value (Matrix ())); ;
    break;}
case 33:
#line 609 "parse.y"
{ yyval.tree_expression_type = new tree_constant (octave_value (Matrix ())); ;
    break;}
case 34:
#line 611 "parse.y"
{ yyval.tree_expression_type = finish_cell (yyvsp[-1].tree_cell_type); ;
    break;}
case 35:
#line 615 "parse.y"
{ yyval.tree_cell_type = yyvsp[0].tree_cell_type; ;
    break;}
case 36:
#line 617 "parse.y"
{ yyval.tree_cell_type = yyvsp[-1].tree_cell_type; ;
    break;}
case 37:
#line 621 "parse.y"
{ yyval.tree_cell_type = new tree_cell (yyvsp[0].tree_argument_list_type); ;
    break;}
case 38:
#line 623 "parse.y"
{
		    yyvsp[-2].tree_cell_type->append (yyvsp[0].tree_argument_list_type);
		    yyval.tree_cell_type = yyvsp[-2].tree_cell_type;
		  ;
    break;}
case 39:
#line 631 "parse.y"
{ yyval.tree_argument_list_type = yyvsp[0].tree_argument_list_type; ;
    break;}
case 40:
#line 633 "parse.y"
{ yyval.tree_argument_list_type = yyvsp[-1].tree_argument_list_type; ;
    break;}
case 41:
#line 637 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_identifier_type; ;
    break;}
case 42:
#line 639 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_constant_type; ;
    break;}
case 43:
#line 641 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 44:
#line 643 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 45:
#line 645 "parse.y"
{ yyval.tree_expression_type = yyvsp[-1].tree_expression_type->mark_in_parens (); ;
    break;}
case 46:
#line 649 "parse.y"
{
		    octave_value tmp (octave_value::magic_colon_t);
		    yyval.tree_constant_type = new tree_constant (tmp);
		  ;
    break;}
case 47:
#line 656 "parse.y"
{ yyval.tree_argument_list_type = new tree_argument_list (yyvsp[0].tree_expression_type); ;
    break;}
case 48:
#line 658 "parse.y"
{ yyval.tree_argument_list_type = new tree_argument_list (yyvsp[0].tree_constant_type); ;
    break;}
case 49:
#line 660 "parse.y"
{
		    octave_value tmp (octave_value::all_va_args_t);
		    tree_constant *all_va_args = new tree_constant (tmp);
		    yyval.tree_argument_list_type = new tree_argument_list (all_va_args);
		  ;
    break;}
case 50:
#line 666 "parse.y"
{
		    yyvsp[-2].tree_argument_list_type->append (yyvsp[0].tree_constant_type);
		    yyval.tree_argument_list_type = yyvsp[-2].tree_argument_list_type;
		  ;
    break;}
case 51:
#line 671 "parse.y"
{
		    yyvsp[-2].tree_argument_list_type->append (yyvsp[0].tree_expression_type);
		    yyval.tree_argument_list_type = yyvsp[-2].tree_argument_list_type;
		  ;
    break;}
case 52:
#line 676 "parse.y"
{
		    octave_value tmp (octave_value::all_va_args_t);
		    tree_constant *all_va_args = new tree_constant (tmp);
		    yyvsp[-2].tree_argument_list_type->append (all_va_args);
		    yyval.tree_argument_list_type = yyvsp[-2].tree_argument_list_type;
		  ;
    break;}
case 53:
#line 685 "parse.y"
{ lexer_flags.looking_at_indirect_ref = true; ;
    break;}
case 54:
#line 689 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 55:
#line 691 "parse.y"
{ yyval.tree_expression_type = make_index_expression (yyvsp[-2].tree_expression_type, 0); ;
    break;}
case 56:
#line 693 "parse.y"
{ yyval.tree_expression_type = make_index_expression (yyvsp[-3].tree_expression_type, yyvsp[-1].tree_argument_list_type); ;
    break;}
case 57:
#line 695 "parse.y"
{ yyval.tree_expression_type = make_index_expression (yyvsp[-3].tree_expression_type, yyvsp[-1].tree_argument_list_type); ;
    break;}
case 58:
#line 697 "parse.y"
{ yyval.tree_expression_type = make_postfix_op (PLUS_PLUS, yyvsp[-1].tree_expression_type, yyvsp[0].tok_val); ;
    break;}
case 59:
#line 699 "parse.y"
{ yyval.tree_expression_type = make_postfix_op (MINUS_MINUS, yyvsp[-1].tree_expression_type, yyvsp[0].tok_val); ;
    break;}
case 60:
#line 701 "parse.y"
{ yyval.tree_expression_type = make_postfix_op (QUOTE, yyvsp[-1].tree_expression_type, yyvsp[0].tok_val); ;
    break;}
case 61:
#line 703 "parse.y"
{ yyval.tree_expression_type = make_postfix_op (TRANSPOSE, yyvsp[-1].tree_expression_type, yyvsp[0].tok_val); ;
    break;}
case 62:
#line 705 "parse.y"
{ yyval.tree_expression_type = make_indirect_ref (yyvsp[-3].tree_expression_type, yyvsp[0].tok_val->text ()); ;
    break;}
case 63:
#line 709 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 64:
#line 711 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 65:
#line 713 "parse.y"
{ yyval.tree_expression_type = make_prefix_op (PLUS_PLUS, yyvsp[0].tree_expression_type, yyvsp[-1].tok_val); ;
    break;}
case 66:
#line 715 "parse.y"
{ yyval.tree_expression_type = make_prefix_op (MINUS_MINUS, yyvsp[0].tree_expression_type, yyvsp[-1].tok_val); ;
    break;}
case 67:
#line 717 "parse.y"
{ yyval.tree_expression_type = make_prefix_op (EXPR_NOT, yyvsp[0].tree_expression_type, yyvsp[-1].tok_val); ;
    break;}
case 68:
#line 719 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 69:
#line 721 "parse.y"
{ yyval.tree_expression_type = make_prefix_op ('-', yyvsp[0].tree_expression_type, yyvsp[-1].tok_val); ;
    break;}
case 70:
#line 725 "parse.y"
{ yyval.tree_expression_type = make_binary_op (POW, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 71:
#line 727 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EPOW, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 72:
#line 729 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('+', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 73:
#line 731 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('-', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 74:
#line 733 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('*', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 75:
#line 735 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('/', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 76:
#line 737 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('+', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 77:
#line 739 "parse.y"
{ yyval.tree_expression_type = make_binary_op ('-', yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 78:
#line 741 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EMUL, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 79:
#line 743 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EDIV, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 80:
#line 745 "parse.y"
{ yyval.tree_expression_type = make_binary_op (LEFTDIV, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 81:
#line 747 "parse.y"
{ yyval.tree_expression_type = make_binary_op (ELEFTDIV, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 82:
#line 751 "parse.y"
{ yyval.tree_expression_type = finish_colon_expression (yyvsp[0].tree_colon_expression_type); ;
    break;}
case 83:
#line 755 "parse.y"
{ yyval.tree_colon_expression_type = new tree_colon_expression (yyvsp[0].tree_expression_type); ;
    break;}
case 84:
#line 757 "parse.y"
{
		    if (! (yyval.tree_colon_expression_type = yyvsp[-2].tree_colon_expression_type->append (yyvsp[0].tree_expression_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 85:
#line 764 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 86:
#line 766 "parse.y"
{ yyval.tree_expression_type = make_binary_op (LSHIFT, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 87:
#line 768 "parse.y"
{ yyval.tree_expression_type = make_binary_op (RSHIFT, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 88:
#line 770 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_LT, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 89:
#line 772 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_LE, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 90:
#line 774 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_EQ, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 91:
#line 776 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_GE, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 92:
#line 778 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_GT, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 93:
#line 780 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_NE, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 94:
#line 782 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_AND, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 95:
#line 784 "parse.y"
{ yyval.tree_expression_type = make_binary_op (EXPR_OR, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 96:
#line 786 "parse.y"
{ yyval.tree_expression_type = make_boolean_op (EXPR_AND_AND, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 97:
#line 788 "parse.y"
{ yyval.tree_expression_type = make_boolean_op (EXPR_OR_OR, yyvsp[-2].tree_expression_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 98:
#line 795 "parse.y"
{ yyval.tree_argument_list_type = new tree_argument_list (yyvsp[0].tree_expression_type); ;
    break;}
case 99:
#line 797 "parse.y"
{
		    yyval.tree_argument_list_type = yyvsp[-1].tree_argument_list_type;
		    lexer_flags.looking_at_matrix_or_assign_lhs = false;
		  ;
    break;}
case 100:
#line 804 "parse.y"
{ yyval.tree_expression_type = make_assign_op ('=', yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 101:
#line 806 "parse.y"
{ yyval.tree_expression_type = make_assign_op (ADD_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 102:
#line 808 "parse.y"
{ yyval.tree_expression_type = make_assign_op (SUB_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 103:
#line 810 "parse.y"
{ yyval.tree_expression_type = make_assign_op (MUL_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 104:
#line 812 "parse.y"
{ yyval.tree_expression_type = make_assign_op (DIV_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 105:
#line 814 "parse.y"
{ yyval.tree_expression_type = make_assign_op (LEFTDIV_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 106:
#line 816 "parse.y"
{ yyval.tree_expression_type = make_assign_op (LSHIFT_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 107:
#line 818 "parse.y"
{ yyval.tree_expression_type = make_assign_op (RSHIFT_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 108:
#line 820 "parse.y"
{ yyval.tree_expression_type = make_assign_op (EMUL_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 109:
#line 822 "parse.y"
{ yyval.tree_expression_type = make_assign_op (EDIV_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 110:
#line 824 "parse.y"
{ yyval.tree_expression_type = make_assign_op (ELEFTDIV_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 111:
#line 826 "parse.y"
{ yyval.tree_expression_type = make_assign_op (AND_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 112:
#line 828 "parse.y"
{ yyval.tree_expression_type = make_assign_op (OR_EQ, yyvsp[-2].tree_argument_list_type, yyvsp[-1].tok_val, yyvsp[0].tree_expression_type); ;
    break;}
case 113:
#line 832 "parse.y"
{ yyval.tree_index_expression_type = make_index_expression (yyvsp[-1].tree_identifier_type, yyvsp[0].tree_argument_list_type); ;
    break;}
case 114:
#line 836 "parse.y"
{
		    tree_constant *tmp = make_constant (TEXT, yyvsp[0].tok_val);
		    yyval.tree_argument_list_type = new tree_argument_list (tmp);
		  ;
    break;}
case 115:
#line 841 "parse.y"
{
		    tree_constant *tmp = make_constant (TEXT, yyvsp[0].tok_val);
		    yyvsp[-1].tree_argument_list_type->append (tmp);
		    yyval.tree_argument_list_type = yyvsp[-1].tree_argument_list_type;
		  ;
    break;}
case 116:
#line 849 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 117:
#line 851 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_index_expression_type; ;
    break;}
case 118:
#line 853 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 119:
#line 861 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_decl_command_type; ;
    break;}
case 120:
#line 863 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_command_type; ;
    break;}
case 121:
#line 865 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_command_type; ;
    break;}
case 122:
#line 867 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_command_type; ;
    break;}
case 123:
#line 869 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_command_type; ;
    break;}
case 124:
#line 871 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_command_type; ;
    break;}
case 125:
#line 873 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_plot_command_type; ;
    break;}
case 126:
#line 881 "parse.y"
{ yyval.tree_decl_command_type = make_decl_command (GLOBAL, yyvsp[-1].tok_val, yyvsp[0].tree_decl_init_list_type); ;
    break;}
case 127:
#line 883 "parse.y"
{ yyval.tree_decl_command_type = make_decl_command (STATIC, yyvsp[-1].tok_val, yyvsp[0].tree_decl_init_list_type); ;
    break;}
case 128:
#line 887 "parse.y"
{ yyval.tree_decl_init_list_type = new tree_decl_init_list (yyvsp[0].tree_decl_elt_type); ;
    break;}
case 129:
#line 889 "parse.y"
{
		    yyvsp[-1].tree_decl_init_list_type->append (yyvsp[0].tree_decl_elt_type);
		    yyval.tree_decl_init_list_type = yyvsp[-1].tree_decl_init_list_type;
		  ;
    break;}
case 130:
#line 896 "parse.y"
{ yyval.tree_decl_elt_type = new tree_decl_elt (yyvsp[0].tree_identifier_type); ;
    break;}
case 131:
#line 898 "parse.y"
{ yyval.tree_decl_elt_type = new tree_decl_elt (yyvsp[-2].tree_identifier_type, yyvsp[0].tree_expression_type); ;
    break;}
case 132:
#line 906 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_if_command_type; ;
    break;}
case 133:
#line 908 "parse.y"
{ yyval.tree_command_type = yyvsp[0].tree_switch_command_type; ;
    break;}
case 134:
#line 916 "parse.y"
{
		    if (! (yyval.tree_if_command_type = finish_if_command (yyvsp[-3].tok_val, yyvsp[-1].tree_if_command_list_type, yyvsp[0].tok_val, yyvsp[-2].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 135:
#line 923 "parse.y"
{ yyval.tree_if_command_list_type = yyvsp[0].tree_if_command_list_type; ;
    break;}
case 136:
#line 925 "parse.y"
{
		    yyvsp[-1].tree_if_command_list_type->append (yyvsp[0].tree_if_clause_type);
		    yyval.tree_if_command_list_type = yyvsp[-1].tree_if_command_list_type;
		  ;
    break;}
case 137:
#line 932 "parse.y"
{ yyval.tree_if_command_list_type = start_if_command (yyvsp[-2].tree_expression_type, yyvsp[0].tree_statement_list_type); ;
    break;}
case 138:
#line 934 "parse.y"
{
		    yyvsp[-1].tree_if_command_list_type->append (yyvsp[0].tree_if_clause_type);
		    yyval.tree_if_command_list_type = yyvsp[-1].tree_if_command_list_type;
		  ;
    break;}
case 139:
#line 941 "parse.y"
{ yyval.tree_if_clause_type = make_elseif_clause (yyvsp[-2].tree_expression_type, yyvsp[0].tree_statement_list_type, yyvsp[-4].comment_type); ;
    break;}
case 140:
#line 945 "parse.y"
{
		    yyval.tree_if_clause_type = new tree_if_clause (yyvsp[0].tree_statement_list_type, yyvsp[-2].comment_type);
		  ;
    break;}
case 141:
#line 955 "parse.y"
{
		    if (! (yyval.tree_switch_command_type = finish_switch_command (yyvsp[-5].tok_val, yyvsp[-3].tree_expression_type, yyvsp[-1].tree_switch_case_list_type, yyvsp[0].tok_val, yyvsp[-4].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 142:
#line 962 "parse.y"
{ yyval.tree_switch_case_list_type = yyvsp[0].tree_switch_case_list_type; ;
    break;}
case 143:
#line 964 "parse.y"
{
		    yyvsp[-1].tree_switch_case_list_type->append (yyvsp[0].tree_switch_case_type);
		    yyval.tree_switch_case_list_type = yyvsp[-1].tree_switch_case_list_type;
		  ;
    break;}
case 144:
#line 971 "parse.y"
{ yyval.tree_switch_case_list_type = new tree_switch_case_list (yyvsp[0].tree_switch_case_type); ;
    break;}
case 145:
#line 973 "parse.y"
{
		    yyvsp[-1].tree_switch_case_list_type->append (yyvsp[0].tree_switch_case_type);
		    yyval.tree_switch_case_list_type = yyvsp[-1].tree_switch_case_list_type;
		  ;
    break;}
case 146:
#line 980 "parse.y"
{ yyval.tree_switch_case_type = make_switch_case (yyvsp[-2].tree_expression_type, yyvsp[0].tree_statement_list_type, yyvsp[-4].comment_type); ;
    break;}
case 147:
#line 984 "parse.y"
{
		    yyval.tree_switch_case_type = new tree_switch_case (yyvsp[0].tree_statement_list_type, yyvsp[-2].comment_type);
		  ;
    break;}
case 148:
#line 994 "parse.y"
{
		    if (! (yyval.tree_command_type = make_while_command (yyvsp[-5].tok_val, yyvsp[-3].tree_expression_type, yyvsp[-1].tree_statement_list_type, yyvsp[0].tok_val, yyvsp[-4].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 149:
#line 999 "parse.y"
{
		    if (! (yyval.tree_command_type = make_do_until_command (yyvsp[-5].tok_val, yyvsp[-2].tree_statement_list_type, yyvsp[0].tree_expression_type, yyvsp[-4].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 150:
#line 1004 "parse.y"
{
		    if (! (yyval.tree_command_type = make_for_command (yyvsp[-7].tok_val, yyvsp[-5].tree_argument_list_type, yyvsp[-3].tree_expression_type, yyvsp[-1].tree_statement_list_type, yyvsp[0].tok_val, yyvsp[-6].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 151:
#line 1015 "parse.y"
{
		    if (! (yyval.tree_command_type = make_break_command (yyvsp[0].tok_val)))
		      ABORT_PARSE;
		  ;
    break;}
case 152:
#line 1020 "parse.y"
{
		    if (! (yyval.tree_command_type = make_continue_command (yyvsp[0].tok_val)))
		      ABORT_PARSE;
		  ;
    break;}
case 153:
#line 1025 "parse.y"
{
		    if (! (yyval.tree_command_type = make_return_command (yyvsp[0].tok_val)))
		      ABORT_PARSE;
		  ;
    break;}
case 154:
#line 1037 "parse.y"
{
		    if (! (yyval.tree_command_type = make_unwind_command (yyvsp[-8].tok_val, yyvsp[-5].tree_statement_list_type, yyvsp[-1].tree_statement_list_type, yyvsp[0].tok_val, yyvsp[-7].comment_type, yyvsp[-3].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 155:
#line 1043 "parse.y"
{
		    if (! (yyval.tree_command_type = make_try_command (yyvsp[-8].tok_val, yyvsp[-5].tree_statement_list_type, yyvsp[-1].tree_statement_list_type, yyvsp[0].tok_val, yyvsp[-7].comment_type, yyvsp[-3].comment_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 156:
#line 1054 "parse.y"
{ curr_sym_tab = global_sym_tab; ;
    break;}
case 157:
#line 1058 "parse.y"
{ curr_sym_tab = tmp_local_sym_tab; ;
    break;}
case 158:
#line 1062 "parse.y"
{ lexer_flags.looking_at_return_list = true; ;
    break;}
case 159:
#line 1066 "parse.y"
{ lexer_flags.parsed_function_name = true; ;
    break;}
case 160:
#line 1074 "parse.y"
{ lexer_flags.looking_at_parameter_list = true; ;
    break;}
case 161:
#line 1078 "parse.y"
{ lexer_flags.looking_at_parameter_list = false; ;
    break;}
case 162:
#line 1082 "parse.y"
{
		    lexer_flags.quote_is_transpose = false;
		    yyval.tree_parameter_list_type = 0;
		  ;
    break;}
case 163:
#line 1087 "parse.y"
{
		    lexer_flags.quote_is_transpose = false;
		    tree_parameter_list *tmp = new tree_parameter_list ();
		    tmp->mark_varargs_only ();
		    yyval.tree_parameter_list_type = tmp;
		  ;
    break;}
case 164:
#line 1094 "parse.y"
{
		    lexer_flags.quote_is_transpose = false;
		    yyvsp[-1].tree_parameter_list_type->mark_as_formal_parameters ();
		    yyval.tree_parameter_list_type = yyvsp[-1].tree_parameter_list_type;
		  ;
    break;}
case 165:
#line 1100 "parse.y"
{
		    lexer_flags.quote_is_transpose = false;
		    yyvsp[-3].tree_parameter_list_type->mark_as_formal_parameters ();
		    yyvsp[-3].tree_parameter_list_type->mark_varargs ();
		    yyval.tree_parameter_list_type = yyvsp[-3].tree_parameter_list_type;
		  ;
    break;}
case 166:
#line 1109 "parse.y"
{ yyval.tree_parameter_list_type = new tree_parameter_list (yyvsp[0].tree_identifier_type); ;
    break;}
case 167:
#line 1111 "parse.y"
{
		    yyvsp[-2].tree_parameter_list_type->append (yyvsp[0].tree_identifier_type);
		    yyval.tree_parameter_list_type = yyvsp[-2].tree_parameter_list_type;
		  ;
    break;}
case 168:
#line 1116 "parse.y"
{
		    yyerror ("invalid parameter list");
		    yyval.tree_parameter_list_type = 0;
		    ABORT_PARSE;
		  ;
    break;}
case 169:
#line 1122 "parse.y"
{
		    yyerror ("invalid parameter list");
		    yyval.tree_parameter_list_type = 0;
		    ABORT_PARSE;
		  ;
    break;}
case 171:
#line 1137 "parse.y"
{
		    lexer_flags.looking_at_return_list = false;
		    yyval.tree_parameter_list_type = new tree_parameter_list ();
		  ;
    break;}
case 172:
#line 1142 "parse.y"
{
		    lexer_flags.looking_at_return_list = false;
		    tree_parameter_list *tmp = new tree_parameter_list ();
		    tmp->mark_varargs_only ();
		    yyval.tree_parameter_list_type = tmp;
		  ;
    break;}
case 173:
#line 1149 "parse.y"
{
		    lexer_flags.looking_at_return_list = false;
		    yyval.tree_parameter_list_type = yyvsp[-1].tree_parameter_list_type;
		  ;
    break;}
case 174:
#line 1154 "parse.y"
{
		    lexer_flags.looking_at_return_list = false;
		    yyvsp[-3].tree_parameter_list_type->mark_varargs ();
		    yyval.tree_parameter_list_type = yyvsp[-3].tree_parameter_list_type;
		  ;
    break;}
case 175:
#line 1162 "parse.y"
{ yyval.tree_parameter_list_type = new tree_parameter_list (yyvsp[0].tree_identifier_type); ;
    break;}
case 176:
#line 1164 "parse.y"
{
		    yyvsp[-2].tree_parameter_list_type->append (yyvsp[0].tree_identifier_type);
		    yyval.tree_parameter_list_type = yyvsp[-2].tree_parameter_list_type;
		  ;
    break;}
case 178:
#line 1178 "parse.y"
{ yyval.comment_type = yyvsp[-1].comment_type; ;
    break;}
case 179:
#line 1182 "parse.y"
{
		    yyvsp[0].octave_user_function_type->stash_leading_comment (yyvsp[-1].comment_type);
		    recover_from_parsing_function ();
		    yyval.tree_command_type = 0;
		  ;
    break;}
case 180:
#line 1188 "parse.y"
{
		    finish_function (yyvsp[-1].tree_identifier_type, yyvsp[0].octave_user_function_type, yyvsp[-2].comment_type);
		    recover_from_parsing_function ();
		    yyval.tree_command_type = 0;
		  ;
    break;}
case 181:
#line 1194 "parse.y"
{
		    finish_function (yyvsp[-1].tree_parameter_list_type, yyvsp[0].octave_user_function_type, yyvsp[-2].comment_type);
		    recover_from_parsing_function ();
		    yyval.tree_command_type = 0;
		  ;
    break;}
case 182:
#line 1202 "parse.y"
{ yyval.octave_user_function_type = yyvsp[0].octave_user_function_type; ;
    break;}
case 183:
#line 1206 "parse.y"
{
		    if (! (yyval.octave_user_function_type = frob_function (yyvsp[-3].tree_identifier_type, yyvsp[0].octave_user_function_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 184:
#line 1213 "parse.y"
{ yyval.octave_user_function_type = start_function (yyvsp[-1].tree_parameter_list_type, yyvsp[0].tree_statement_list_type); ;
    break;}
case 185:
#line 1215 "parse.y"
{ yyval.octave_user_function_type = start_function (0, yyvsp[0].tree_statement_list_type); ;
    break;}
case 186:
#line 1219 "parse.y"
{ yyval.tree_statement_list_type = yyvsp[-1].tree_statement_list_type; ;
    break;}
case 187:
#line 1223 "parse.y"
{
		    if (end_token_ok (yyvsp[0].tok_val, token::function_end))
		      {
			if (reading_fcn_file)
			  check_for_garbage_after_fcn_def ();
		      }
		    else
		      ABORT_PARSE;
		  ;
    break;}
case 188:
#line 1233 "parse.y"
{
		    if (! (reading_fcn_file || reading_script_file))
		      YYABORT;
		  ;
    break;}
case 189:
#line 1244 "parse.y"
{
		    if (! (yyval.tree_plot_command_type = make_plot_command (yyvsp[0].tok_val, 0, 0)))
		      ABORT_PARSE;
		  ;
    break;}
case 190:
#line 1249 "parse.y"
{
		    if (! (yyval.tree_plot_command_type = make_plot_command (yyvsp[-1].tok_val, yyvsp[0].plot_limits_type, 0)))
		      ABORT_PARSE;
		  ;
    break;}
case 191:
#line 1254 "parse.y"
{
		    if (! (yyval.tree_plot_command_type = make_plot_command (yyvsp[-1].tok_val, 0, yyvsp[0].subplot_list_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 192:
#line 1259 "parse.y"
{
		    if (! (yyval.tree_plot_command_type = make_plot_command (yyvsp[-2].tok_val, yyvsp[-1].plot_limits_type, yyvsp[0].subplot_list_type)))
		      ABORT_PARSE;
		  ;
    break;}
case 193:
#line 1266 "parse.y"
{ yyval.plot_limits_type = new plot_limits (yyvsp[0].plot_range_type); ;
    break;}
case 194:
#line 1268 "parse.y"
{ yyval.plot_limits_type = new plot_limits (yyvsp[-1].plot_range_type, yyvsp[0].plot_range_type); ;
    break;}
case 195:
#line 1270 "parse.y"
{ yyval.plot_limits_type = new plot_limits (yyvsp[-2].plot_range_type, yyvsp[-1].plot_range_type, yyvsp[0].plot_range_type); ;
    break;}
case 196:
#line 1274 "parse.y"
{ yyval.plot_range_type = new plot_range (yyvsp[-3].tree_expression_type, yyvsp[-1].tree_expression_type); ;
    break;}
case 197:
#line 1276 "parse.y"
{ yyval.plot_range_type = new plot_range (0, yyvsp[-1].tree_expression_type); ;
    break;}
case 198:
#line 1278 "parse.y"
{ yyval.plot_range_type = new plot_range (yyvsp[-2].tree_expression_type, 0); ;
    break;}
case 199:
#line 1280 "parse.y"
{ yyval.plot_range_type = new plot_range (); ;
    break;}
case 200:
#line 1282 "parse.y"
{ yyval.plot_range_type = new plot_range (); ;
    break;}
case 201:
#line 1286 "parse.y"
{ yyval.subplot_list_type = new subplot_list (yyvsp[0].subplot_type); ;
    break;}
case 202:
#line 1288 "parse.y"
{
		    yyvsp[-2].subplot_list_type->append (yyvsp[0].subplot_type);
		    yyval.subplot_list_type = yyvsp[-2].subplot_list_type;
		  ;
    break;}
case 203:
#line 1295 "parse.y"
{ yyval.subplot_type = new subplot (yyvsp[0].tree_expression_type); ;
    break;}
case 204:
#line 1297 "parse.y"
{ yyval.subplot_type = yyvsp[0].subplot_type->add_data (yyvsp[-1].tree_expression_type); ;
    break;}
case 205:
#line 1301 "parse.y"
{
		    subplot *tmp = new subplot ();
		    yyval.subplot_type = tmp->add_clause (yyvsp[0].subplot_using_type);
		  ;
    break;}
case 206:
#line 1306 "parse.y"
{
		    subplot *tmp = new subplot ();
		    yyval.subplot_type = tmp->add_clause (yyvsp[0].tree_expression_type);
		  ;
    break;}
case 207:
#line 1311 "parse.y"
{
		    subplot *tmp = new subplot ();
		    yyval.subplot_type = tmp->add_clause (yyvsp[0].subplot_style_type);
		  ;
    break;}
case 208:
#line 1316 "parse.y"
{
		    subplot *tmp = new subplot ();
		    yyval.subplot_type = tmp->add_clause (yyvsp[0].subplot_axes_type);
		  ;
    break;}
case 209:
#line 1321 "parse.y"
{
		    if (! (yyval.subplot_type = yyvsp[-1].subplot_type->add_clause (yyvsp[0].subplot_using_type)))
		      {
			yyerror ("only one using option may be specified");
			ABORT_PARSE;
		      }
		  ;
    break;}
case 210:
#line 1329 "parse.y"
{
		    if (! (yyval.subplot_type = yyvsp[-1].subplot_type->add_clause (yyvsp[0].tree_expression_type)))
		      {
			yyerror ("only one title option my be specified");
			ABORT_PARSE;
		      }
		  ;
    break;}
case 211:
#line 1337 "parse.y"
{
		    if (! (yyval.subplot_type = yyvsp[-1].subplot_type->add_clause (yyvsp[0].subplot_style_type)))
		      {
			yyerror ("only one style option my be specified");
			ABORT_PARSE;
		      }
		  ;
    break;}
case 212:
#line 1345 "parse.y"
{
		    if (! (yyval.subplot_type = yyvsp[-1].subplot_type->add_clause (yyvsp[0].subplot_axes_type)))
		      {
			yyerror ("only one axes option may be specified");
			ABORT_PARSE;
		      }
		  ;
    break;}
case 213:
#line 1355 "parse.y"
{
		    lexer_flags.in_plot_axes = false;
		    yyval.subplot_axes_type = new subplot_axes (yyvsp[0].tok_val->text ());
		  ;
    break;}
case 214:
#line 1362 "parse.y"
{
		    lexer_flags.in_plot_using = false;
		    yyval.subplot_using_type = yyvsp[0].subplot_using_type;
		  ;
    break;}
case 215:
#line 1367 "parse.y"
{
		    lexer_flags.in_plot_using = false;
		    yyval.subplot_using_type = yyvsp[-1].subplot_using_type->set_format (yyvsp[0].tree_expression_type);
		  ;
    break;}
case 216:
#line 1374 "parse.y"
{
		    subplot_using *tmp = new subplot_using ();
		    yyval.subplot_using_type = tmp->add_qualifier (yyvsp[0].tree_expression_type);
		  ;
    break;}
case 217:
#line 1379 "parse.y"
{ yyval.subplot_using_type = yyvsp[-2].subplot_using_type->add_qualifier (yyvsp[0].tree_expression_type); ;
    break;}
case 218:
#line 1383 "parse.y"
{ yyval.tree_expression_type = yyvsp[0].tree_expression_type; ;
    break;}
case 219:
#line 1387 "parse.y"
{ yyval.subplot_style_type = new subplot_style (yyvsp[0].tok_val->text ()); ;
    break;}
case 220:
#line 1389 "parse.y"
{ yyval.subplot_style_type = new subplot_style (yyvsp[-1].tok_val->text (), yyvsp[0].tree_expression_type); ;
    break;}
case 221:
#line 1391 "parse.y"
{ yyval.subplot_style_type = new subplot_style (yyvsp[-2].tok_val->text (), yyvsp[-1].tree_expression_type, yyvsp[0].tree_expression_type); ;
    break;}
case 222:
#line 1399 "parse.y"
{ yyval.comment_type = octave_comment_buffer::get_comment (); ;
    break;}
case 223:
#line 1403 "parse.y"
{ yyerror ("parse error"); ;
    break;}
case 225:
#line 1408 "parse.y"
{ yyval.sep_type = ','; ;
    break;}
case 226:
#line 1410 "parse.y"
{ yyval.sep_type = ';'; ;
    break;}
case 227:
#line 1412 "parse.y"
{ yyval.sep_type = yyvsp[-1].sep_type; ;
    break;}
case 228:
#line 1414 "parse.y"
{ yyval.sep_type = yyvsp[-1].sep_type; ;
    break;}
case 229:
#line 1418 "parse.y"
{ yyval.sep_type = 0; ;
    break;}
case 230:
#line 1420 "parse.y"
{ yyval.sep_type = yyvsp[0].sep_type; ;
    break;}
case 231:
#line 1424 "parse.y"
{ yyval.sep_type = ','; ;
    break;}
case 232:
#line 1426 "parse.y"
{ yyval.sep_type = ';'; ;
    break;}
case 233:
#line 1428 "parse.y"
{ yyval.sep_type = '\n'; ;
    break;}
case 234:
#line 1430 "parse.y"
{ yyval.sep_type = yyvsp[-1].sep_type; ;
    break;}
case 235:
#line 1432 "parse.y"
{ yyval.sep_type = yyvsp[-1].sep_type; ;
    break;}
case 236:
#line 1434 "parse.y"
{ yyval.sep_type = yyvsp[-1].sep_type; ;
    break;}
case 237:
#line 1438 "parse.y"
{ yyval.sep_type = 0; ;
    break;}
case 238:
#line 1440 "parse.y"
{ yyval.sep_type = yyvsp[0].sep_type; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 1443 "parse.y"


// Generic error messages.

static void
yyerror (const char *s)
{
  int err_col = current_input_column - 1;

  std::ostrstream output_buf;

  if (reading_fcn_file || reading_script_file)
    output_buf << "parse error near line " << input_line_number
	       << " of file " << curr_fcn_file_full_name;
  else
    output_buf << "parse error:";

  if (s && strcmp (s, "parse error") != 0)
    output_buf << "\n\n  " << s;

  output_buf << "\n\n";

  if (! current_input_line.empty ())
    {
      size_t len = current_input_line.length ();

      if (current_input_line[len-1] == '\n')
        current_input_line.resize (len-1);

// Print the line, maybe with a pointer near the error token.

      output_buf << ">>> " << current_input_line << "\n";

      if (err_col == 0)
	err_col = len;

      for (int i = 0; i < err_col + 3; i++)
	output_buf << " ";

      output_buf << "^";
    }

  output_buf << "\n" << std::ends;

  char *msg = output_buf.str ();

  parse_error ("%s", msg);

  delete [] msg;
}

// Error mesages for mismatched end tokens.

static void
end_error (const char *type, token::end_tok_type ettype, int l, int c)
{
  static const char *fmt
    = "`%s' command matched by `%s' near line %d column %d";

  switch (ettype)
    {
    case token::simple_end:
      error (fmt, type, "end", l, c);
      break;

    case token::for_end:
      error (fmt, type, "endfor", l, c);
      break;

    case token::function_end:
      error (fmt, type, "endfunction", l, c);
      break;

    case token::if_end:
      error (fmt, type, "endif", l, c);
      break;

    case token::switch_end:
      error (fmt, type, "endswitch", l, c); 
      break;

    case token::while_end:
      error (fmt, type, "endwhile", l, c); 
      break;

    case token::unwind_protect_end:
      error (fmt, type, "end_unwind_protect", l, c); 
      break;

    default:
      panic_impossible ();
      break;
    }
}

// Check to see that end tokens are properly matched.

static bool
end_token_ok (token *tok, token::end_tok_type expected)
{
  bool retval = true;

  token::end_tok_type ettype = tok->ettype ();

  if (ettype != expected && ettype != token::simple_end)
    {
      retval = false;

      yyerror ("parse error");

      int l = tok->line ();
      int c = tok->column ();

      switch (expected)
	{
	case token::for_end:
	  end_error ("for", ettype, l, c);
	  break;

	case token::function_end:
	  end_error ("function", ettype, l, c);
	  break;

	case token::if_end:
	  end_error ("if", ettype, l, c);
	  break;

	case token::try_catch_end:
	  end_error ("try", ettype, l, c);
	  break;

	case token::switch_end:
	  end_error ("switch", ettype, l, c);
	  break;

	case token::unwind_protect_end:
	  end_error ("unwind_protect", ettype, l, c);
	  break;

	case token::while_end:
	  end_error ("while", ettype, l, c);
	  break;

	default:
	  panic_impossible ();
	  break;
	}
    }

  return retval;
}

// Maybe print a warning if an assignment expression is used as the
// test in a logical expression.

static void
maybe_warn_assign_as_truth_value (tree_expression *expr)
{
  if (Vwarn_assign_as_truth_value
      && expr->is_assignment_expression ()
      && expr->paren_count () < 2)
    {
      warning ("suggest parenthesis around assignment used as truth value");
    }
}

// Maybe print a warning about switch labels that aren't constants.

static void
maybe_warn_variable_switch_label (tree_expression *expr)
{
  if (Vwarn_variable_switch_label && ! expr->is_constant ())
    {
      warning ("variable switch label");
    }
}

// Create a plot command.

static tree_plot_command *
make_plot_command (token *tok, plot_limits *range, subplot_list *list)
{
  if (range)
    {
      if (tok->pttype () == token::replot)
	{
	  yyerror ("cannot specify new ranges with replot");
	  return 0;
	}
    }
  else if (! list && tok->pttype () != token::replot)
    {
      yyerror ("must have something to plot");
      return 0;
    }

  lexer_flags.plotting = false;
  lexer_flags.past_plot_range = false;
  lexer_flags.in_plot_range = false;
  lexer_flags.in_plot_using = false;
  lexer_flags.in_plot_style = false;
  
  return new tree_plot_command (list, range, tok->pttype ());
}

static tree_expression *
fold (tree_binary_expression *e)
{
  tree_expression *retval = e;

  unwind_protect::begin_frame ("fold_binary_expression");

  unwind_protect_int (error_state);

  unwind_protect_bool (discard_error_messages);
  discard_error_messages = true;

  tree_expression *op1 = e->lhs ();
  tree_expression *op2 = e->rhs ();

  if (op1->is_constant () && op2->is_constant ())
    {
      octave_value tmp = e->rvalue ();

      if (! (error_state || warning_state))
	{
	  tree_constant *tc_retval = new tree_constant (tmp);

	  std::ostrstream buf;

	  tree_print_code tpc (buf);

	  e->accept (tpc);

	  buf << std::ends;

	  char *s = buf.str ();

	  tc_retval->stash_original_text (s);

	  delete [] s;

	  delete e;

	  retval = tc_retval;
	}
    }

  unwind_protect::run_frame ("fold_binary_expression");

  return retval;
}

static tree_expression *
fold (tree_unary_expression *e)
{
  tree_expression *retval = e;

  unwind_protect::begin_frame ("fold_unary_expression");

  unwind_protect_int (error_state);

  unwind_protect_bool (discard_error_messages);
  discard_error_messages = true;

  tree_expression *op = e->operand ();

  if (op->is_constant ())
    {
      octave_value tmp = e->rvalue ();

      if (! (error_state || warning_state))
	{
	  tree_constant *tc_retval = new tree_constant (tmp);

	  std::ostrstream buf;

	  tree_print_code tpc (buf);

	  e->accept (tpc);

	  buf << std::ends;

	  char *s = buf.str ();

	  tc_retval->stash_original_text (s);

	  delete [] s;

	  delete e;

	  retval = tc_retval;
	}
    }

  unwind_protect::run_frame ("fold_unary_expression");

  return retval;
}

// Finish building a range.

static tree_expression *
finish_colon_expression (tree_colon_expression *e)
{
  tree_expression *retval = e;

  unwind_protect::begin_frame ("finish_colon_expression");

  unwind_protect_int (error_state);

  unwind_protect_bool (discard_error_messages);
  discard_error_messages = true;

  tree_expression *base = e->base ();
  tree_expression *limit = e->limit ();
  tree_expression *incr = e->increment ();

  if (base)
    {
      if (limit)
	{
	  if (base->is_constant () && limit->is_constant ()
	      && (! incr || (incr && incr->is_constant ())))
	    {
	      octave_value tmp = e->rvalue ();

	      if (! (error_state || warning_state))
		{
		  tree_constant *tc_retval = new tree_constant (tmp);

		  std::ostrstream buf;

		  tree_print_code tpc (buf);

		  e->accept (tpc);

		  buf << std::ends;

		  char *s = buf.str ();

		  tc_retval->stash_original_text (s);

		  delete [] s;

		  delete e;

		  retval = tc_retval;
		}
	    }
	}
      else
	{
	  e->preserve_base ();
	  delete e;

	  // XXX FIXME XXX -- need to attempt constant folding here
	  // too (we need a generic way to do that).
	  retval = base;
	}
    }

  unwind_protect::run_frame ("finish_colon_expression");

  return retval;
}

// Make a constant.

static tree_constant *
make_constant (int op, token *tok_val)
{
  int l = tok_val->line ();
  int c = tok_val->column ();

  tree_constant *retval = 0;

  switch (op)
    {
    case NUM:
      {
	octave_value tmp (tok_val->number ());
	retval = new tree_constant (tmp, l, c);
	retval->stash_original_text (tok_val->text_rep ());
      }
      break;

    case IMAG_NUM:
      {
	octave_value tmp (Complex (0.0, tok_val->number ()));
	retval = new tree_constant (tmp, l, c);
	retval->stash_original_text (tok_val->text_rep ());
      }
      break;

    case TEXT:
      {
	octave_value tmp (tok_val->text ());
	retval = new tree_constant (tmp, l, c);
      }
      break;

    default:
      panic_impossible ();
      break;
    }

  return retval;
}

// Build a binary expression.

static tree_expression *
make_binary_op (int op, tree_expression *op1, token *tok_val,
		tree_expression *op2)
{
  octave_value::binary_op t = octave_value::unknown_binary_op;

  switch (op)
    {
    case POW:
      t = octave_value::op_pow;
      break;

    case EPOW:
      t = octave_value::op_el_pow;
      break;

    case '+':
      t = octave_value::op_add;
      break;

    case '-':
      t = octave_value::op_sub;
      break;

    case '*':
      t = octave_value::op_mul;
      break;

    case '/':
      t = octave_value::op_div;
      break;

    case EMUL:
      t = octave_value::op_el_mul;
      break;

    case EDIV:
      t = octave_value::op_el_div;
      break;

    case LEFTDIV:
      t = octave_value::op_ldiv;
      break;

    case ELEFTDIV:
      t = octave_value::op_el_ldiv;
      break;

    case LSHIFT:
      t = octave_value::op_lshift;
      break;

    case RSHIFT:
      t = octave_value::op_rshift;
      break;

    case EXPR_LT:
      t = octave_value::op_lt;
      break;

    case EXPR_LE:
      t = octave_value::op_le;
      break;

    case EXPR_EQ:
      t = octave_value::op_eq;
      break;

    case EXPR_GE:
      t = octave_value::op_ge;
      break;

    case EXPR_GT:
      t = octave_value::op_gt;
      break;

    case EXPR_NE:
      t = octave_value::op_ne;
      break;

    case EXPR_AND:
      t = octave_value::op_el_and;
      break;

    case EXPR_OR:
      t = octave_value::op_el_or;
      break;

    default:
      panic_impossible ();
      break;
    }

  int l = tok_val->line ();
  int c = tok_val->column ();

  tree_binary_expression *e
    = new tree_binary_expression (op1, op2, l, c, t);

  return fold (e);
}

// Build a boolean expression.

static tree_expression *
make_boolean_op (int op, tree_expression *op1, token *tok_val,
		 tree_expression *op2)
{
  tree_boolean_expression::type t;

  switch (op)
    {
    case EXPR_AND_AND:
      t = tree_boolean_expression::bool_and;
      break;

    case EXPR_OR_OR:
      t = tree_boolean_expression::bool_or;
      break;

    default:
      panic_impossible ();
      break;
    }

  int l = tok_val->line ();
  int c = tok_val->column ();

  tree_boolean_expression *e
    = new tree_boolean_expression (op1, op2, l, c, t);

  return fold (e);
}

// Build a prefix expression.

static tree_expression *
make_prefix_op (int op, tree_expression *op1, token *tok_val)
{
  octave_value::unary_op t = octave_value::unknown_unary_op;

  switch (op)
    {
    case EXPR_NOT:
      t = octave_value::op_not;
      break;

    case '-':
      t = octave_value::op_uminus;
      break;

    case PLUS_PLUS:
      t = octave_value::op_incr;
      break;

    case MINUS_MINUS:
      t = octave_value::op_decr;
      break;

    default:
      panic_impossible ();
      break;
    }

  int l = tok_val->line ();
  int c = tok_val->column ();

  tree_prefix_expression *e
    = new tree_prefix_expression (op1, l, c, t);

  return fold (e);
}

// Build a postfix expression.

static tree_expression *
make_postfix_op (int op, tree_expression *op1, token *tok_val)
{
  octave_value::unary_op t = octave_value::unknown_unary_op;

  switch (op)
    {
    case QUOTE:
      t = octave_value::op_hermitian;
      break;

    case TRANSPOSE:
      t = octave_value::op_transpose;
      break;

    case PLUS_PLUS:
      t = octave_value::op_incr;
      break;

    case MINUS_MINUS:
      t = octave_value::op_decr;
      break;

    default:
      panic_impossible ();
      break;
    }

  int l = tok_val->line ();
  int c = tok_val->column ();

  tree_postfix_expression *e
    = new tree_postfix_expression (op1, l, c, t);

  return fold (e);
}

// Build an unwind-protect command.

static tree_command *
make_unwind_command (token *unwind_tok, tree_statement_list *body,
		     tree_statement_list *cleanup, token *end_tok,
		     octave_comment_list *lc, octave_comment_list *mc)
{
  tree_command *retval = 0;

  if (end_token_ok (end_tok, token::unwind_protect_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      int l = unwind_tok->line ();
      int c = unwind_tok->column ();

      retval = new tree_unwind_protect_command (body, cleanup,
						lc, mc, tc, l, c);
    }

  return retval;
}

// Build a try-catch command.

static tree_command *
make_try_command (token *try_tok, tree_statement_list *body,
		  tree_statement_list *cleanup, token *end_tok,
		  octave_comment_list *lc, octave_comment_list *mc)
{
  tree_command *retval = 0;

  if (end_token_ok (end_tok, token::try_catch_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      int l = try_tok->line ();
      int c = try_tok->column ();

      retval = new tree_try_catch_command (body, cleanup,
					   lc, mc, tc, l, c);
    }

  return retval;
}

// Build a while command.

static tree_command *
make_while_command (token *while_tok, tree_expression *expr,
		    tree_statement_list *body, token *end_tok,
		    octave_comment_list *lc)
{
  tree_command *retval = 0;

  maybe_warn_assign_as_truth_value (expr);

  if (end_token_ok (end_tok, token::while_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      lexer_flags.looping--;

      int l = while_tok->line ();
      int c = while_tok->column ();

      retval = new tree_while_command (expr, body, lc, tc, l, c);
    }

  return retval;
}

// Build a do-until command.

static tree_command *
make_do_until_command (token *do_tok, tree_statement_list *body,
		       tree_expression *expr, octave_comment_list *lc)
{
  tree_command *retval = 0;

  maybe_warn_assign_as_truth_value (expr);

  octave_comment_list *tc = octave_comment_buffer::get_comment ();

  lexer_flags.looping--;

  int l = do_tok->line ();
  int c = do_tok->column ();

  retval = new tree_do_until_command (expr, body, lc, tc, l, c);

  return retval;
}

// Build a for command.

static tree_command *
make_for_command (token *for_tok, tree_argument_list *lhs,
		  tree_expression *expr, tree_statement_list *body,
		  token *end_tok, octave_comment_list *lc)
{
  tree_command *retval = 0;

  if (end_token_ok (end_tok, token::for_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      lexer_flags.looping--;

      int l = for_tok->line ();
      int c = for_tok->column ();

      if (lhs->length () == 1)
	{
	  tree_expression *tmp = lhs->remove_front ();

	  retval = new tree_simple_for_command (tmp, expr, body,
						lc, tc, l, c);

	  delete lhs;
	}
      else
	retval = new tree_complex_for_command (lhs, expr, body,
					       lc, tc, l, c);
    }

  return retval;
}

// Build a break command.

static tree_command *
make_break_command (token *break_tok)
{
  tree_command *retval = 0;

  int l = break_tok->line ();
  int c = break_tok->column ();

  if (lexer_flags.looping || lexer_flags.defining_func
      || reading_script_file || evaluating_function_body)
    retval = new tree_break_command (l, c);
  else
    retval = new tree_no_op_command ("break", l, c);

  return retval;
}

// Build a continue command.

static tree_command *
make_continue_command (token *continue_tok)
{
  tree_command *retval = 0;

  int l = continue_tok->line ();
  int c = continue_tok->column ();

  if (lexer_flags.looping)
    retval = new tree_continue_command (l, c);
  else
    retval = new tree_no_op_command ("continue", l, c);

  return retval;
}

// Build a return command.

static tree_command *
make_return_command (token *return_tok)
{
  tree_command *retval = 0;

  int l = return_tok->line ();
  int c = return_tok->column ();

  if (lexer_flags.defining_func || reading_script_file
      || evaluating_function_body)
    retval = new tree_return_command (l, c);
  else
    retval = new tree_no_op_command ("return", l, c);

  return retval;
}

// Start an if command.

static tree_if_command_list *
start_if_command (tree_expression *expr, tree_statement_list *list)
{
  maybe_warn_assign_as_truth_value (expr);

  tree_if_clause *t = new tree_if_clause (expr, list);

  return new tree_if_command_list (t);
}

// Finish an if command.

static tree_if_command *
finish_if_command (token *if_tok, tree_if_command_list *list,
		   token *end_tok, octave_comment_list *lc)
{
  tree_if_command *retval = 0;

  if (end_token_ok (end_tok, token::if_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      int l = if_tok->line ();
      int c = if_tok->column ();

      retval = new tree_if_command (list, lc, tc, l, c);
    }

  return retval;
}

// Build an elseif clause.

static tree_if_clause *
make_elseif_clause (tree_expression *expr, tree_statement_list *list,
		    octave_comment_list *lc)
{
  maybe_warn_assign_as_truth_value (expr);

  return new tree_if_clause (expr, list, lc);
}

// Finish a switch command.

static tree_switch_command *
finish_switch_command (token *switch_tok, tree_expression *expr,
		       tree_switch_case_list *list, token *end_tok,
		       octave_comment_list *lc)
{
  tree_switch_command *retval = 0;

  if (end_token_ok (end_tok, token::switch_end))
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      int l = switch_tok->line ();
      int c = switch_tok->column ();

      retval = new tree_switch_command (expr, list, lc, tc, l, c);
    }

  return retval;
}

// Build a switch case.

static tree_switch_case *
make_switch_case (tree_expression *expr, tree_statement_list *list,
		  octave_comment_list *lc)
{
  maybe_warn_variable_switch_label (expr);

  return new tree_switch_case (expr, list, lc);
}

// Build an assignment to a variable.

static tree_expression *
make_assign_op (int op, tree_argument_list *lhs, token *eq_tok,
		tree_expression *rhs)
{
  tree_expression *retval = 0;

  octave_value::assign_op t = octave_value::unknown_assign_op;

  switch (op)
    {
    case '=':
      t = octave_value::op_asn_eq;
      break;

    case ADD_EQ:
      t = octave_value::op_add_eq;
      break;

    case SUB_EQ:
      t = octave_value::op_sub_eq;
      break;

    case MUL_EQ:
      t = octave_value::op_mul_eq;
      break;

    case DIV_EQ:
      t = octave_value::op_div_eq;
      break;

    case LEFTDIV_EQ:
      t = octave_value::op_ldiv_eq;
      break;

    case LSHIFT_EQ:
      t = octave_value::op_lshift_eq;
      break;

    case RSHIFT_EQ:
      t = octave_value::op_rshift_eq;
      break;

    case EMUL_EQ:
      t = octave_value::op_el_mul_eq;
      break;

    case EDIV_EQ:
      t = octave_value::op_el_div_eq;
      break;

    case ELEFTDIV_EQ:
      t = octave_value::op_el_ldiv_eq;
      break;

    case AND_EQ:
      t = octave_value::op_el_and_eq;
      break;

    case OR_EQ:
      t = octave_value::op_el_or_eq;
      break;

    default:
      panic_impossible ();
      break;
    }

  int l = eq_tok->line ();
  int c = eq_tok->column ();

  if (lhs->length () == 1)
    {
      tree_expression *tmp = lhs->remove_front ();

      retval = new tree_simple_assignment (tmp, rhs, false, l, c, t);

      delete lhs;
    }
  else
    return new tree_multi_assignment (lhs, rhs, false, l, c, t);

  return retval;
}

// Begin defining a function.

static octave_user_function *
start_function (tree_parameter_list *param_list, tree_statement_list *body)
{
  body->mark_as_function_body ();

  // We'll fill in the return list later.

  octave_user_function *fcn
    = new octave_user_function (param_list, 0, body, curr_sym_tab);

  if (fcn)
    {
      octave_comment_list *tc = octave_comment_buffer::get_comment ();

      fcn->stash_trailing_comment (tc);
    }

  return fcn;
}

// Do most of the work for defining a function.

static octave_user_function *
frob_function (tree_identifier *id, octave_user_function *fcn)
{
  std::string id_name = id->name ();

  // If input is coming from a file, issue a warning if the name of
  // the file does not match the name of the function stated in the
  // file.  Matlab doesn't provide a diagnostic (it ignores the stated
  // name).

  fcn->stash_function_name (id_name);

  if (reading_fcn_file)
    {
      if (curr_fcn_file_name != id_name)
	{
	  if (Vwarn_function_name_clash)
	    warning ("function name `%s' does not agree with function\
 file name `%s'", id_name.c_str (), curr_fcn_file_full_name.c_str ());

	  global_sym_tab->rename (id_name, curr_fcn_file_name);

	  if (error_state)
	    return 0;

	  id_name = id->name ();
	}

      octave_time now;

      fcn->stash_function_name (id_name);
      fcn->stash_fcn_file_name ();
      fcn->stash_fcn_file_time (now);
      fcn->mark_as_system_fcn_file ();

      if (Vwarn_future_time_stamp)
	{
	  std::string nm = fcn->fcn_file_name ();

	  file_stat fs (nm);

	  if (fs && fs.is_newer (now))
	    warning ("time stamp for `%s' is in the future", nm.c_str ());
	}
    }
  else if (! (input_from_tmp_history_file || input_from_startup_file)
	   && reading_script_file
	   && curr_fcn_file_name == id_name)
    {
      warning ("function `%s' defined within script file `%s'",
	       id_name.c_str (), curr_fcn_file_full_name.c_str ());
    }

  top_level_sym_tab->clear (id_name);

  symbol_record *sr = global_sym_tab->lookup (id_name);

  if (sr)
    fcn->stash_symtab_ptr (sr);
  else
    panic_impossible ();

  id->define (fcn, symbol_record::USER_FUNCTION);

  id->document (help_buf);

  help_buf.resize (0);

  return fcn;
}

// Finish defining a function.

static octave_user_function *
finish_function (tree_identifier *id, octave_user_function *fcn,
		 octave_comment_list *lc)
{
  tree_parameter_list *tpl = new tree_parameter_list (id);

  tpl->mark_as_formal_parameters ();

  fcn->stash_leading_comment (lc);

  return fcn->define_ret_list (tpl);
}

// Finish defining a function a different way.

static octave_user_function *
finish_function (tree_parameter_list *ret_list,
		 octave_user_function *fcn, octave_comment_list *lc)
{
  ret_list->mark_as_formal_parameters ();

  fcn->stash_leading_comment (lc);

  return fcn->define_ret_list (ret_list);
}

static void
recover_from_parsing_function (void)
{
  curr_sym_tab = top_level_sym_tab;

  lexer_flags.defining_func = false;
  lexer_flags.beginning_of_function = false;
  lexer_flags.parsed_function_name = false;
  lexer_flags.looking_at_return_list = false;
  lexer_flags.looking_at_parameter_list = false;
}

// Make an index expression.

static tree_index_expression *
make_index_expression (tree_expression *expr, tree_argument_list *args)
{
  tree_index_expression *retval = 0;

  int l = expr->line ();
  int c = expr->column ();

  expr->mark_postfix_indexed ();

  retval =  new tree_index_expression (expr, args, l, c);

  return retval;
}

// Make an indirect reference expression.

static tree_indirect_ref *
make_indirect_ref (tree_expression *expr, const std::string& elt)
{
  tree_indirect_ref *retval = 0;

  int l = expr->line ();
  int c = expr->column ();

  retval = new tree_indirect_ref (expr, elt, l, c);

  lexer_flags.looking_at_indirect_ref = false;

  return retval;
}

// Make a declaration command.

static tree_decl_command *
make_decl_command (int tok, token *tok_val, tree_decl_init_list *lst)
{
  tree_decl_command *retval = 0;

  int l = tok_val->line ();
  int c = tok_val->column ();

  switch (tok)
    {
    case GLOBAL:
      retval = new tree_global_command (lst, l, c);
      break;

    case STATIC:
      if (lexer_flags.defining_func)
	retval = new tree_static_command (lst, l, c);
      else
	{
	  if (reading_script_file)
	    warning ("ignoring static declaration near line %d of file `%s'",
		     l, curr_fcn_file_full_name.c_str ());
	  else
	    warning ("ignoring static declaration near line %d", l);
	}
      break;

    default:
      panic_impossible ();
      break;
    }

  return retval;
}

// Finish building a matrix list.

static tree_expression *
finish_matrix (tree_matrix *m)
{
  tree_expression *retval = m;

  unwind_protect::begin_frame ("finish_matrix");

  unwind_protect_int (error_state);

  unwind_protect_bool (discard_error_messages);
  discard_error_messages = true;

  if (m->all_elements_are_constant ())
    {
      octave_value tmp = m->rvalue ();

      if (! (error_state || warning_state))
	{
	  tree_constant *tc_retval = new tree_constant (tmp);

	  std::ostrstream buf;

	  tree_print_code tpc (buf);

	  m->accept (tpc);

	  buf << std::ends;

	  char *s = buf.str ();

	  tc_retval->stash_original_text (s);

	  delete [] s;

	  delete m;

	  retval = tc_retval;
	}
    }

  unwind_protect::run_frame ("finish_matrix");

  return retval;
}

// Finish building a cell list.

static tree_expression *
finish_cell (tree_cell *c)
{
  // For now, this doesn't do anything.

  return c;
}

static void
maybe_warn_missing_semi (tree_statement_list *t)
{
  if (lexer_flags.defining_func && Vwarn_missing_semicolon)
    {
      tree_statement *tmp = t->rear();

      if (tmp->is_expression ())
	warning ("missing semicolon near line %d, column %d in file `%s'",
		 tmp->line (), tmp->column (),
		 curr_fcn_file_full_name.c_str ());
    }
}

static void
set_stmt_print_flag (tree_statement_list *list, char sep,
		     bool warn_missing_semi)
{
  switch (sep)
    {
    case ';':
      {
	tree_statement *tmp = list->rear ();
	tmp->set_print_flag (0);
      }
      break;

    case 0:
    case ',':
    case '\n':
      if (warn_missing_semi)
	maybe_warn_missing_semi (list);
      break;

    default:
      warning ("unrecognized separator type!");
      break;
    }
}

void
parse_and_execute (FILE *f)
{
  unwind_protect::begin_frame ("parse_and_execute");
  
  YY_BUFFER_STATE old_buf = current_buffer ();
  YY_BUFFER_STATE new_buf = create_buffer (f);

  unwind_protect::add (restore_input_buffer, old_buf);
  unwind_protect::add (delete_input_buffer, new_buf);

  switch_to_buffer (new_buf);

  unwind_protect_bool (line_editing);
  unwind_protect_bool (input_from_command_line_file);

  line_editing = false;
  input_from_command_line_file = false;

  unwind_protect_ptr (curr_sym_tab);

  int retval;
  do
    {
      reset_parser ();

      retval = yyparse ();

      if (retval == 0 && global_command)
	{
	  global_command->eval ();

	  delete global_command;

	  global_command = 0;

	  bool quit = (tree_return_command::returning
		       || tree_break_command::breaking);

	  if (tree_return_command::returning)
	    tree_return_command::returning = 0;

	  if (tree_break_command::breaking)
	    tree_break_command::breaking--;

	  if (error_state)
	    {
	      error ("near line %d of file `%s'", input_line_number,
		     curr_fcn_file_full_name.c_str ());

	      break;
	    }

	  if (quit)
	    break;
	}
    }
  while (retval == 0);

  unwind_protect::run_frame ("parse_and_execute");
}

static void
safe_fclose (void *f)
{
  // XXX FIXME XXX -- comments at the end of an input file are
  // discarded (otherwise, they would be appended to the next
  // statement, possibly from the command line or another file, which
  // can be quite confusing).

  octave_comment_buffer::get_comment ();

  if (f)
    fclose (static_cast<FILE *> (f));
}

void
parse_and_execute (const std::string& s, bool verbose, const char *warn_for)
{
  unwind_protect::begin_frame ("parse_and_execute_2");

  unwind_protect_bool (reading_script_file);
  unwind_protect_str (curr_fcn_file_full_name);

  reading_script_file = true;
  curr_fcn_file_full_name = s;

  FILE *f = get_input_from_file (s, 0);

  if (f)
    {
      unwind_protect::add (safe_fclose, f);

      unwind_protect_int (input_line_number);
      unwind_protect_int (current_input_column);

      input_line_number = 0;
      current_input_column = 1;

      if (verbose)
	{
	  std::cout << "reading commands from " << s << " ... ";
	  reading_startup_message_printed = true;
	  std::cout.flush ();
	}

      parse_and_execute (f);

      if (verbose)
	std::cout << "done." << std::endl;
    }
  else if (warn_for)
    error ("%s: unable to open file `%s'", warn_for, s.c_str ());

  unwind_protect::run_frame ("parse_and_execute_2");
}

static bool
looks_like_octave_copyright (const std::string& s)
{
  bool retval = false;

  std::string t = s.substr (0, 14);

  if (t == "Copyright (C) ")
    {
      size_t pos = s.find ('\n');

      if (pos != NPOS)
	{
	  pos = s.find ('\n', pos + 1);

	  if (pos != NPOS)
	    {
	      pos++;

	      t = s.substr (pos, 28);

	      if (t == "This file is part of Octave."
		  || t == "This program is free softwar")
		retval = true;
	    }
	}
    }

  return retval;
}

// Eat whitespace and comments from FFILE, returning the text of the
// comments read if it doesn't look like a copyright notice.  If
// IN_PARTS, consider each block of comments separately; otherwise,
// grab them all at once.  If UPDATE_POS is TRUE, line and column
// number information is updated.  If SAVE_COPYRIGHT is TRUE, then
// comments that are recognized as a copyright notice are saved in the
// comment buffer.

// XXX FIXME XXX -- grab_help_text() in lex.l duplicates some of this
// code!

static std::string
gobble_leading_white_space (FILE *ffile, bool in_parts,
			    bool update_pos, bool save_copyright)
{
  std::string help_txt;

  // TRUE means we have already seen the first block of comments.
  bool first_comments_seen = false;

  // TRUE means we are at the beginning of a comment block.
  bool begin_comment = false;

  // TRUE means we have already cached the help text.
  bool have_help_text = false;

  // TRUE means we are currently reading a comment block.
  bool in_comment = false;

  // TRUE means we should discard the first space from the input
  // (used to strip leading spaces from the help text).
  bool discard_space = true;

  int c;

  while ((c = getc (ffile)) != EOF)
    {
      if (update_pos)
	current_input_column++;

      if (begin_comment)
	{
	  if (c == '%' || c == '#')
	    continue;
	  else if (discard_space && c == ' ')
	    {
	      discard_space = false;
	      continue;
	    }
	  else
	    begin_comment = false;
	}

      if (in_comment)
	{
	  if (! have_help_text)
	    {
	      first_comments_seen = true;
	      help_txt += (char) c;
	    }

	  if (c == '\n')
	    {
	      if (update_pos)
		{
		  input_line_number++;
		  current_input_column = 0;
		}

	      in_comment = false;
	      discard_space = true;

	      if (in_parts)
		{
		  if ((c = getc (ffile)) != EOF)
		    {
		      if (update_pos)
			current_input_column--;
		      ungetc (c, ffile);
		      if (c == '\n')
			break;
		    }
		  else
		    break;
		}
	    }
	}
      else
	{
	  switch (c)
	    {
	    case ' ':
	    case '\t':
	      if (first_comments_seen)
		have_help_text = true;
	      break;

	    case '%':
	    case '#':
	      begin_comment = true;
	      in_comment = true;
	      break;

	    case '\n':
	      if (first_comments_seen)
		have_help_text = true;
	      if (update_pos)
		{
		  input_line_number++;
		  current_input_column = 0;
		}
	      continue;

	    case '\r':
	      c = getc (ffile);
	      if (update_pos)
		current_input_column++;
	      if (c == EOF)
		goto done;
	      else if (c == '\n')
		{
		  if (first_comments_seen)
		    have_help_text = true;
		  if (update_pos)
		    {
		      input_line_number++;
		      current_input_column = 0;
		    } 
		  continue;
		}

	      // Fall through...

	    default:
	      if (update_pos)
		current_input_column--;
	      ungetc (c, ffile);
	      goto done;
	    }
	}
    }

 done:

  if (! help_txt.empty ())
    {
      if (looks_like_octave_copyright (help_txt))
	{
	  if (save_copyright)
	    octave_comment_buffer::append (help_txt);

	  help_txt.resize (0);
	}

      if (in_parts && help_txt.empty ())
	help_txt = gobble_leading_white_space (ffile, in_parts,
					       update_pos, false);
    }

  return help_txt;
}

std::string
get_help_from_file (const std::string& path)
{
  std::string retval;

  if (! path.empty ())
    {
      FILE *fptr = fopen (path.c_str (), "r");

      if (fptr)
	{
	  unwind_protect::add (safe_fclose, (void *) fptr);

	  retval = gobble_leading_white_space (fptr, true, true, false);

	  unwind_protect::run ();
	}
    }

  return retval;
}

static int
is_function_file (FILE *ffile)
{
  int status = 0;

  long pos = ftell (ffile);

  gobble_leading_white_space (ffile, false, false, false);

  char buf [10];
  fgets (buf, 10, ffile);
  int len = strlen (buf);
  if (len > 8 && strncmp (buf, "function", 8) == 0
      && ! (isalnum (buf[8]) || buf[8] == '_'))
    status = 1;

  fseek (ffile, pos, SEEK_SET);

  return status;
}

static void
restore_command_history (void *)
{
  command_history::ignore_entries (! Vsaving_history);
}

static void
restore_input_stream (void *f)
{
  command_editor::set_input_stream (static_cast<FILE *> (f));
}

static void
clear_current_script_file_name (void *)
{
  bind_builtin_variable ("current_script_file_name", octave_value ());
}

static bool
parse_fcn_file (const std::string& ff, bool exec_script, bool force_script = false)
{
  unwind_protect::begin_frame ("parse_fcn_file");

  int script_file_executed = false;

  // Open function file and parse.

  bool old_reading_fcn_file_state = reading_fcn_file;

  FILE *in_stream = command_editor::get_input_stream ();

  unwind_protect::add (restore_input_stream, in_stream);

  unwind_protect_ptr (ff_instream);

  unwind_protect_int (input_line_number);
  unwind_protect_int (current_input_column);
  unwind_protect_bool (reading_fcn_file);
  unwind_protect_bool (line_editing);

  input_line_number = 0;
  current_input_column = 1;
  reading_fcn_file = true;
  line_editing = false;

  FILE *ffile = get_input_from_file (ff, 0);

  unwind_protect::add (safe_fclose, ffile);

  if (ffile)
    {
      // Check to see if this file defines a function or is just a
      // list of commands.

      if (! force_script && is_function_file (ffile))
	{
	  // XXX FIXME XXX -- we shouldn't need both the
	  // command_history object and the
	  // Vsaving_history variable...
	  command_history::ignore_entries ();

	  unwind_protect::add (restore_command_history, 0);

	  unwind_protect_int (Vecho_executing_commands);
	  unwind_protect_bool (Vsaving_history);
	  unwind_protect_bool (reading_fcn_file);
	  unwind_protect_bool (input_from_command_line_file);

	  Vecho_executing_commands = ECHO_OFF;
	  Vsaving_history = false;
	  reading_fcn_file = true;
	  input_from_command_line_file = false;

	  YY_BUFFER_STATE old_buf = current_buffer ();
	  YY_BUFFER_STATE new_buf = create_buffer (ffile);

	  unwind_protect::add (restore_input_buffer, (void *) old_buf);
	  unwind_protect::add (delete_input_buffer, (void *) new_buf);

	  switch_to_buffer (new_buf);

	  unwind_protect_ptr (curr_sym_tab);

	  reset_parser ();

	  help_buf = gobble_leading_white_space (ffile, true, true, true);

	  octave_comment_buffer::append (help_buf);

	  // XXX FIXME XXX -- this should not be necessary.
	  gobble_leading_white_space (ffile, false, true, false);

	  int status = yyparse ();

	  if (status != 0)
	    {
	      error ("parse error while reading function file %s",
		     ff.c_str ());
	      global_sym_tab->clear (curr_fcn_file_name);
	    }
	}
      else if (exec_script)
	{
	  // The value of `reading_fcn_file' will be restored to the
	  // proper value when we unwind from this frame.
	  reading_fcn_file = old_reading_fcn_file_state;

	  // XXX FIXME XXX -- we shouldn't need both the
	  // command_history object and the
	  // Vsaving_history variable...
	  command_history::ignore_entries ();

	  unwind_protect::add (restore_command_history, 0);

	  unwind_protect_bool (Vsaving_history);
	  unwind_protect_bool (reading_script_file);

	  Vsaving_history = false;
	  reading_script_file = true;

	  unwind_protect::add (clear_current_script_file_name, 0);

	  bind_builtin_variable ("current_script_file_name", ff);

	  parse_and_execute (ffile);

	  script_file_executed = true;
	}
    }

  unwind_protect::run_frame ("parse_fcn_file");

  return script_file_executed;
}

bool
load_fcn_from_file (symbol_record *sym_rec, bool exec_script)
{
  bool script_file_executed = false;

  std::string nm = sym_rec->name ();

  if (octave_dynamic_loader::load (nm))
    {
      force_link_to_function (nm);
    }
  else
    {
      std::string ff = fcn_file_in_path (nm);

      // These are needed by yyparse.

      unwind_protect::begin_frame ("load_fcn_from_file");

      unwind_protect_str (curr_fcn_file_name);
      unwind_protect_str (curr_fcn_file_full_name);

      curr_fcn_file_name = nm;
      curr_fcn_file_full_name = ff;

      if (ff.length () > 0)
	script_file_executed = parse_fcn_file (ff, exec_script);

      if (! (error_state || script_file_executed))
	force_link_to_function (nm);

      unwind_protect::run_frame ("load_fcn_from_file");
    }

  return script_file_executed;
}

DEFUN (source, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} source (@var{file})\n\
Parse and execute the contents of @var{file}.  This is equivalent to\n\
executing commands from a script file, but without requiring the file to\n\
be named @file{@var{file}.m}.\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin == 1)
    {
      std::string file_name = args(0).string_value ();

      if (! error_state)
	{
	  std::string file_full_name = file_ops::tilde_expand (file_name);

	  unwind_protect::begin_frame ("Fsource");

	  unwind_protect_str (curr_fcn_file_name);
	  unwind_protect_str (curr_fcn_file_full_name);

	  curr_fcn_file_name = file_name;
	  curr_fcn_file_full_name = file_full_name;

	  parse_fcn_file (file_full_name, true, true);

	  if (error_state)
	    error ("source: error sourcing file `%s'",
		   file_full_name.c_str ());

	  unwind_protect::run_frame ("Fsource");
	}
      else
	error ("source: expecting file name as argument");
    }
  else
    print_usage ("source");

  return retval;
}

// Evaluate an Octave function (built-in or interpreted) and return
// the list of result values.  NAME is the name of the function to
// call.  ARGS are the arguments to the function.  NARGOUT is the
// number of output arguments expected. 

octave_value_list
feval (const std::string& name, const octave_value_list& args, int nargout)
{
  octave_value_list retval;

  octave_function *fcn = is_valid_function (name, "feval", 1);

  if (fcn)
    retval = fcn->do_multi_index_op (nargout, args);

  return retval;
}

// Evaluate an Octave function (built-in or interpreted) and return
// the list of result values.  The first element of ARGS should be a
// string containing the name of the function to call, then the rest
// are the actual arguments to the function.  NARGOUT is the number of
// output arguments expected.

octave_value_list
feval (const octave_value_list& args, int nargout)
{
  octave_value_list retval;

  if (args.length () > 0)
    {
      std::string name = args(0).string_value ();

      if (! error_state)
	{
	  int tmp_nargin = args.length () - 1;

	  octave_value_list tmp_args (tmp_nargin, octave_value ());

	  for (int i = 0; i < tmp_nargin; i++)
	    tmp_args(i) = args(i+1);

	  string_vector arg_names = args.name_tags ();

	  if (! arg_names.empty ())
	    {
	      // tmp_nargin and arg_names.length () - 1 may differ if
	      // we are passed all_va_args.

	      int n = arg_names.length () - 1;

	      int len = n > tmp_nargin ? tmp_nargin : n;

	      string_vector tmp_arg_names (len);

	      for (int i = 0; i < len; i++)
		tmp_arg_names(i) = arg_names(i+1);

	      tmp_args.stash_name_tags (tmp_arg_names);
	    }

	  retval = feval (name, tmp_args, nargout);
	}
    }

  return retval;
}

DEFUN (feval, args, nargout,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} feval (@var{name}, @dots{})\n\
Evaluate the function named @var{name}.  Any arguments after the first\n\
are passed on to the named function.  For example,\n\
\n\
@example\n\
feval (\"acos\", -1)\n\
     @result{} 3.1416\n\
@end example\n\
\n\
@noindent\n\
calls the function @code{acos} with the argument @samp{-1}.\n\
\n\
The function @code{feval} is necessary in order to be able to write\n\
functions that call user-supplied functions, because Octave does not\n\
have a way to declare a pointer to a function (like C) or to declare a\n\
special kind of variable that can be used to hold the name of a function\n\
(like @code{EXTERNAL} in Fortran).  Instead, you must refer to functions\n\
by name, and use @code{feval} to call them.\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 0)
    retval = feval (args, nargout);
  else
    print_usage ("feval");

  return retval;
}

octave_value_list
eval_string (const std::string& s, bool silent, int& parse_status, int nargout)
{
  unwind_protect::begin_frame ("eval_string");

  unwind_protect_bool (get_input_from_eval_string);
  unwind_protect_bool (input_from_command_line_file);
  unwind_protect_ptr (global_command);
  unwind_protect_str (current_eval_string);

  get_input_from_eval_string = true;
  input_from_command_line_file = false;
  current_eval_string = s;

  YY_BUFFER_STATE old_buf = current_buffer ();
  YY_BUFFER_STATE new_buf = create_buffer (0);

  unwind_protect::add (restore_input_buffer, old_buf);
  unwind_protect::add (delete_input_buffer, new_buf);

  switch_to_buffer (new_buf);

  unwind_protect_ptr (curr_sym_tab);

  reset_parser ();

  parse_status = yyparse ();

  // Important to reset the idea of where input is coming from before
  // trying to eval the command we just parsed -- it might contain the
  // name of an function file that still needs to be parsed!

  tree_statement_list *command = global_command;

  unwind_protect::run_frame ("eval_string");

  octave_value_list retval;

  if (parse_status == 0 && command)
    {
      retval = command->eval (silent, nargout);
      delete command;
    }

  return retval;
}

octave_value
eval_string (const std::string& s, bool silent, int& parse_status)
{
  octave_value retval;

  octave_value_list tmp = eval_string (s, silent, parse_status, 1);

  if (! tmp.empty ())
    retval = tmp(0);

  return retval;
}

static octave_value_list
eval_string (const octave_value& arg, bool silent, int& parse_status,
	     int nargout)
{
  std::string s = arg.string_value ();

  if (error_state)
    {
      error ("eval: expecting std::string argument");
      return -1.0;
    }

  return eval_string (s, silent, parse_status, nargout);
}

DEFUN (eval, args, nargout,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} eval (@var{try}, @var{catch})\n\
Parse the string @var{try} and evaluate it as if it were an Octave\n\
program, returning the last value computed.  If that fails, evaluate\n\
the string @var{catch}.  The string @var{try} is evaluated in the\n\
current context, so any results remain available after @code{eval}\n\
returns.  For example,\n\
\n\
@example\n\
@group\n\
eval (\"a = 13\")\n\
     @print{} a = 13\n\
     @result{} 13\n\
@end group\n\
@end example\n\
\n\
In this case, the value of the evaluated expression is printed and it is\n\
also returned returned from @code{eval}.  Just as with any other\n\
expression, you can turn printing off by ending the expression in a\n\
semicolon.  For example,\n\
\n\
@example\n\
eval (\"a = 13;\")\n\
     @result{} 13\n\
@end example\n\
\n\
In this example, the variable @code{a} has been given the value 13, but\n\
the value of the expression is not printed.  You can also turn off\n\
automatic printing for all expressions executed by @code{eval} using the\n\
variable @code{default_eval_print_flag}.\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 0)
    {
      unwind_protect::begin_frame ("Feval");

      if (nargin > 1)
	{
	  unwind_protect_bool (buffer_error_messages);
	  buffer_error_messages = true;
	}

      int parse_status = 0;

      retval = eval_string (args(0), ! Vdefault_eval_print_flag,
			    parse_status, nargout);

      if (nargin > 1 && (parse_status != 0 || error_state))
	{
	  error_state = 0;

	  // Set up for letting the user print any messages from
	  // errors that occurred in the first part of this eval().

	  buffer_error_messages = false;

	  bind_global_error_variable ();

	  unwind_protect::add (clear_global_error_variable, 0);

	  eval_string (args(1), 0, parse_status, nargout);

	  retval = octave_value_list ();
	}

      unwind_protect::run_frame ("Feval");
    }
  else
    print_usage ("eval");

  return retval;
}

static int
default_eval_print_flag (void)
{
  Vdefault_eval_print_flag = check_preference ("default_eval_print_flag");

  return 0;
}

static int
warn_assign_as_truth_value (void)
{
  Vwarn_assign_as_truth_value
    = check_preference ("warn_assign_as_truth_value");

  return 0;
}

static int
warn_function_name_clash (void)
{
  Vwarn_function_name_clash = check_preference ("warn_function_name_clash");

  return 0;
}

static int
warn_future_time_stamp (void)
{
  Vwarn_future_time_stamp = check_preference ("warn_future_time_stamp");

  return 0;
}

static int
warn_missing_semicolon (void)
{
  Vwarn_missing_semicolon = check_preference ("warn_missing_semicolon");

  return 0;
}

static int
warn_variable_switch_label (void)
{
  Vwarn_variable_switch_label
    = check_preference ("warn_variable_switch_label");

  return 0;
}

void
symbols_of_parse (void)
{
  DEFVAR (default_eval_print_flag, 1.0, default_eval_print_flag,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} default_eval_print_flag\n\
If the value of this variable is nonzero, Octave prints the results of\n\
commands executed by @code{eval} that do not end with semicolons.  If it\n\
is zero, automatic printing is suppressed.  The default value is 1.\n\
@end defvr");

  DEFVAR (warn_assign_as_truth_value, 1.0, warn_assign_as_truth_value,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} warn_assign_as_truth_value\n\
If the value of @code{warn_assign_as_truth_value} is nonzero, a\n\
warning is issued for statements like\n\
\n\
@example\n\
if (s = t)\n\
  ...\n\
@end example\n\
\n\
@noindent\n\
since such statements are not common, and it is likely that the intent\n\
was to write\n\
\n\
@example\n\
if (s == t)\n\
  ...\n\
@end example\n\
\n\
@noindent\n\
instead.\n\
\n\
There are times when it is useful to write code that contains\n\
assignments within the condition of a @code{while} or @code{if}\n\
statement.  For example, statements like\n\
\n\
@example\n\
while (c = getc())\n\
  ...\n\
@end example\n\
\n\
@noindent\n\
are common in C programming.\n\
\n\
It is possible to avoid all warnings about such statements by setting\n\
@code{warn_assign_as_truth_value} to 0, but that may also\n\
let real errors like\n\
\n\
@example\n\
if (x = 1)  # intended to test (x == 1)!\n\
  ...\n\
@end example\n\
\n\
@noindent\n\
slip by.\n\
\n\
In such cases, it is possible suppress errors for specific statements by\n\
writing them with an extra set of parentheses.  For example, writing the\n\
previous example as\n\
\n\
@example\n\
while ((c = getc()))\n\
  ...\n\
@end example\n\
\n\
@noindent\n\
will prevent the warning from being printed for this statement, while\n\
allowing Octave to warn about other assignments used in conditional\n\
contexts.\n\
\n\
The default value of @code{warn_assign_as_truth_value} is 1.\n\
@end defvr");

  DEFVAR (warn_function_name_clash, 1.0, warn_function_name_clash,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} warn_function_name_clash\n\
If the value of @code{warn_function_name_clash} is nonzero, a warning is\n\
issued when Octave finds that the name of a function defined in a\n\
function file differs from the name of the file.  (If the names\n\
disagree, the name declared inside the file is ignored.)  If the value\n\
is 0, the warning is omitted.  The default value is 1.\n\
@end defvr");

  DEFVAR (warn_future_time_stamp, 1.0, warn_future_time_stamp,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} warn_future_time_stamp\n\
If the value of this variable is nonzero, Octave will print a warning\n\
if it finds a function file with a time stamp that is in the future.\n\
@end defvr");

  DEFVAR (warn_missing_semicolon, 0.0, warn_missing_semicolon,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} warn_missing_semicolon\n\
If the value of this variable is nonzero, Octave will warn when\n\
statements in function definitions don't end in semicolons.  The default\n\
value is 0.\n\
@end defvr");

  DEFVAR (warn_variable_switch_label, 0.0, warn_variable_switch_label,
    "-*- texinfo -*-\n\
@defvr {Built-in Variable} warn_variable_switch_label\n\
If the value of this variable is nonzero, Octave will print a warning if\n\
a switch label is not a constant or constant expression\n\
@end defvr");

}

/*
;;; Local Variables: ***
;;; mode: text ***
;;; End: ***
*/
