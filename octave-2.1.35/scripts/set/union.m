## Copyright (C) 1996, 1997 John W. Eaton
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

## -*- texinfo -*-
## @deftypefn {Function File} {} union (@var{x}, @var{y})
## Return the set of elements that are in either of the sets @var{x} and
## @var{y}.  For example,
##
## @example
## @group
## union ([ 1, 2, 4 ], [ 2, 3, 5 ])
##      @result{} [ 1, 2, 3, 4, 5 ]
## @end group
## @end example
## @end deftypefn
## @seealso{create_set, intersection, and complement}

## Author: jwe

function y = union(a,b)

  if (nargin != 2)
    usage ("union(a,b)");
  endif

  if(isempty(a))
    y = create_set(b);
  elseif(isempty(b))
    y = create_set(a);
  else
    [nra, nca] = size(a);
    a = reshape(a,1,nra*nca);
    [nrb, ncb] = size(b);
    b = reshape(b,1,nrb*ncb);
    y = create_set([a, b]);
  endif

endfunction
