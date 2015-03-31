// -*-c++-*-

// Gordon's source file

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include <string>

#include <rna/rna.h>

#include "pt-walk.h"

#include "ov-usr-fcn.h"
#include "pt-misc.h"
#include "pt-decl.h"
#include "pt-stmt.h"

#include "pt-cmd.h"
#include "pt-jump.h"
#include "pt-loop.h"
#include "pt-select.h"

#include "pt-binop.h"
#include "pt-colon.h"
#include "pt-except.h"
#include "pt-id.h"
#include "pt-const.h"
#include "pt-assign.h"
#include "pt-idx.h"
#include "pt-mat.h"
#include "pt-cell.h"
#include "pt-indir.h"
#include "pt-unop.h"

#include "pt-arg-list.h"
#include "comment-list.h"
#include "symtab.h"

#include "cp.h"
#include "cp-air.h"

using namespace RNA;

struct cmmt {
    octave_comment_list *   list;
    char *                  label;
};

static void
warn_plot_command ()
{
    fprintf (stderr, "warning: skipping plot command\n");
}

Rna
compiler_air::emit_octave_user_function (octave_user_function& item)
{
    char * old_src_filename = src_filename;

    src_filename=m->alloc0(item.fcn_file_name().c_str());

    Rna rna = TreeFunction::Create
	(m, TreeDef::Create (m, m->alloc0(item.fcn_name.c_str ()),
                             Rna::Null ()),
	 emit_parameter_list (*item.parameter_list ()),
	 emit_parameter_list (*item.return_list ()),
	 emit_statement_list (*item.body ()),
	 item.takes_varargs ());

    if ( item.leading_comment() )
        rna[(Str)"leading-comment"] = Rna(emit_comment_list(item.leading_comment()));

    if ( item.trailing_comment() )
        rna[(Str)"trailing-comment"] = Rna(emit_comment_list(item.trailing_comment()));

    // FIXME: get starting row & col info of the function from somewhere
    rna[(Str)"src-info"] = SrcInfo::Create (m, src_filename, 1, 1);
    
    src_filename=old_src_filename;

    return rna;
}

Rna
compiler_air::emit_parameter_list (tree_parameter_list& item)
{
    if (&item == 0)
	return Rna::Null ();

//    RNA_ASSERT (!item.takes_varargs ());

    Rna result = RNA::Array::Create (m, item.length (), RECORD);
    int i = 0;
    for (Pix p = item.first (); p; item.next (p))
	result [i++] = TreeDef::Create (m,
					m->alloc0(item(p)->name().c_str ()),
                                        Rna::Null ());

    return result;
}

Rna
compiler_air::emit_statement_list (tree_statement_list& item)
{
    Rna array = RNA::Array::Create (m, item.length (), RECORD);
    Rna result = TreeSeq::Create (m, array);

    int i;
    Pix p;

    for ( i = 0, p = item.first (); p; item.next (p), i++ ) {

        Rna stmt = emit_statement(*item(p));
        array[i] = stmt;

        if ( (*item(p)).comment_text() )
            stmt[(Str)"comment"] = emit_comment_list((*item(p)).comment_text());

        // FIXME: obtain file name info of the current statement from somewhere
        if ( (*item(p)).expression() )
            stmt[(Str)"src-info"] = SrcInfo::Create
		(m, src_filename,
		 (*item (p)).line (), (*item (p)).column ());
    }

    Memory * m2 = new Memory;
    int new_length = 0;
    m2->startArray (0, RECORD);
    cleanup_statement_list_rec (array, m2, new_length);    
    RNA::Array * array2 = m2->finishArray ();
    array2->setLength (new_length);
    
    return TreeSeq::Create (m, array2);
}

void
compiler_air::cleanup_statement_list_rec (RNA::Array * array, RNA::Memory * m2, int & new_length)
{
    for (int i = 0; i < array->getLength(); i++)
	if (TreeSeq::IsA (array->at (i)))
	    cleanup_statement_list_rec 
		(TreeSeq::Convert (array->at(i))->get_list (),
		 m2, new_length);
	else if (!array->at (i).isNull ()) {
	    m2->grow (array->at (i));
	    new_length++;
	}
}

class tree_switch_emit_function
    : public tree_walker
{
public:
    tree_switch_emit_function (compiler_air * cp)
	: compiler (cp),
	  result (Rna::Null()) {}

    void visit_argument_list (tree_argument_list& item) {
	result = compiler->emit_argument_list (item); };
    void visit_binary_expression (tree_binary_expression& item) {
	result = compiler->emit_binary_expression (item); };
    void visit_break_command (tree_break_command& item) {
	result = compiler->emit_break_command (item); };
    void visit_cell (tree_cell&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_colon_expression (tree_colon_expression& item) {
	result = compiler->emit_colon_expression (item); };
    void visit_complex_for_command (tree_complex_for_command&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_constant (tree_constant& item) {
	result = compiler->emit_constant(item); };
    void visit_continue_command (tree_continue_command& item) {
	result = compiler->emit_continue_command (item); };
    void visit_decl_command (tree_decl_command& item) {
	result = compiler->emit_decl_command(item); }
    void visit_decl_elt (tree_decl_elt& item) {
	result = compiler->emit_decl_elt(item); };
    void visit_decl_init_list (tree_decl_init_list& item) {
	result = compiler->emit_decl_init_list(item); };
    void visit_do_until_command (tree_do_until_command& item) {
	result = compiler->emit_do_until_command (item); };
    void visit_identifier (tree_identifier& item) {
	result = compiler->emit_identifier(item); };
    void visit_if_clause (tree_if_clause& item) {
	result = compiler->emit_if_clause(item); };
    void visit_if_command (tree_if_command& item) {
	result = compiler->emit_if_command (item); };
    void visit_if_command_list (tree_if_command_list& item) {
	result = compiler->emit_if_command_list (item); };
    void visit_index_expression (tree_index_expression& item) {
	result = compiler->emit_index_expression (item); };
    void visit_indirect_ref (tree_indirect_ref& item) {
	result = compiler->emit_indirect_ref (item); };
    void visit_matrix (tree_matrix& item) {
	result = compiler->emit_matrix (item); };
    void visit_multi_assignment (tree_multi_assignment& item) {
	result = compiler->emit_multi_assignment (item); };
    void visit_no_op_command (tree_no_op_command&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_octave_user_function (octave_user_function&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_parameter_list (tree_parameter_list&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_plot_command (tree_plot_command&) {
        warn_plot_command (); };
    void visit_plot_limits (plot_limits&) {
        warn_plot_command (); };
    void visit_plot_range (plot_range&) {
        warn_plot_command (); };
    void visit_postfix_expression (tree_postfix_expression& item) {
	result = compiler->emit_postfix_expression (item);};
    void visit_prefix_expression (tree_prefix_expression& item) {
	result = compiler->emit_prefix_expression (item);};
    void visit_return_command (tree_return_command& item) {
	result = compiler->emit_return_command (item);};
    void visit_return_list (tree_return_list&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_simple_assignment (tree_simple_assignment& item) {
	result = compiler->emit_simple_assignment (item); };
    void visit_simple_for_command (tree_simple_for_command& item) {
	result = compiler->emit_simple_for_command (item); };
    void visit_statement (tree_statement&) {
	RNA_SHOULD_NOT_REACH_HERE;};
    void visit_statement_list (tree_statement_list&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_subplot (subplot&) {
        warn_plot_command (); };
    void visit_subplot_axes (subplot_axes&) {
        warn_plot_command (); };
    void visit_subplot_list (subplot_list&) {
        warn_plot_command (); };
    void visit_subplot_style (subplot_style&) {
        warn_plot_command (); };
    void visit_subplot_using (subplot_using&) {
        warn_plot_command (); };
    void visit_switch_case (tree_switch_case& item) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_switch_case_list (tree_switch_case_list& item) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_switch_command (tree_switch_command& item) {
	result = compiler->emit_switch_command (item); };
    void visit_try_catch_command (tree_try_catch_command&) {
	RNA_SHOULD_NOT_REACH_HERE; };
    void visit_unwind_protect_command (tree_unwind_protect_command& item) {
	result = compiler->emit_unwind_protect_command (item); };
    void visit_while_command (tree_while_command& item) {
	result = compiler->emit_while_command (item); };

    compiler_air * compiler;
    Rna result;
};

Rna
compiler_air::emit_statement (tree_statement& item)
{
    if (item.command ()) {
	return emit_command (*item.command());
    }
    else if (item.expression ()){
	return emit_expression (*item.expression());
    }

    return Rna::Null ();
}

Rna
compiler_air::emit_command (tree_command& item)
{
    tree_switch_emit_function tsef (this);
    item.accept (tsef);

    return tsef.result;
}

Rna
compiler_air::emit_expression (tree_expression& item)
{
    tree_switch_emit_function tsef (this);
    item.accept (tsef);

    return tsef.result;
}

Rna
compiler_air::emit_identifier (tree_identifier& item)
{
    return TreeVar::Create (m, m->alloc0(item.name().c_str ()));
}

Rna compiler_air::emit_symbol_table (symbol_table&){}
Rna compiler_air::emit_symbol (symbol_record&){}

RNA::Array * compiler_air::emit_comment_list (octave_comment_list* list)
{
    RNA::Array * a = RNA::Array::Create(m, list->length(), RNA::STR);

    int i;
    Pix p;
    for ( i = 0, p = list->first(); p != NULL; list->next(p), i++ )
        a->atPut(i, m->alloc0(list->operator () (p).text().c_str()));

    return a;
}

Rna compiler_air::emit_comment (octave_comment_elt&){}

Rna
compiler_air::emit_break_command (tree_break_command& item)
{
    return TreeBreak::Create (m);
}

Rna
compiler_air::emit_continue_command (tree_continue_command&)
{
    return TreeContinue::Create (m);
}

Rna
compiler_air::emit_return_command (tree_return_command&)
{
    return TreeReturn::Create (m);
}

Rna
compiler_air::emit_no_op_command (tree_no_op_command&)
{
    return Rna::Null ();
}

Rna
compiler_air::emit_simple_for_command (tree_simple_for_command& item)
{
    Rna rna = emit_expression (*item.left_hand_side ());
    assert (TreeVar::IsA (rna));
    TreeVar * var = TreeVar::Convert (rna);
    Rna name = var->get_name ();

    return TreeFor::Create (m, TreeDef::Create (m, name, Rna::Null ()),
			    emit_expression (*item.control_expr ()),
			    emit_statement_list (*item.body ()));
}

Rna compiler_air::emit_complex_for_command (tree_complex_for_command&){}

Rna
compiler_air::emit_while_command (tree_while_command & item)
{
    return TreeWhile::Create (m, emit_expression (* item.condition ()),
			      emit_statement_list (* item.body ()));
}

Rna
compiler_air::emit_do_until_command (tree_do_until_command& item)
{
    Rna array = RNA::Array::Create (m, 2, RECORD);
    Rna result = TreeSeq::Create (m, array);
    Rna args = RNA::Array::Create (m, 1, RECORD);
    args [0] = emit_expression (*item.condition());
    array [0] = emit_statement_list (* item.body ());
    array [1] = TreeWhile::Create (m, TreeOp::Create (m, "!", args),
				   emit_statement_list (* item.body ()));

    return result;
}

Rna
compiler_air::emit_if_command (tree_if_command& item)
{
    return emit_if_command_list (* item.cmd_list());
}

Rna
compiler_air::emit_if_command_list (tree_if_command_list& item)
{
    switch (if_mode) {
        case IF_MODE_TREE: {
            Rna array = RNA::Array::Create (m, item.length (), RECORD);
            TreeIf * my_if = 0;
            Rna var, var2 = Rna::Null ();
            int i = 0;
            for (Pix p = item.first (); p; item.next (p))
	        array [i++] = emit_if_clause (*item (p));
            for (i = item.length () -1; i > 0; i--){
	        var = array [i-1];
	        var2 = array [i];
	        my_if = TreeIf::Convert (var);
	        RNA_ASSERT (my_if != 0);
	        my_if->set_fbody (var2);
            }
            return my_if;
	    break;
	    }

        case IF_MODE_LIST: {
            Rna array = RNA::Array::Create (m, item.length (), RECORD);
	    Rna elsebody = Rna::Null ();
            int i = 0;
            for (Pix p = item.first (); p; item.next (p)) {
                if (TreeElseif::IsA (emit_if_clause (*item (p)))) {
	            array [i++] = emit_if_clause (*item (p));
                } else if (TreeElse::IsA (emit_if_clause (*item (p)))) {
		    elsebody = emit_if_clause (*item (p));
		    RNA_ASSERT(i == item.length ()-1);
		    array.readArray ()->setLength (item.length ()-1);
		} else {
		    RNA_SHOULD_NOT_REACH_HERE;
		    return Rna::Null ();
		}
            }
            return TreeIfs::Create (m, array, elsebody);
	    break;
	    }

        default: {
            RNA_SHOULD_NOT_REACH_HERE;
	    return Rna::Null ();
        }
    }
}

Rna
compiler_air::emit_if_clause (tree_if_clause& item)
{
    switch (if_mode) {
        case IF_MODE_TREE: {
            Rna falsebody = Rna::Null ();
            if (item.is_else_clause ()){
	        return emit_statement_list (* item.commands());
            }
            else{
	        return TreeIf::Create (m, emit_expression (* item.condition()),
			       emit_statement_list (* item.commands()),
			       falsebody);
            break;
            }
        }
        case IF_MODE_LIST: {
            if (item.is_else_clause ()){
	        return TreeElse::Create (m, emit_statement_list (* item.commands()));
            }
            else{
	        return TreeElseif::Create (m, emit_expression (* item.condition()),
			       emit_statement_list (* item.commands()));
            }
            break;
        }

        default: {
            RNA_SHOULD_NOT_REACH_HERE;
	    return Rna::Null ();
        }
    }
}

Rna
compiler_air::emit_switch_command (tree_switch_command& item)
{
    return emit_switch_case_list (* item.case_list(), * item.switch_value() );
}

Rna
compiler_air::emit_switch_case_list (tree_switch_case_list& item, tree_expression& switch_value)
{
    switch (switch_mode) {
    case SWITCH_MODE_ON: {
	Rna array = RNA::Array::Create (m, item.length (), RECORD);
	Rna otherwise = Rna::Null ();
	int i = 0;
	for (Pix p = item.first (); p; item.next (p)) {
	    array [i++] = emit_switch_case (*item (p), switch_value);
	    if (TreeSeq::IsA (emit_switch_case (*item (p), switch_value))) {
		otherwise = emit_switch_case (*item (p), switch_value);
		RNA_ASSERT (i == item.length ());
		array.readArray ()->setLength (item.length ()-1);
	    }
	}
	return TreeSwitch::Create (m, emit_expression (switch_value), array, otherwise);
	break;
    }
    case SWITCH_MODE_OFF: {
	switch (if_mode) {
	case IF_MODE_TREE: {
	    Rna array = RNA::Array::Create (m, item.length (), RECORD);
	    TreeIf * my_if = 0;
	    Rna var, var2 = Rna::Null ();
	    int i = 0;
	    for (Pix p = item.first (); p; item.next (p))
		array [i++] = emit_switch_case (*item (p), switch_value);
	    for (i = item.length () -1; i > 0; i--) {
		var = array [i-1];
		var2 = array [i];
		my_if = TreeIf::Convert (var);
		RNA_ASSERT (my_if != 0);
		my_if->set_fbody (var2);
	    }
	    return my_if;
	    break;
	}
	case IF_MODE_LIST: {
	    Rna array = RNA::Array::Create (m, item.length (), RECORD);
	    Rna elsebody = Rna::Null ();
	    int i = 0;
	    for (Pix p = item.first (); p; item.next (p)) {
		if (TreeElseif::IsA (emit_switch_case (*item (p), switch_value))) {
		    array [i++] = emit_switch_case (*item (p), switch_value);
		} else if (TreeElse::IsA (emit_switch_case (*item (p), switch_value))) {
		    elsebody = emit_switch_case (*item (p), switch_value);
		    RNA_ASSERT(i == item.length ()-1);
		    array.readArray ()->setLength (item.length ()-1);
		} else {
		    RNA_SHOULD_NOT_REACH_HERE;
		    return Rna::Null ();
		}
	    }
	    return TreeIfs::Create (m, array, elsebody);
	    break;
	}
	default: {
	    RNA_SHOULD_NOT_REACH_HERE;
	    return Rna::Null ();
	}
	    break;
	}
    }
    default: {
	RNA_SHOULD_NOT_REACH_HERE;
	return Rna::Null ();
    }
    }
}

Rna compiler_air::emit_switch_case (tree_switch_case& item, tree_expression& switch_value)
{
    switch (switch_mode) {
    case SWITCH_MODE_ON: {
	Rna array = RNA::Array::Create (m, 1, RECORD);
	if (item.is_default_case ()) {
	    array[0] = emit_statement_list (* item.commands());
	    return TreeSeq::Create (m, array);
	} else {
	    return TreeCase::Create (m, emit_expression (* item.case_label()),
				     emit_statement_list (* item.commands()));
	}
	break;
    }
    case SWITCH_MODE_OFF: {
	switch (if_mode) {
        case IF_MODE_TREE: {
            Rna falsebody = Rna::Null ();
            Rna array = RNA::Array::Create (m, 2, RECORD);
            if (item.is_default_case ()) {
	        return emit_statement_list (* item.commands());
            } else {
	        array [0] = emit_expression (switch_value);
	        array [1] = emit_expression (* item.case_label());
	        return TreeIf::Create (m,
				       TreeOp::Create (m, "==", array),
				       emit_statement_list (* item.commands()),
				       falsebody);
            }
            break;
        }
        case IF_MODE_LIST: {
            Rna array = RNA::Array::Create (m, 2, RECORD);
            if (item.is_default_case ()) {
	        return TreeElse::Create (m, emit_statement_list (* item.commands()));
            } else {
	        array [0] = emit_expression (switch_value);
	        array [1] = emit_expression (* item.case_label());
	        return TreeElseif::Create (m,
					   TreeOp::Create (m, "==", array),
					   emit_statement_list (* item.commands()));
            }
	    break;
	}
	default: {
            RNA_SHOULD_NOT_REACH_HERE;
	    return Rna::Null ();
        }
	break;
	}
    }
    default: {
	RNA_SHOULD_NOT_REACH_HERE;
	return Rna::Null ();
    }
    }
}

Rna
compiler_air::emit_decl_command (tree_decl_command& item)
{
    return emit_decl_init_list(*item.initializer_list());
}

Rna
compiler_air::emit_decl_init_list (tree_decl_init_list& item)
{
    Rna array = RNA::Array::Create (m, item.length (), RECORD);
    Rna result = TreeSeq::Create (m, array);
    int i = 0;
    for (Pix p = item.first (); p; item.next (p))
	array [i++] = emit_decl_elt (*item (p));

    return result;
}

Rna
compiler_air::emit_decl_elt (tree_decl_elt& item)
{
    if (item.expression() != 0){
	Rna array = RNA::Array::Create (m, 2, RECORD);
	Rna result = TreeSeq::Create (m, array);
	array [0] = TreeDef::Create
            (m , m->alloc0(item.ident ()->name().c_str()), Rna::Null ());
	array [1] = TreeAssign::Create (m, emit_identifier (*item.ident ()),
					"=",
					emit_expression (*item.expression ()));
	return result;
    }
    else{
	return TreeDef::Create (m , m->alloc0(item.ident ()->name().c_str()),
                                Rna::Null ());
    }
}

Rna compiler_air::emit_plot_command (tree_plot_command&)
{
    warn_plot_command (); 
}

Rna compiler_air::emit_try_catch_command (tree_try_catch_command&){}

Rna
compiler_air::emit_unwind_protect_command (tree_unwind_protect_command& item)
{
    return TreeUnwindProtect::Create
	(m,
	 emit_statement_list (*item.body ()),
	 emit_statement_list (*item.cleanup ()));
}

Rna
compiler_air::emit_binary_expression (tree_binary_expression& item)
{
    Rna args = RNA::Array::Create (m, 2, RECORD);
    args [0] = emit_expression (*item.lhs());
    args [1] = emit_expression (*item.rhs());

    return TreeOp::Create (m,
			   m->alloc0 (item.oper ().c_str()),
			   args);
}

Rna
compiler_air::emit_colon_expression (tree_colon_expression& item)
{
    return TreeRange::Create
	(m, emit_expression (*item.base()),
	 item.increment() ? emit_expression (*item.increment())
	 : Rna (ValueInt::Create (m, 1)),
	 emit_expression (*item.limit()));
}

Rna
compiler_air::double2value (double dd)
{
    int ii = int(dd);
    if (dd == double(ii)) {
	return ValueInt::Create (m, Int (ii));
    } else {
	return ValueFloat::Create (m, Float (dd));
    }
}

Rna
compiler_air::emit_constant (tree_constant& item)
{
    octave_value ov = item.rvalue();
    if (debug_mode) {
	printf(ov.is_range () ? "is_range () = true\n" : "");
	printf(ov.is_complex_scalar () ? "is_complex_scalar () = true\n" : "");
	printf(ov.is_magic_colon () ? "is_magic_colon () = true\n" : "");
	printf(ov.is_string () ? "is_string () = true\n" : "");
	printf(ov.is_real_matrix () ? "is_real_matrix () = true\n" : "");
	printf(ov.is_complex_matrix () ? "is_complex_matrix () = true\n" : "");
	printf(ov.is_scalar_type () ? "is_scalar_type () = true\n" : "");
	printf(ov.is_char_matrix () ? "is_char_matrix () = true\n" : "");
	printf(ov.is_matrix_type () ? "is_matrix_type () = true\n" : "");
	printf(ov.is_map () ? "is_map () = true\n" : "");
	printf(ov.is_real_scalar () ? "is_real_scalar () = true\n" : "");
    }
    if (ov.is_real_scalar ()) {
	if (debug_mode)
	    printf("ov.is_real_scalar() is reached\n");
        return double2value (ov.double_value ());
    } else if (ov.is_range ()) {
	if (debug_mode)
	    printf("ov.is_range() is reached\n");
	Range range = ov.range_value ();
	return TreeRange::Create (m, double2value (range.base ()),
				  double2value (range.inc ()),
				  double2value (range.limit ()));
    } else if (ov.is_magic_colon()){
	if (debug_mode)
	    printf("ov.is_magic_colon() is reached\n");
	return TreeColon::Create (m);
    } else if (ov.is_complex_scalar ()) {
	if (debug_mode)
	    printf("ov.is_complex_scalar() is reached\n");
	Complex cx (ov.complex_value ());
	return ValueComplex::Create (m, double2value (cx.real ()),
				     double2value (cx.imag ()));
    }/*
      else if (ov.is_char_matrix ()){
	if (debug_mode)
	    printf("ov.is_char_matrix() is reached\n");
	charMatrix chmx = ov.char_matrix_value ();
	int rows = chmx.rows ();
	int cols = chmx.cols ();
	Rna row_array = RNA::Array::Create (m, rows, RNA::STR);
	for (int i = 0; i < rows; i++) {
	    std::string s = chmx.row_as_string (i);
	    row_array[i] = m->alloc0(s.c_str());
	}
	return ValueMatrix::Create (m, row_array);
	}*/ 
      else if (ov.is_string ()){
	return ValueString::Create (m, m->alloc0((ov.string_value ()).c_str ()));
    } else if (ov.is_real_matrix () && ov.is_matrix_type ()){
	if (debug_mode)
	    printf("ov.is_real_matrix() is reached\n");
	Matrix mx (ov.matrix_value ());
	int rows = mx.rows ();
	int cols = mx.cols ();
	Rna row_array = RNA::Array::Create (m, rows, ARRAY);
	for (int i = 0; i < rows; i++) {
	    Rna row = RNA::Array::Create (m, cols, RECORD);
	    for (int j = 0; j < cols; j++) {
		row [j] = double2value (mx (i,j));
	    }
	    row_array [i] = row;
	}
	return ValueMatrix::Create (m, row_array);
    } else if (ov.is_complex_matrix ()){
	if (debug_mode)
	    printf("ov.is_complex_matrix() is reached\n");
	ComplexMatrix cmx (ov.complex_matrix_value ());
	int rows = cmx.rows ();
	int cols = cmx.cols ();
	Rna row_array = RNA::Array::Create (m, rows, ARRAY);
	for (int i = 0; i < rows; i++) {
	    Rna row = RNA::Array::Create (m, cols, RECORD);
	    for (int j = 0; j < cols; j++) {
		Complex cx (cmx (i,j));
		row [j] = ValueComplex::Create (m, double2value (cx.real ()),
						double2value (cx.imag ()));
	    }
	    row_array [i] = row;
	}
	return ValueMatrix::Create (m, row_array);
    }
#if 0
    else if (ov.is_map){
	Octave_map map (ov.map_value());
	int length = 0;
	for (Pix p = map.first (); p; map.next (p))
	    length++;
	Rna array = RNA::Array::Create (m, length, RECORD);
	int i = 0;
	for (Pix p = map.first (); p; map.next (p)) {
	    std::string key = map.key (p);
	    char * key = map.key (p).c_str ();
	    octave_value val = map.contents (p);
	    Element * el = Element::Create (m, key, emit_constant (value));
	    array [i++] = el;
	}
	return ValueStruct::Create (m, array);
    } 
#endif   
    else {
	RNA_SHOULD_NOT_REACH_HERE;
    }
    return Rna::Null();
}

Rna
compiler_air::emit_index_expression (tree_index_expression& item)
{
    Rna args = Rna::Null ();
    if (item.arg_list())
	if (item.arg_list()->length() > 0) {
	    args = RNA::Array::Create (m, item.arg_list()->length(), RECORD);
	    int i = 0;
	    for (Pix p = item.arg_list()->first (); p;  item.arg_list()->next (p))
		args [i++] = emit_expression (*(*item.arg_list())(p));
	}

    return TreeSubscript::Create (m, emit_expression (*item.expression ()), args);
}

Rna
compiler_air::emit_simple_assignment (tree_simple_assignment& item)
{
    return TreeAssign::Create (m, emit_expression (*item.left_hand_side ()),
			       m->alloc0 (item.oper ().c_str()),
			       emit_expression (*item.right_hand_side ()));
}

Rna 
compiler_air::emit_multi_assignment (tree_multi_assignment& item)
{
    return TreeAssignMultiple::Create 
	(m, emit_argument_list (*item.left_hand_side ()),
	 m->alloc0 (item.oper ().c_str()),
	 emit_expression (*item.right_hand_side ()));
}

Rna 
compiler_air::emit_argument_list (tree_argument_list& item)
{
    Rna args = RNA::Array::Create (m, item.length(), RECORD);

    int i = 0;
    for (Pix p = item.first (); p; item.next (p))
	args [i++] = emit_expression (*item (p));

    return args;
}

Rna 
compiler_air::emit_indirect_ref (tree_indirect_ref& item)
{
    return TreePick::Create
	(m, emit_expression (*item.expression ()),
	 m->alloc0 (item.elt_name ().c_str ()));
}

Rna 
compiler_air::emit_matrix (tree_matrix& item)
{
    Rna args = RNA::Array::Create (m, item.length(), ARRAY);

    int i = 0;
    for (Pix p = item.first (); p; item.next (p))
	args [i++] = emit_argument_list (*item (p));

    return TreeMatrix::Create (m, args);
}

Rna compiler_air::emit_cell (tree_cell&){}

Rna
compiler_air::emit_postfix_expression (tree_postfix_expression& item)
{
    Rna args = RNA::Array::Create (m, 1, RECORD);
    args [0] = emit_expression (*item.operand());

    return TreeOp::Create (m, m->alloc0 (item.oper ().c_str()),
			   args);
}

Rna
compiler_air::emit_prefix_expression (tree_prefix_expression& item)
{
    Rna args = RNA::Array::Create (m, 1, RECORD);
    args [0] = emit_expression (*item.operand());

    return TreeOp::Create (m, m->alloc0 (item.oper ().c_str()),
			   args);
}
