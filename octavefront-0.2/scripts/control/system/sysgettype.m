## Copyright (C) 1998 Auburn University.  All rights reserved.
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation; either version 2, or (at your option) any
## later version.
##
## Octave is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111 USA.

## -*- texinfo -*-
## @deftypefn {Function File} {} sysgettype (@var{sys})
## return the initial system type of the system
##
## @strong{Inputs}
## @var{sys}: system data structure
##
## @strong{Outputs}
## @var{systype}: string indicating how the structure was initially
## constructed:
## values: @code{"ss"}, @code{"zp"}, or @code{"tf"}
##
## @strong{Note} FIR initialized systems return @code{systype="tf"}.
## @end deftypefn

function systype = sysgettype (sys)

  if(!is_struct(sys))
    error("sysgettype: input sys is not a structure");
  endif

  typestr = list("tf","zp","ss");
  systype = nth(typestr,sys.sys(1) + 1);
endfunction
