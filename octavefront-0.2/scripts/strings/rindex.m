## Copyright (C) 1996 Kurt Hornik
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
## @deftypefn {Function File} {} rindex (@var{s}, @var{t})
## Return the position of the last occurrence of the string @var{t} in the
## string @var{s}, or 0 if no occurrence is found.  For example,
##
## @example
## rindex ("Teststring", "t")
##      @result{} 6
## @end example
##
## @strong{Note:}  This function does not work for arrays of strings.
## @end deftypefn

## Author: Kurt Hornik <Kurt.Hornik@ci.tuwien.ac.at>
## Adapted-By: jwe

function n = rindex (s, t)

  ## This is patterned after the AWK function of the same name.

  if (nargin != 2)
    usage ("rindex (s, t)");
  endif

  n = 0;

  if (isstr (s) && isstr (t))

    l_s = length (s);
    l_t = length (t);

    if (l_t <= l_s)
      tmp = l_s - l_t + 1;
      for idx = tmp : -1 : 1
        if (strcmp (substr (s, idx, l_t), t))
          n = idx;
          return;
        endif
      endfor
    endif

  else
    error ("rindex: expecting string arguments");
  endif

endfunction
