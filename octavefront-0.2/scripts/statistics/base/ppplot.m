## Copyright (C) 1995, 1996, 1997  Kurt Hornik
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
## @deftypefn {Function File} {[@var{p}, @var{y}] =} ppplot (@var{x}, @var{dist}, @var{params})
## Perform a PP-plot (probability plot).
##
## If F is the CDF of the distribution @var{dist} with parameters
## @var{params} and @var{x} a sample vector of length @var{n}, the
## PP-plot graphs ordinate @var{y}(@var{i}) = F (@var{i}-th largest
## element of @var{x}) versus abscissa @var{p}(@var{i}) = (@var{i} -
## 0.5)/@var{n}.  If the sample comes from F, the pairs will
## approximately follow a straight line.
##
## The default for @var{dist} is the standard normal distribution.  The
## optional argument @var{params} contains a list of parameters of
## @var{dist}.  For example, for a probability plot of the uniform
## distribution on [2,4] and @var{x}, use
##
## @example
## ppplot (x, "uniform", 2, 4)
## @end example
##
## If no output arguments are given, the data are plotted directly.
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Perform a PP-plot (probability plot)

function [p, y] = ppplot (x, dist, ...)

  if (nargin < 1)
    usage ("ppplot (x, dist, params)");
  endif

  if (! is_vector (x))
    error ("ppplot: x must be a vector");
  endif

  s = sort (x);
  n = length (x);
  p = ((1 : n)' - 0.5) / n;
  if (nargin == 1)
    F = "stdnormal_cdf";
  else
    F = sprintf ("%s_cdf", dist);
  endif;
  if (nargin <= 2)
    y = feval (F, s);
  else
    y = feval (F, s, all_va_args);
  endif

  if (nargout == 0)
    axis ([0, 1, 0, 1]);
    gset nokey;
    plot (p, y);
  endif

endfunction
