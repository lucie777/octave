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
## @deftypefn {Function File} {[@var{pval}, @var{t}, @var{df}] =} t_test (@var{x}, @var{m}, @var{alt})
## For a sample @var{x} from a normal distribution with unknown mean and
## variance, perform a t-test of the null hypothesis @code{mean
## (@var{x}) == @var{m}}.  Under the null, the test statistic @var{t}
## follows a Student distribution with @code{@var{df} = length (@var{x})
## - 1} degrees of freedom.
##
## With the optional argument string @var{alt}, the alternative of
## interest can be selected.  If @var{alt} is @code{"!="} or
## @code{"<>"}, the null is tested against the two-sided alternative
## @code{mean (@var{x}) != @var{m}}.  If @var{alt} is @code{">"}, the
## one-sided alternative @code{mean (@var{x}) > @var{m}} is considered.
## Similarly for @var{"<"}, the one-sided alternative @code{mean
## (@var{x}) < @var{m}} is considered,  The default is the two-sided
## case.
##
## The p-value of the test is returned in @var{pval}.
##
## If no output argument is given, the p-value of the test is displayed.
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Student's one-sample t test

function [pval, t, df] = t_test (x, m, alt)

  if ((nargin < 2) || (nargin > 3))
    usage ("[pval, t, df] = t_test (x, m, alt)");
  endif

  if (! is_vector (x))
    error ("t_test: x must be a vector");
  endif
  if (! is_scalar (m))
    error ("t_test: m must be a scalar");
  endif

  n   = length (x);
  df  = n - 1;
  t   = sqrt (n) * (sum (x) / n - m) / std (x);
  cdf = t_cdf (t, df);

  if (nargin == 2)
    alt  = "!=";
  endif

  if (! isstr (alt))
    error ("t_test: alt must be a string");
  endif
  if (strcmp (alt, "!=") || strcmp (alt, "<>"))
    pval = 2 * min (cdf, 1 - cdf);
  elseif strcmp (alt, ">")
    pval = 1 - cdf;
  elseif strcmp (alt, "<")
    pval = cdf;
  else
    error ("t_test: option %s not recognized", alt);
  endif

  if (nargout == 0)
    printf ("  pval: %g\n", pval);
  endif

endfunction
