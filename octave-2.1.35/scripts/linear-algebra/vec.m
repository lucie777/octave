## Copyright (C) 1995, 1996  Kurt Hornik
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this file.  If not, write to the Free Software Foundation,
## 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

## -*- texinfo -*-
## @deftypefn {Function File} {} vec (@var{x})
## Return the vector obtained by stacking the columns of the matrix @var{x}
## one above the other.
## @end deftypefn

## See Magnus and Neudecker (1988), Matrix differential calculus with
## applications in statistics and econometrics.

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Created: 8 May 1995
## Adapted-By: jwe

function v = vec (x)

  if (nargin != 1)
    usage ("vec (x)");
  endif

  v = reshape (x, prod (size (x)), 1);

endfunction
