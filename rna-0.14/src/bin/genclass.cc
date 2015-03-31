/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/11 08:05:04 $
 * $Source: /sys/cvs/rna/src/bin/genclass.cc,v $
 * $Revision: 1.6 $
 */

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rna/rna.h"
using namespace RNA;

#include "config.h"
#include "emit.h"
#include "naming.h"

#include <set>
#include <vector>
#include <map>

/////////////////////////////////////////////////////////////////////////////
//
// Type Information
//
/////////////////////////////////////////////////////////////////////////////

struct TypeInfo;
struct LeafInfo;
struct CompositeInfo;
struct RecordInfo;
struct VariantInfo;
struct MemberInfo;
struct ModuleInfo;
struct SystemInfo;

struct TypeInfo {
    virtual bool is_leaf () const { return false; };
    virtual bool is_composite () const { return false; };

    virtual char * type_name () = 0;
    virtual char * fq_type_name () = 0;
    virtual char * conversion (char * base) = 0;
};

struct LeafInfo : public TypeInfo {
    virtual bool is_leaf () const { return true; };

    Type type;

    static LeafInfo * For (Type type);

    LeafInfo (Type type1) : type (type1) {};
    virtual char * type_name ();
    virtual char * fq_type_name ();
    virtual char * conversion (char * base) {
	char * result = "*error*";
	switch (type) {
	    // @@@ TYPELIST @@@
	case BOOL: asprintf (&result, "%s.readBool ()", base); break;
	case CHAR: asprintf (&result, "%s.readChar ()", base); break;
	case INT: asprintf (&result, "%s.readInt ()", base); break;
	case FLOAT: asprintf (&result, "%s.readFloat ()", base); break;
	case PTR: asprintf (&result, "%s.readPtr ()", base); break;
	case REF: asprintf (&result, "%s.readRef ()", base); break;
	case STR: asprintf (&result, "%s.readStr ()", base); break;
	case RECORD: asprintf (&result, "%s.readRecord ()", base); break;
	case ARRAY: asprintf (&result, "%s.readArray ()", base); break;
	case LIST: asprintf (&result, "%s.readList ()", base); break;
	default: RNA_SHOULD_NOT_REACH_HERE;
	}
	return result;
    };
};

struct ltCompositeInfo {
    bool operator () (const CompositeInfo * ci1, const CompositeInfo * ci2) const;
};

struct CompositeInfo : public TypeInfo {
    virtual bool is_composite () const { return true; };

    Record * info;
    ModuleInfo * module;

    char * name;
    char * macro_name;
    char * class_name;
    char * member_name;

    set<CompositeInfo *, ltCompositeInfo> forward_declarations;

    set<CompositeInfo *> supertypes;
    set<CompositeInfo *> all_supertypes;

    virtual bool is_record () const { return false; };
    virtual bool is_variant () const { return false; };
    virtual TypeDescriptor * get_record () { RNA_SHOULD_NOT_REACH_HERE; };
    virtual Variant * get_variant () { RNA_SHOULD_NOT_REACH_HERE; };

    static CompositeInfo * Create (Record * info1);
    virtual char * type_name () {
	char * result;
	asprintf (&result, "%s *", class_name);
	return result;
    };
    virtual char * fq_type_name ();
    virtual char * conversion (char * base);
    static map<Record *, CompositeInfo *> type_table;
    void init (Record *info1, char * name1);
    void init2 (ModuleInfo *);
    virtual void init3 () = 0;
    bool init3_done;

    void generate_cpp ();
    void generate_cpp_header ();
    void generate_cpp_inline ();
    void generate_cpp_source ();
    virtual void generate_cpp_header_body (FILE *) = 0;
    virtual void generate_cpp_inline_body (FILE *) = 0;
    virtual void generate_cpp_source_body (FILE *) = 0;

    virtual void emit_members (FILE *, int indent) = 0;
};

bool 
ltCompositeInfo::operator () (const CompositeInfo * ci1, const CompositeInfo * ci2) const
{
    if (ci1->is_record () && ci2->is_variant ())
	return true;

    return strcmp (ci1->class_name, ci2->class_name) < 0;
}

struct MemberInfo {
    TypeDescriptor * base;
    int index;

    char * name;
    TypeInfo * type;

    char * macro_name;
    char * member_name;

    MemberInfo (TypeDescriptor * desc, int index1) {
	base = desc;
	index = index1;

	name = desc->get_parts () [index];
	macro_name = make_macro_name (name);
	member_name = make_member_name (name);

	Type type1 (desc->type (index));
	type = LeafInfo::For (type1);
	List * list (desc->getAttr ()->find ("types"));
	Record * type2 = 0;
	if (list) {
	    Array * types = list->getCar ();
	    type2 = types->at (index);
	    if (type2) {
		RNA_ASSERT (type1 == RECORD || type1 == ARRAY);
		if (type1 == RECORD)
		    type = CompositeInfo::Create (type2);
	    }
	}
    };
};

struct RecordInfo : public CompositeInfo {
    vector<MemberInfo *> members;

    virtual bool is_record () const { return true; };
    virtual TypeDescriptor * get_record () {
	return TypeDescriptor::Convert (info);
    };

    RecordInfo (TypeDescriptor * record);

    virtual void init3 ();

    virtual void generate_cpp_header_body (FILE * header);
    virtual void generate_cpp_inline_body (FILE * inlin);
    virtual void generate_cpp_source_body (FILE * source);

    virtual void emit_members (FILE *, int indent);
};

struct VariantInfo : public CompositeInfo {
    set<CompositeInfo *> subtypes;
    set<CompositeInfo *> all_subtypes;

    set<MemberInfo *> members;

    virtual bool is_variant () const { return true; };
    virtual Variant * get_variant () {
	return Variant::Convert (info);
    };

    VariantInfo (Variant * variant) {
	init (variant, variant->get_name ());
	
	Array * array (variant->get_choices ());
	int size (array->getLength ());
	for (int i = 0; i < size; i++) {
	    Record * type (array->at (i));
	    subtypes.insert (CompositeInfo::Create (type));
	}
    };

    virtual void init3 ();

    virtual void generate_cpp_header_body (FILE *);
    virtual void generate_cpp_inline_body (FILE *);
    virtual void generate_cpp_source_body (FILE *);

    virtual void emit_members (FILE *, int indent);
};

struct ModuleInfo 
{
    Module * info;

    vector<CompositeInfo *> types;

    char * name;

    char * path_prefix;
    char * header_path_prefix;
    char * header_path;
    char * inline_path;
    char * source_path;
    char * init_filename;
    char * decl_filename;
    char * make_filename;

    char * cpp_namespace;
    char * class_prefix;
    char * typedescriptor_prefix;
    char * module_prefix;
    char * library;

    char * header_prefix;
    char * namespace_prefix;

    ModuleInfo (Record * info1);

    void generate_cpp ();
    void generate_cpp_decl (FILE * decl);
    void generate_cpp_init (FILE * init);
    void generate_cpp_make (FILE * make);

    void record_dependencies ();
};

struct SystemInfo 
{
    map<Type, LeafInfo *> types;
    set<ModuleInfo *> modules;

    char * rna_prefix;
    bool verbose;

    static SystemInfo * Instance;

    SystemInfo (char * rna_prefix1) :
	rna_prefix (rna_prefix1),
	verbose (false) {};
    
    static void Initialize (char * rna_prefix = "RNA::") {
	Instance = new SystemInfo (rna_prefix);
    };

    static LeafInfo * get_type (Type type) {
	RNA_ASSERT (Instance != 0);

	map<Type,LeafInfo *>::iterator it (Instance->types.find (type));
	if (it == Instance->types.end ()) {
	    return Instance->types [type] = new LeafInfo (type);
	} else
	    return it->second;
    };

    static void register_context (StrContext * context, vector<ModuleInfo *> * outputs = 0) {
        for (Context::Binding * binding = context->_bindings;
	     binding; binding = binding->next) {
	    char * label_name = binding->handle.readStr ();
	    if (binding->resolved) {
		if (binding->type == RECORD) {
		    Record * record = binding->definition.record;
		    if (record->getTypeInfo () == Module::TypeInfo ()) {
			ModuleInfo * module = new ModuleInfo (record);
			SystemInfo::Instance->modules.insert (module);
			if (SystemInfo::Instance->verbose)
			    fprintf (stderr, "\tregistering module %s\n",
				     module->name);
			if (outputs) {
			    outputs->push_back (module);
			    if (SystemInfo::Instance->verbose)
				fprintf (stderr, "\tfound module %s\n",
					 module->name);
			}
		    }
		}
	    } else {
		if (SystemInfo::Instance->verbose)
		    fprintf (stderr, "warning: unresolved label %s\n", label_name);
	    }
	}
    };
};

char * 
LeafInfo::type_name () 
{
    switch (type) {
    // @@@ TYPELIST @@@
    case BOOL: return "Bool";
    Case CHAR: return "Char";
    Case INT: return "Int";
    Case FLOAT: return "Float";
    Case PTR: return "Ptr";
    Case REF: return "Ref";
    Case STR: return "Str";
    Case RECORD: return "Record *";
    Case ARRAY: return "Array *";
    Case LIST: return "List *";
    Default: 
	RNA_SHOULD_NOT_REACH_HERE;
    }
    return "*unknown*";
}

char * 
LeafInfo::fq_type_name () 
{
    char * result = 0;
    asprintf (&result, "%s%s", SystemInfo::Instance->rna_prefix, type_name ());
    return result;
}

char * 
CompositeInfo::fq_type_name () 
{
    char * result;
    asprintf (&result, "%s%s *", module->namespace_prefix, class_name);
    return result;
}

char * 
CompositeInfo::conversion (char * base) 
{
    char * result;
    asprintf (&result, "%s%s::Convert (%s.readRecord ())", 
	      module->namespace_prefix, class_name, base);
    return result;
}

void
CompositeInfo::generate_cpp_header ()
{
	char * header_filename = 0;
	asprintf (&header_filename, "%s/%s/%s/%s.h", 
		  module->path_prefix, module->header_path_prefix,
		  module->header_path, name);
	FILE * header = my_fopen (header_filename, "w");

	print_c_header (header);
	print_indent (header, 0, "#ifndef %sCLASS_%s_H\n", 
		      module->header_prefix, macro_name);
	print_indent (header, 0, "#define %sCLASS_%s_H\n", 
		      module->header_prefix, macro_name);
	print_indent (header, 0, "\n");

	print_c_comment_block (header, "Forward Declarations for Class %s", class_name);
	
	for (set<CompositeInfo *, ltCompositeInfo>::iterator it (forward_declarations.begin ());
	     it != forward_declarations.end (); it++) {
	    CompositeInfo * ci (*it);
	    print_indent (header, 0, "");
	    if (ci->module->cpp_namespace) {
		fprintf (header, "namespace %s { ", ci->module->cpp_namespace);
	    }
	    fprintf (header, "class %s;", ci->class_name);
	    if (ci->module->cpp_namespace) {
		fprintf (header, " }");
	    }
	    fprintf (header, "\n");
	}

	generate_cpp_header_body (header);

	print_indent (header, 0, "#endif /* %sCLASS_%s_H */\n", 
		      module->header_prefix, macro_name);
	print_indent (header, 0, "\n");

	fclose (header);
}

void
CompositeInfo::generate_cpp_inline ()
{
	char * inline_filename = 0;
	asprintf (&inline_filename, "%s/%s/%s/%s.inline.cc",
		  module->path_prefix, module->header_path_prefix,
		  module->inline_path, name);
	FILE * inlin = my_fopen (inline_filename, "w");

	print_c_header (inlin);
	print_indent (inlin, 0, "#ifndef RNA_INSIDE_HEADER\n");
	print_indent (inlin, 0, "# include <rna/rna.h>\n");
	print_indent (inlin, 0, "# include \"%s\"\n", module->decl_filename);
	print_indent (inlin, 0, "#endif /* RNA_INSIDE_HEADER */\n");
	print_indent (inlin, 0, "\n");
	print_indent (inlin, 0, "#ifdef %s_DEFINE_INLINE_FUNCTIONS_NOW\n",
		      module->header_prefix);
	print_indent (inlin, 0, "\n");

	print_c_comment_block (inlin, "Forward Declarations for Class %s", class_name);
	
	for (set<CompositeInfo *, ltCompositeInfo>::iterator it (forward_declarations.begin ());
	     it != forward_declarations.end (); it++) {
	    CompositeInfo * ci (*it);
	    print_indent (inlin, 0, "#include \"%s/%s.h\"\n", 
			  ci->module->header_path, ci->name);
	}

	generate_cpp_inline_body (inlin);

	print_indent (inlin, 0, "#endif /* %s_DEFINE_INLINE_FUNCTIONS_NOW */\n", 
		      module->header_prefix);
	print_indent (inlin, 0, "\n");

	fclose (inlin);
}

void
CompositeInfo::generate_cpp_source ()
{
	char * source_filename = 0;
	asprintf (&source_filename, "%s/%s/%s.cc", 
		  module->path_prefix, module->source_path, name);
	FILE * source = my_fopen (source_filename, "w");
	
	print_c_header (source);
	print_indent (source, 0, "#include <rna/rna.h>\n");
	print_indent (source, 0, "#include \"%s\"\n", module->decl_filename);
	print_indent (source, 0, "\n");

	generate_cpp_source_body (source);

	fclose (source);
}

map<Record *, CompositeInfo *> CompositeInfo::type_table;

CompositeInfo * 
CompositeInfo::Create (Record * info1) 
{
    if (type_table.find (info1) != type_table.end ())
	return type_table [info1];
    
    if (info1->getTypeInfo () == TypeDescriptor::TypeInfo ())
	return new RecordInfo (TypeDescriptor::Convert (info1));
    else if (info1->getTypeInfo () == Variant::TypeInfo ())
	return new VariantInfo (Variant::Convert (info1));
    else
	RNA_SHOULD_NOT_REACH_HERE;
}

RecordInfo::RecordInfo (TypeDescriptor * record) 
{
    init (record, record->get_name ());
    
    int size (record->get_size ());
    members.resize (size);
    for (int i = 0; i < size; i++)
	members [i] = new MemberInfo (record, i);
}

SystemInfo * SystemInfo::Instance = 0;

LeafInfo * 
LeafInfo::For (Type type)
{
    return SystemInfo::get_type (type);
}

void 
CompositeInfo::init (Record *info1, char * name1) 
{
    type_table.insert (pair<Record*,CompositeInfo*> (info1, this));
    info = info1;
    name = strdup (name1);

    init3_done = false;
}

void
CompositeInfo::init2 (ModuleInfo * module1)
{
    module = module1;

    char * name2 = fix_prefix (name, module->typedescriptor_prefix, module->class_prefix);

    macro_name = make_macro_name (name2);
    class_name = make_class_name (name2);
    member_name = make_member_name (name2);

    free (name2);
}

void
print_set (FILE * f, char * name, set<CompositeInfo *> & my_set)
{
    fprintf (f, "\t%s:", name);
    for (set<CompositeInfo *>::iterator it (my_set.begin ());
	 it != my_set.end (); it++) {
	CompositeInfo * ci (*it);
	fprintf (f, " %s", ci->name);
    }
    fprintf (f, "\n");
}

void 
ModuleInfo::record_dependencies () 
{
    for (int i = 0; i < types.size (); i++)
	types [i]->init3 ();

    if (SystemInfo::Instance->verbose) {
	fprintf (stderr, "init3 finished:\n");
	for (int i = 0; i < types.size (); i++) {
	    fprintf (stderr, "%d: %s %s\n", i, 
		     types [i]->is_record () ? "record" :
		     types [i]->is_variant () ? "variant" : 
		     "*unknown*",
		     types [i]->name);
	    print_set (stderr, "supertypes", types [i]->supertypes);
	    print_set (stderr, "all_supertypes", types [i]->all_supertypes);
	    if (types [i]->is_variant ()) {
		print_set (stderr, "subtypes", ((VariantInfo *) types [i])->subtypes);
		print_set (stderr, "all_subtypes", ((VariantInfo *) types [i])->all_subtypes);
	    }
	}
    }
}

void
RecordInfo::init3 ()
{
    if (init3_done)
	return;

    for (int j = 0; j < members.size (); j++)
	if (members [j]->type->is_composite ())
	    forward_declarations.insert ((CompositeInfo *) members [j]->type);

    init3_done = true;
}

void
VariantInfo::init3 ()
{
    if (init3_done)
	return;

    for (set<CompositeInfo *>::iterator it (subtypes.begin ());
	 it != subtypes.end (); it++) {
	CompositeInfo * ci (*it);
	ci->init3 ();
    }

    all_subtypes.insert (this);
    for (set<CompositeInfo *>::iterator it (subtypes.begin ());
	 it != subtypes.end (); it++) {
	CompositeInfo * ci (*it);

	if (ci->is_record ())
	    all_subtypes.insert (ci);
	else if (ci->is_variant ()) {
	    VariantInfo * vi ((VariantInfo *) ci);
	    all_subtypes.insert (vi->all_subtypes.begin (), vi->all_subtypes.end ());
	} else
	    RNA_SHOULD_NOT_REACH_HERE;
    }

    for (set<CompositeInfo *>::iterator it (subtypes.begin ());
	 it != subtypes.end (); it++)
	(*it)->supertypes.insert (this);

    for (set<CompositeInfo *>::iterator it (all_subtypes.begin ());
	 it != all_subtypes.end (); it++)
	(*it)->all_supertypes.insert (this);

    forward_declarations.insert (all_subtypes.begin (), all_subtypes.end ());
    for (set<CompositeInfo *>::iterator it (all_subtypes.begin ());
	 it != all_subtypes.end (); it++) {
	CompositeInfo * ci (*it);

	forward_declarations.insert (ci);
	if (ci->is_record ()) {
	    RecordInfo * ri ((RecordInfo *) ci);	    
	    for (int i = 0; i < ri->members.size (); i++)
		if (ri->members [i]->type->is_composite ())
		    forward_declarations.insert ((CompositeInfo *) ri->members [i]->type);
	}
    }
    
    init3_done = true;
}

ModuleInfo::ModuleInfo (Record * info1) 
{
    // store Rna info
    RNA_ASSERT (info1->getTypeInfo () == Module::TypeInfo ());
    info = (Module *) info1;

    // get type list
    Array * array (info->get_contents ());
    int length (array->getLength ());
    types.resize (length);
    for (int i = 0; i < length; i++)
	types [i] = CompositeInfo::Create (array->at (i));

    // get configuration attributes
    name = info->get_name ().readStr ();
    typedescriptor_prefix = read_attribute (info, "typedescriptor_prefix", "");
    module_prefix = read_attribute (info, "cpp_module_prefix", "");
    path_prefix = read_attribute (info, "cpp_path_prefix", ".");
    header_path_prefix = read_attribute (info, "cpp_header_path_prefix", ".");
    header_path = read_attribute (info, "cpp_header_path", ".");
    inline_path = read_attribute (info, "cpp_inline_path", ".");
    source_path = read_attribute (info, "cpp_source_path", ".");
    decl_filename = read_attribute (info, "cpp_decl_filename", "decl.h");
    init_filename = read_attribute (info, "cpp_init_filename", "init.cc");
    make_filename = read_attribute (info, "cpp_make_filename", "Makefile");
    cpp_namespace = read_attribute (info, "cpp_namespace", 0);
    class_prefix = read_attribute (info, "cpp_class_prefix", "");
    library = read_attribute (info, "cpp_lib_filename", "lib.a");

    // compute header prefix
    if (cpp_namespace)
	header_prefix = make_macro_name (cpp_namespace);
    else
	header_prefix = strdup ("USER");
    if (module_prefix && strcmp (module_prefix, "") != 0) {
	char * module_prefix_upper = make_macro_name (module_prefix);
	char * new_header_prefix = (char *) malloc 
	    (strlen (header_prefix) + strlen (module_prefix_upper) + 2);
	strcpy (new_header_prefix, header_prefix);
	strcat (new_header_prefix, "_");
	strcat (new_header_prefix, module_prefix_upper);
	free (header_prefix);
	free (module_prefix_upper);
	header_prefix = new_header_prefix;
    }

    // compute namespace prefix
    if (cpp_namespace)
	asprintf (&namespace_prefix, "%s::", cpp_namespace);
    else
	namespace_prefix = "";

    if (SystemInfo::Instance->verbose)
	fprintf (stderr, "%s: using namespace %s (prefix: %s)\n",
		 name, cpp_namespace, namespace_prefix);
	
    // setup type information
    for (int i = 0; i < types.size (); i++)
	types [i]->init2 (this);
}

void 
ModuleInfo::generate_cpp () 
{
    if (decl_filename)
	if (strcmp (decl_filename, "") != 0) {
	    if (SystemInfo::Instance->verbose)
		fprintf (stderr, "writing decl file %s\n", decl_filename);

	    char * filename = 0;
	    asprintf (&filename, "%s/%s/%s", path_prefix, header_path_prefix, decl_filename);
	    FILE * decl = my_fopen (filename, "w");
	    free (filename);
	    generate_cpp_decl (decl);
	    fclose (decl);
	}

    if (init_filename)
	if (strcmp (init_filename, "") != 0) {
	    if (SystemInfo::Instance->verbose)
		fprintf (stderr, "writing init file %s\n", init_filename);
	    
	    char * filename = 0;
	    asprintf (&filename, "%s/%s/%s", path_prefix, source_path, init_filename);
	    FILE * init = my_fopen (filename, "w");
	    free (filename);
	    generate_cpp_init (init);
	    fclose (init);
	}
    
    if (make_filename)
	if (strcmp (make_filename, "") != 0) {
	    if (SystemInfo::Instance->verbose)
		fprintf (stderr, "writing make file %s\n", make_filename);
	
	    char * filename = 0;
	    asprintf (&filename, "%s/%s", path_prefix, make_filename);
	    FILE * make = my_fopen (filename, "w");
	    free (filename);
	    generate_cpp_make (make);
	    fclose (make);
	}

    for (int i = 0; i < types.size (); i++)
	types [i]->generate_cpp ();
}

void 
ModuleInfo::generate_cpp_decl (FILE * decl) 
{
    int i;

    print_c_header (decl);
    print_indent (decl, 0, "#ifndef %s_H\n", header_prefix);
    print_indent (decl, 0, "#define %s_H\n", header_prefix);
    print_indent (decl, 0, "\n");

    print_indent (decl, 0, "#define %s_INSIDE_HEADER\n", header_prefix);
    print_indent (decl, 0, "#ifdef RNA_USE_INLINE\n");
    print_indent (decl, 0, "# define %s_DEFINE_INLINE_FUNCTIONS_NOW\n", header_prefix);
    print_indent (decl, 0, "#else /* !RNA_USE_INLINE */\n");
    print_indent (decl, 0, "# undef %s_DEFINE_INLINE_FUNCTIONS_NOW\n", header_prefix);
    print_indent (decl, 0, "#endif /* RNA_USE_INLINE */\n");
    print_indent (decl, 0, "\n");
	
    print_c_comment_block (decl, "Class Declarations");
    if (cpp_namespace) {
	print_indent (decl, 0, "namespace %s {\n", cpp_namespace);
	print_indent (decl, 0, "\n");
    }
    for (i = 0; i < types.size (); i++)
	print_indent (decl, 0, "class %s;\n", 
		      types [i]->class_name);
    if (cpp_namespace) {
	print_indent (decl, 0, "\n");
	print_indent (decl, 0, "} /* namespace %s */\n", cpp_namespace);
    }
	
    print_c_comment_block (decl, "Class Definitions");
    for (i = 0; i < types.size (); i++)
	print_indent (decl, 0, "#include \"%s/%s.h\"\n", 
		      header_path, types [i]->name);
	
    print_c_comment_block (decl, "Inline Implementations");
    for (i = 0; i < types.size (); i++)
	print_indent (decl, 0, "#include \"%s/%s.inline.cc\"\n", 
		      inline_path, types [i]->name);
	
    print_c_comment_block (decl, "System Initialization");
    if (cpp_namespace) {
	print_indent (decl, 0, "namespace %s {\n", cpp_namespace);
	print_indent (decl, 0, "\n");
    }
    print_indent (decl, 0, "%sContext * %sInitialize (%sContext * context = 0);\n", 
		  SystemInfo::Instance->rna_prefix, module_prefix, SystemInfo::Instance->rna_prefix);
    if (cpp_namespace) {
	print_indent (decl, 0, "\n");
	print_indent (decl, 0, "} /* namespace %s */\n", cpp_namespace);
    }
    print_indent (decl, 0, "\n");
    print_indent (decl, 0, "#undef %s_INSIDE_HEADER\n", header_prefix);
    print_indent (decl, 0, "#ifdef RNA_USE_INLINE\n");
    print_indent (decl, 0, "# undef %s_DEFINE_INLINE_FUNCTIONS_NOW\n", header_prefix);
    print_indent (decl, 0, "#else /* !RNA_USE_INLINE */\n");
    print_indent (decl, 0, "# define %s_DEFINE_INLINE_FUNCTIONS_NOW\n", header_prefix);
    print_indent (decl, 0, "#endif /* RNA_USE_INLINE */\n");
    print_indent (decl, 0, "\n");
    print_indent (decl, 0, "#endif /* %s_H */\n", header_prefix);
    print_indent (decl, 0, "\n");
}

void 
ModuleInfo::generate_cpp_init (FILE * init) 
{
    int i;

    print_c_header (init);
    print_indent (init, 0, "#include <rna/rna.h>\n");
    print_indent (init, 0, "#include \"%s\"\n", decl_filename);
    print_indent (init, 0, "\n");
    if (cpp_namespace) {
	print_indent (init, 0, "namespace %s {\n", cpp_namespace);
	print_indent (init, 0, "\n");
    }
    print_indent (init, 0, "%sContext *\n", SystemInfo::Instance->rna_prefix);
    print_indent (init, 0, "%sInitialize (%sContext * context)\n", 
		  module_prefix, SystemInfo::Instance->rna_prefix);
    print_indent (init, 0, "{\n");
    print_indent (init, 4, "if (context == 0)\n");
    print_indent (init, 8, "context = new %sStrContext;\n", SystemInfo::Instance->rna_prefix);
    print_indent (init, 0, "\n");
    for (i = 0; i < types.size (); i++)
	if (types [i]->is_record ())
	    print_indent (init, 4, "%s::Initialize (context);\n", 
			  types [i]->class_name);
    print_indent (init, 0, "\n");
    print_indent (init, 4, "return context;\n");
    print_indent (init, 0, "}\n");
    if (cpp_namespace) {
	print_indent (init, 0, "\n");
	print_indent (init, 0, "} /* namespace %s */\n", cpp_namespace);
    }
    print_indent (init, 0, "\n");
}

void 
ModuleInfo::generate_cpp_make (FILE * make) 
{
    char * path_prefix (make_makefile_env (this->path_prefix));
    char * header_path_prefix (make_makefile_env (this->header_path_prefix));
    char * header_path (make_makefile_env (this->header_path));
    char * inline_path (make_makefile_env (this->inline_path));
    char * source_path (make_makefile_env (this->source_path));
    char * init_filename (make_makefile_env (this->init_filename));
    char * decl_filename (make_makefile_env (this->decl_filename));
    char * make_filename (make_makefile_env (this->make_filename));

    print_shell_header (make, "Makefile");
    print_indent (make, 0, "VPATH =");
    fprintf (make, " %s/%s", path_prefix, source_path);
    fprintf (make, " %s/%s/%s", path_prefix, header_path_prefix, inline_path);
    fprintf (make, "\n");

    print_indent (make, 0, "CXXINCLUDE =");
    for (set<ModuleInfo *>::iterator it (SystemInfo::Instance->modules.begin ());
	 it != SystemInfo::Instance->modules.end (); it++) {
	ModuleInfo * module (*it);
	char * path_prefix (make_makefile_env (module->path_prefix));
	char * header_path_prefix (make_makefile_env (module->header_path_prefix));
	fprintf (make, " -I%s/%s", path_prefix, header_path_prefix);
	free (path_prefix);
	free (header_path_prefix);
    }
    fprintf (make, "\n");
    print_indent (make, 0, "CXXFLAGS = -MD $(RNAFLAGS) $(CXXINCLUDE) $(CXXDEBUG)\n");
    print_indent (make, 0, "\n");

    if (library) {
	print_indent (make, 0, "AR = ar\n");
	print_indent (make, 0, "ARFLAGS = rv\n");
	print_indent (make, 0, "\n");
    }
	
    print_indent (make, 0, "sources = \\\n");
    if (init_filename) {
	print_indent (make, 8, "%s \\\n", init_filename);	    
	print_indent (make, 8, "\\\n");
    }
    for (int i = 0; i < types.size (); i++) {
	print_indent (make, 8, "%s.cc \\\n", types [i]->name);
	print_indent (make, 8, "%s.inline.cc \\\n", types [i]->name);
    }
    print_indent (make, 0, "\n");

    print_indent (make, 0, "OBJS = $(sources:.cc=.o)\n");
    print_indent (make, 0, "\n");
    if (library) {
	print_indent (make, 0, "LIB = %s/%s\n", path_prefix, library);
	print_indent (make, 0, "\n");
    }
	
    if (library) {
	print_indent (make, 0, "all: $(LIB)\n", library);
	print_indent (make, 0, "\n");
	print_indent (make, 0, "$(LIB): $(OBJS)\n");
	print_indent (make, 8, "@if [ -f $(LIB) ] ; then \\\n");
	print_indent (make, 12, "rm $(LIB) ;\\\n");
	print_indent (make, 8, "fi\n");
	print_indent (make, 8, "$(AR) $(ARFLAGS) $(LIB) $(OBJS)\n");
	print_indent (make, 0, "\n");
    } else {
	print_indent (make, 0, "all: $(OBJS)\n");
	print_indent (make, 0, "\n");
    }

    print_indent (make, 0, "-include $(sources:.cc=.d)\n");
    print_indent (make, 0, "\n");

    if (library) {
	print_indent (make, 0, "realclean: clean\n");
	print_indent (make, 8, "@echo rm $(LIB)\n");
	print_indent (make, 8, "@for i in $(LIB) ; do \\\n");
	print_indent (make, 12, "if [ -f $$i ] ; then \\\n");
	print_indent (make, 16, "rm $$i ;\\\n");
	print_indent (make, 12, "fi \\\n");
	print_indent (make, 8, "done\n");
	print_indent (make, 0, "\n");
    }

    print_indent (make, 0, "clean:\n");
    print_indent (make, 8, "@echo rm *.o *.d\n");
    print_indent (make, 8, "@for i in *.o *.d ; do \\\n");
    print_indent (make, 12, "if [ -f $$i ] ; then \\\n");
    print_indent (make, 16, "rm $$i ;\\\n");
    print_indent (make, 12, "fi \\\n");
    print_indent (make, 8, "done\n");
    print_indent (make, 0, "\n");

    free (path_prefix);
    free (header_path_prefix);
    free (header_path);
    free (inline_path);
    free (source_path);
    free (init_filename);
    free (decl_filename);
    free (make_filename);
}

void
CompositeInfo::generate_cpp ()
{
    if (SystemInfo::Instance->verbose)
	fprintf (stderr, "processing %s\n", name);
    
    generate_cpp_header ();
    generate_cpp_inline ();
    generate_cpp_source ();
}

void 
RecordInfo::generate_cpp_header_body (FILE * header)
{
    int j;

    print_c_comment_block (header, "Definition of Class %s", class_name);

    if (module->cpp_namespace) {
	print_indent (header, 0, "namespace %s {\n", module->cpp_namespace);
	print_indent (header, 0, "\n");
    }

    print_indent (header, 0, "class %s : public %sRecord\n", 
		  class_name, SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "{\n");
    print_indent (header, 0, "public:\n");
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Constants\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "static const %sIndex IDX_%s = %d;\n",
		      SystemInfo::Instance->rna_prefix, members [j]->macro_name, j);
    print_indent (header, 4, "static const %sSize SIZE = %d;\n", 
		  SystemInfo::Instance->rna_prefix, members.size ());
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Instance Creation\n");
    print_indent (header, 4, "RNA_INLINE %s (", class_name);
    for (j = 0; j < members.size (); j++) 
	fprintf (header, "%s%sRna %s",
		 j == 0 ? "" : ", ", 
		 SystemInfo::Instance->rna_prefix,
		 members [j]->member_name);
    fprintf (header, ");\n");
    print_indent (header, 4, "static RNA_INLINE %s * Create (%sMemory * s", 
		  class_name, SystemInfo::Instance->rna_prefix);
    for (j = 0; j < members.size (); j++)
	fprintf (header, ", %sRna %s", SystemInfo::Instance->rna_prefix, members [j]->member_name);
    fprintf (header, ");\n");
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Type Checking\n");
    print_indent (header, 4, "static RNA_INLINE %s * Convert (%sRecord *);\n", 
		  class_name, SystemInfo::Instance->rna_prefix);
    print_indent (header, 4, "static RNA_INLINE %sBool IsA (%sRecord *);\n", 
		  SystemInfo::Instance->rna_prefix, SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// TypeInfo\n");
    print_indent (header, 4, "static RNA_INLINE %sTypeDescriptor * TypeInfo ();\n", SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Access - Read\n");
    for (j = 0; j < members.size (); j++) 
	print_indent (header, 4, "RNA_INLINE %s get_%s ();\n", 
		      members [j]->type->fq_type_name (),
		      members [j]->member_name);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Access - Write\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "RNA_INLINE void set_%s (%sRna);\n",
		      members [j]->member_name, SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Class Initialization\n");
    print_indent (header, 4, "static void Initialize (%sContext * context = 0);\n", 
		  SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
    print_indent (header, 0, "protected:\n");
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// TypeInfo\n");
    print_indent (header, 4, "static %sTypeDescriptor * _TypeInfo;\n", SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
    print_indent (header, 0, "private:\n");
    print_indent (header, 0, "\n");
    emit_members (header, 4);
    print_indent (header, 0, "\n");
    print_indent (header, 0, "};\n");
    print_indent (header, 0, "\n");

    if (module->cpp_namespace) {
	print_indent (header, 0, "} /* namespace %s */\n", module->cpp_namespace);
	print_indent (header, 0, "\n");
    }
}

void 
RecordInfo::emit_members (FILE * f, int indent)
{
    print_indent (f, indent, "// Members of %s\n", class_name);
    for (int j = 0; j < members.size (); j++) 
	print_indent (f, indent, "%s _%s;\n", 
		      members [j]->type->fq_type_name (),
		      members [j]->member_name);
}

void 
RecordInfo::generate_cpp_inline_body (FILE * inlin) 
{
    int j;

    print_c_comment_block (inlin, "Instance Creation");
    print_indent (inlin, 0, "RNA_INLINE\n");
    print_indent (inlin, 0, "%s%s::%s (", 
		  module->namespace_prefix, class_name, class_name);
    for (j = 0; j < members.size (); j++)
	fprintf (inlin, "%s%sRna %s",
		 j == 0 ? "" : ", ", 
		 SystemInfo::Instance->rna_prefix,
		 members [j]->member_name);
    fprintf (inlin, ")\n");
    print_indent (inlin, 0, "{\n");
    for (j = 0; j < members.size (); j++)
	print_indent (inlin, 4, "set (%d, %s);\n",
		      j, members [j]->member_name);
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
    print_indent (inlin, 0, "RNA_INLINE %s%s *\n", module->namespace_prefix, class_name);
    print_indent (inlin, 0, "%s%s::Create (%sMemory * s", module->namespace_prefix, 
		  class_name, SystemInfo::Instance->rna_prefix);
    for (j = 0; j < members.size (); j++)
	fprintf (inlin, ", %sRna %s", SystemInfo::Instance->rna_prefix, members [j]->member_name);
    fprintf (inlin, ")\n");
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "%s * result = (%s *) %sRecord::Create (s, _TypeInfo);\n",
		  class_name, class_name, SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "\n");
    for (j = 0; j < members.size (); j++)
	print_indent (inlin, 4, "result->set (%d, %s);\n",
		      j, members [j]->member_name);
    print_indent (inlin, 0, "\n");
    print_indent (inlin, 4, "return result;\n");
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
	
    print_c_comment_block (inlin, "Type Checking");
    print_indent (inlin, 0, "RNA_INLINE %s%s *\n", module->namespace_prefix, class_name);
    print_indent (inlin, 0, "%s%s::Convert (%sRecord * record)\n", 
		  module->namespace_prefix, class_name, SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "if (record == 0)\n");
    print_indent (inlin, 8, "return 0;\n");
    print_indent (inlin, 4, "if (record->getTypeInfo () == _TypeInfo)\n");
    print_indent (inlin, 8, "return (%s *) record;\n", class_name);
    print_indent (inlin, 4, "else\n");
    print_indent (inlin, 8, "RNA_SHOULD_NOT_REACH_HERE;\n");
    print_indent (inlin, 4, "return (%s%s*) 0;\n", module->namespace_prefix, class_name);
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
    print_indent (inlin, 0, "RNA_INLINE %sBool\n", SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "%s%s::IsA (%sRecord * record)\n", 
		  module->namespace_prefix, class_name, 
		  SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "if (record)\n");
    print_indent (inlin, 8, "return record->getTypeInfo () == _TypeInfo;\n");
    print_indent (inlin, 4, "else\n");
    print_indent (inlin, 8, "return false;\n");
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
	
    print_c_comment_block (inlin, "TypeInfo");
    print_indent (inlin, 0, "RNA_INLINE %sTypeDescriptor *\n", SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "%s%s::TypeInfo ()\n", module->namespace_prefix, class_name);
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "return _TypeInfo;\n");
    print_indent (inlin, 0, "}\n");	
    print_indent (inlin, 0, "\n");
	
    print_c_comment_block (inlin, "Access - Read");
    for (j = 0; j < members.size (); j++) {
	print_indent (inlin, 0, "RNA_INLINE %s\n", 
		      members [j]->type->fq_type_name ());
	print_indent (inlin, 0, "%s%s::get_%s ()\n", 
		      module->namespace_prefix, class_name, 
		      members [j]->member_name);
	print_indent (inlin, 0, "{\n");
	char * item = 0;
	asprintf (&item, "get (%d)", j);
	print_indent (inlin, 4, "return %s;\n",
		      members [j]->type->conversion (item));
	free (item);
	print_indent (inlin, 0, "}\n");	
	print_indent (inlin, 0, "\n");
    }
    print_indent (inlin, 0, "\n");
	
    print_c_comment_block (inlin, "Access - Write");
    for (j = 0; j < members.size (); j++) {
	print_indent (inlin, 0, "RNA_INLINE void\n");
	print_indent (inlin, 0, "%s%s::set_%s (%sRna rna)\n",
		      module->namespace_prefix, class_name, 
		      members [j]->member_name, SystemInfo::Instance->rna_prefix);
	print_indent (inlin, 0, "{\n");
	print_indent (inlin, 4, "set (%d, rna);\n", j);
	print_indent (inlin, 0, "}\n");	
	print_indent (inlin, 0, "\n");
    }
    print_indent (inlin, 0, "\n");
}

void 
RecordInfo::generate_cpp_source_body (FILE * source) 
{
    print_c_comment_block (source, "TypeInfo");
    print_indent (source, 0, "%sTypeDescriptor * %s%s::_TypeInfo = 0;\n", 
		  SystemInfo::Instance->rna_prefix, module->namespace_prefix, class_name);
    print_indent (source, 0, "\n");

    print_c_comment_block (source, "Class Initialization");
    print_indent (source, 0, "void\n");
    print_indent (source, 0, "%s%s::Initialize (%sContext * context)\n", 
		  module->namespace_prefix, class_name, SystemInfo::Instance->rna_prefix);
    print_indent (source, 0, "{\n");
    print_indent (source, 4, "// step 1: look into context if applicable\n");
    print_indent (source, 4, "%sContext::Binding * binding = 0;\n", SystemInfo::Instance->rna_prefix);
    print_indent (source, 4, "if (context) {");
    print_indent (source, 8, "binding = context->lookup (\"%s\");\n", name);
    print_indent (source, 8, "if (binding->resolved) {\n");
    print_indent (source, 12, "RNA_ASSERT (binding->type == %sRECORD);\n", SystemInfo::Instance->rna_prefix);
    print_indent (source, 12, "_TypeInfo = %sTypeDescriptor::Convert (binding->definition.record);\n",
		  SystemInfo::Instance->rna_prefix);
    print_indent (source, 8, "}\n");
    print_indent (source, 0, "\n");
    print_indent (source, 8, "if (binding->type == %sANY)\n", SystemInfo::Instance->rna_prefix);
    print_indent (source, 12, "binding->type = %sRECORD;\n", SystemInfo::Instance->rna_prefix);
    print_indent (source, 8, "RNA_ASSERT (binding->type == %sRECORD);\n", 
		  SystemInfo::Instance->rna_prefix);
    print_indent (source, 4, "}\n");
    print_indent (source, 0, "\n");
    print_indent (source, 4, "// step 2: assemble TypeDescriptor if necessary\n");
    print_indent (source, 4, "%sArray * parts = %sArray::Create (%sMemory::Main (), %d, %sSTR);\n",
		  SystemInfo::Instance->rna_prefix, 
		  SystemInfo::Instance->rna_prefix, 
		  SystemInfo::Instance->rna_prefix, 
		  members.size (), 
		  SystemInfo::Instance->rna_prefix);
    for (int j = 0; j < members.size (); j++)
	print_indent (source, 4, "parts->atPut (%d, \"%s\");\n",
		      j, (char *) get_record ()->get_parts() [j]);
    print_indent (source, 0, "\n");
    print_indent (source, 4, "_TypeInfo = %sTypeDescriptor::Create (%sMemory::Main (), \"%s\", %d, \"%s\", parts);\n",
		  SystemInfo::Instance->rna_prefix, SystemInfo::Instance->rna_prefix,
		  name, members.size (), get_record ()->get_format ().readStr ());
    print_indent (source, 0, "\n");
    print_indent (source, 4, "// step 2a: transport some annotations (needs to be improved a lot!)\n");
    print_indent (source, 4, "%sList * attr = _TypeInfo->getAttr ();\n", SystemInfo::Instance->rna_prefix);
    for (List * list = get_record ()->getAttr (); list; list = list->getCdr ()) {
	if (list->getType () == STR) {
	    print_indent (source, 4, "attr = %sList::Create (%sMemory::Main (), "
			  "\"%s\", %sSTR, \"%s\", attr);\n",
			  SystemInfo::Instance->rna_prefix, 
			  SystemInfo::Instance->rna_prefix,
			  list->getTag (),
			  SystemInfo::Instance->rna_prefix,
			  list->getCar ().readStr ());
	}
    }
    print_indent (source, 4, "_TypeInfo->setAttr (attr);\n");
    print_indent (source, 0, "\n");
    print_indent (source, 4, "// step 3: register TypeDescriptor in context if applicable\n");
    print_indent (source, 4, "if (context && binding) {");
    print_indent (source, 8, "%sWord item;\n", SystemInfo::Instance->rna_prefix);
    print_indent (source, 8, "item.record = _TypeInfo;\n");
    print_indent (source, 8, "context->resolve (binding, item);\n");
    print_indent (source, 4, "}\n");
    print_indent (source, 0, "\n");
    print_indent (source, 4, "// step 4: check resulting TypeDescriptor\n");
    print_indent (source, 4, "RNA_ASSERT (_TypeInfo->check ());\n");
    print_indent (source, 0, "}\n");	
    print_indent (source, 0, "\n");

}

void 
VariantInfo::generate_cpp_header_body (FILE * header) 
{
    int j;

    print_c_comment_block (header, "Definition of Class %s", class_name);
	
    if (module->cpp_namespace) {
	print_indent (header, 0, "namespace %s {\n", module->cpp_namespace);
	print_indent (header, 0, "\n");
    }

    print_indent (header, 0, "class %s : public %sRecord\n", 
		  class_name, SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "{\n");
    print_indent (header, 0, "public:\n");
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Type Checking\n");
    print_indent (header, 4, "static RNA_INLINE %s * Convert (%sRecord *);\n", 
		  class_name, SystemInfo::Instance->rna_prefix);
    print_indent (header, 4, "static RNA_INLINE %sBool IsA (%sRecord *);\n", 
		  SystemInfo::Instance->rna_prefix, SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
#if 0
    print_indent (header, 4, "// Type Checking\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "RNA_INLINE %sBool is%s ();\n", 
		      SystemInfo::Instance->rna_prefix, members [j]->member_name);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Type Conversion\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "RNA_INLINE %s * as%s  ();\n", 
		      member_types [j], member_types [j]);
    print_indent (header, 0, "\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "RNA_INLINE operator %s * ();\n",
		      member_types [j]);
    print_indent (header, 0, "\n");
#if 0
    print_indent (header, 4, "// Access - Read\n");
    for (j = 0; j < members.size (); j++)
	print_indent (header, 4, "RNA_INLINE %s%s get_%s ();\n",
		      SystemInfo::Instance->rna_prefix, 
		      member_type ((Type) get_format [j]),
		      member_names [j]);
    print_indent (header, 0, "\n");
    print_indent (header, 4, "// Access - Write\n");
    for (j = 0; j < size; j++)
	print_indent (header, 4, "RNA_INLINE void set_%s (%sRna);\n",
		      member_names [j], SystemInfo::Instance->rna_prefix);
    print_indent (header, 0, "\n");
#endif
#endif
    print_indent (header, 0, "private:\n");
    print_indent (header, 0, "\n");
	
    print_indent (header, 4, "// Members\n");
    emit_members (header, 4);
    print_indent (header, 0, "\n");

    print_indent (header, 0, "};\n");
    print_indent (header, 0, "\n");

    if (module->cpp_namespace) {
	print_indent (header, 0, "} /* namespace %s */\n", module->cpp_namespace);
	print_indent (header, 0, "\n");
    }
}

void 
VariantInfo::emit_members (FILE * f, int indent)
{
    print_indent (f, indent, "// Members of %s\n", class_name);
    print_indent (f, indent, "union {\n");
    for (set<CompositeInfo *>::iterator it (all_subtypes.begin ());
	 it != all_subtypes.end (); it++) {
	CompositeInfo * ci (*it);
	if (ci->is_record ()) {
	    print_indent (f, indent+4, "struct {\n");
	    ci->emit_members (f, indent+8);
	    print_indent (f, indent+4, "} _%s;\n", ci->member_name);
	}
    }
    print_indent (f, indent, "};\n");
}

void 
VariantInfo::generate_cpp_inline_body (FILE * inlin) 
{
    int j;
	
    print_c_comment_block (inlin, "Type Checking");
    print_indent (inlin, 0, "RNA_INLINE %s%s *\n", module->namespace_prefix, class_name);
    print_indent (inlin, 0, "%s%s::Convert (%sRecord * record)\n", 
		  module->namespace_prefix, class_name, SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "if (record == 0)\n");
    print_indent (inlin, 8, "return (%s *) 0;\n", class_name);
    for (set<CompositeInfo *>::iterator it (all_subtypes.begin ());
	 it != all_subtypes.end (); it++) {
	CompositeInfo * ci (*it);
	if (ci->is_record ()) {
	    print_indent (inlin, 4, "else if (record->getTypeInfo () == %s%s::TypeInfo ())\n",
			  ci->module->namespace_prefix, ci->class_name);
	    print_indent (inlin, 8, "return (%s *) record;\n", class_name);
	}
    }
    print_indent (inlin, 4, "else\n");
    print_indent (inlin, 8, "RNA_SHOULD_NOT_REACH_HERE;\n");
    print_indent (inlin, 4, "return (%s *) 0;\n", class_name);
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
    print_indent (inlin, 0, "RNA_INLINE %sBool\n", SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "%s%s::IsA (%sRecord * record)\n", 
		  module->namespace_prefix, class_name, 
		  SystemInfo::Instance->rna_prefix);
    print_indent (inlin, 0, "{\n");
    print_indent (inlin, 4, "if (record == 0)\n");
    print_indent (inlin, 8, "return false;\n");
    for (set<CompositeInfo *>::iterator it (all_subtypes.begin ());
	 it != all_subtypes.end (); it++) {
	CompositeInfo * ci (*it);
	if (ci->is_record ()) {
	    print_indent (inlin, 4, "else if (record->getTypeInfo () == %s%s::TypeInfo ())\n",
			  ci->module->namespace_prefix, ci->class_name);
	    print_indent (inlin, 8, "return true;\n");
	}
    }
    print_indent (inlin, 4, "else\n");
    print_indent (inlin, 8, "return false;\n");
    print_indent (inlin, 0, "}\n");
    print_indent (inlin, 0, "\n");
}

void 
VariantInfo::generate_cpp_source_body (FILE * source) 
{
}


/////////////////////////////////////////////////////////////////////////////
//
// Main Program
//
/////////////////////////////////////////////////////////////////////////////

void
usage (FILE * f, int exit_code)
{
    fprintf (f, "usage: %s [OPTIONS] <RNA-File>\n", 
	     program_invocation_short_name);
    fprintf (f, "    -h           print this help message\n");
    fprintf (f, "    -i FILENAME  include additional RNA file\n");
    fprintf (f, "    -r PREFIX    RNA namespace prefix\n"); 
    fprintf (f, "    -v           verbose output\n");
    
    exit (exit_code);
}

int
main (int argc, char * argv [])
{
    int i, j;

    vector<char*> rna_includes;
    vector<ModuleInfo*> outputs;

    /////////////////////////////////////////////////////////////////////////////
    //
    // Initialization and Context Setup
    //
    /////////////////////////////////////////////////////////////////////////////

    RNA::Initialization::Initialize ();
    SystemInfo::Initialize ();

    /////////////////////////////////////////
    // determine program invocation
    /////////////////////////////////////////
    int c;
    if (argc == 1) 
	usage (stderr, EXIT_FAILURE);
    for (opterr = 0; (c = getopt (argc, argv, "d:hi:r:v")) != -1;)
	switch (c) {
	case 'h': usage (stdout, EXIT_SUCCESS);
	Case 'i': rna_includes.push_back (optarg);
	Case 'p': SystemInfo::Instance->rna_prefix = optarg;
	Case 'v': SystemInfo::Instance->verbose = true;
	Default:
	    fprintf (stderr, "Unknown option -%c\n", c);
	    usage (stderr, EXIT_FAILURE);
	}

    /////////////////////////////////////////
    // read include files
    /////////////////////////////////////////
    if (SystemInfo::Instance->verbose)
	fprintf (stderr, "reading include files\n");
    StrContext * parent_context = new StrContext;
    for (i = 0; i < rna_includes.size (); i++) {
	// skip files on the main list
	bool flag = true;
	for (j = optind; j < argc; j++)
	    if (strcmp (argv [j], rna_includes [i]) == 0) {
		flag = false;
		break;
	    }
	if (flag) {
	    if (SystemInfo::Instance->verbose)
		fprintf (stderr, "\treading file %s\n", rna_includes [i]);
	    File * file = new File (rna_includes [i], "r");
	    Serializer * serializer = Serializer::Create (file);
	    serializer->set_context (parent_context);
	    while (!file->atEnd ())
		serializer->read (ANY);
	    delete file;
	} else 
	    if (SystemInfo::Instance->verbose)
		fprintf (stderr, "\tskipping file %s\n", rna_includes [i]);
    }

    /////////////////////////////////////////
    // read input files
    /////////////////////////////////////////
    if (SystemInfo::Instance->verbose)
	fprintf (stderr, "reading input files\n");
    StrContext * context = new StrContext (parent_context);
    for (i = optind; i < argc; i++) {
	if (SystemInfo::Instance->verbose)
	    fprintf (stderr, "\treading file %s\n", argv [i]);
	File * file = new File (argv [i], "r");
	Serializer * serializer = Serializer::Create (file);
	serializer->set_context (context);
	while (!file->atEnd ())
	    serializer->read (ANY);
    }
    
    /////////////////////////////////////////
    // context registration
    /////////////////////////////////////////
    SystemInfo::Instance->register_context (parent_context);
    SystemInfo::Instance->register_context (context, & outputs);

    /////////////////////////////////////////
    // record depencendies
    /////////////////////////////////////////
    for (i = 0; i < outputs.size (); i++)
	outputs [i]->record_dependencies ();

    /////////////////////////////////////////
    // generate C++ code
    /////////////////////////////////////////
    for (i = 0; i < outputs.size (); i++)
	outputs [i]->generate_cpp ();

    exit (EXIT_SUCCESS);
    return 0;
}
