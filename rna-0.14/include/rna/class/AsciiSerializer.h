/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/11 08:05:04 $
 * $Source: /sys/cvs/rna/include/rna/class/AsciiSerializer.h,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_CLASS_ASCIISERIALIZER_H
#define RNA_CLASS_ASCIISERIALIZER_H

class AsciiSerializer : public Serializer
{
public:

    // Constructors/Destructors
    AsciiSerializer ();
    virtual ~AsciiSerializer ();

    // Transport
    virtual Rna read (Type type = RECORD);
    virtual void write (Rna, Type type = RECORD);
    
    // Testing
    virtual Bool isAscii ();

protected:

    // Types
    struct Snippet 
    {
	Str text;
	Int indent;

	Snippet * prev;
	Snippet * next;
	Snippet * up;
	Snippet * first;
    };
    class Snippets
    {
    public:
	Snippets (Stream *);
	~Snippets ();

	void indent (char * begin_marker);
	void unindent (char * end_marker);
	void separator ();
	void printf (const char * format, ...);
	void flush ();

    protected:
	enum Mode { IMMEDIATE, RETAINED };

	Mode mode;
	Snippet * start;
	Snippet * current;
	Int current_length;
	Int current_indent;    
	Stream * stream;
    };


    // Different types of Context, depending on
    // Serializer Type
    virtual StrContext * strcontext ();

    // Reading
    void read_whitespace ();
    Str read_name ();
    void check_type (Type & expected, Type found);
    Context::Binding * lookup (Rna name, Type & type);
    Bool read_item1 (Word * result,
		     Type type,
		     Record * container, Bool must_define);
    Bool read_item (Word * result,
		    Type & type,
		    Record * container, Bool must_define);

    // Writing
    void scan_item (Rna, Type, Int, Int);
    void write_item (Rna, Type, Snippets &, Int, Int);
    void write_composite_item (Rna, Type, Snippets &, Int, Int);
    void write_attr_list (List *, Snippets &, Int, Int);

    // Error handling
    void error (const char * message, ...);
};

#endif /* RNA_CLASS_ASCIISERIALIZER_H */


