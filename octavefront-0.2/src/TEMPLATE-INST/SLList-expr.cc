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

// Instantiate Lists of various values.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SLList.h"
#include "SLList.cc"

#include "ov.h"
#include "pt-exp.h"
#include "pt-id.h"
#include "pt-idx.h"

template class SLNode<tree_expression *>;
template class SLList<tree_expression *>;

template class SLNode<tree_identifier *>;
template class SLList<tree_identifier *>;

template class SLNode<tree_index_expression *>;
template class SLList<tree_index_expression *>;

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
