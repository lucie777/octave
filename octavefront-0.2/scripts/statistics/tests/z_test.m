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
## @deftypefn {Function File} {[@var{pval}, @var{z}] =} z_test (@var{x}, @var{m}, @var{v}, @var{alt})
## Perform a Z-test of the null hypothesis @code{mean (@var{x}) ==
## @var{m}} for a sample @var{x} from a normal distribution with unknown
## mean and known variance @var{v}.  Under the null, the test statistic
## @var{z} follows a standard normal distribution.
##
## With the optional argument string @var{alt}, the alternative of
## interest can be selected.  If @var{alt} is @code{"!="} or
## @code{"<>"}, the null is tested against the two-sided alternative
## @code{mean (@var{x}) != @var{m}}.  If @var{alt} is @code{">"}, the
## one-sided alternative @code{mean (@var{x}) > @var{m}} is considered.
## Similarly for @code{"<"}, the one-sided alternative @code{mean
## (@var{x}) < @var{m}} is considered.  The default is the two-sided
## case.
##
## The p-value of the test is returned in @var{pval}.
##
## If no output argument is given, the p-value of the test is displayed
## along with some information.
## @end deftypefn

## Author: KH <Kurt.Hornik@ci.tuwien.ac.at>
## Description: Test for mean of a normal sample with known variance

function [pval, z] = z_test (x, m, v, alt)

  if ((nargin < 3) || (nargin > 4))
    usage ("[pval, z] = z_test (x, m, v, alt)");
  endif

  if (! is_vector (x))
    error ("z_test: x must be a vector");
  endif
  if (! is_scalar (m))
    error ("z_test: m must be a scalar");
  endif
  if (! (is_scalar (v) && (v > 0)))
    error ("z_test: v must be a positive scalar");
  endif

  n = length (x);
  z = sqrt (n/v) * (sum (x) / n - m);
  cdf = stdnormal_cdf (z);

  if (nargin == 3)
    alt = "!=";
  endif

  if (! isstr (alt))
    error ("z_test: alt must be a string");
  elseif (strcmp (alt, "!=") || strcmp (alt, "<>"))
    pval = 2 * min (cdf, 1 - cdf);
  elseif (strcmp (alt, ">"))
    pval = 1 - cdf;
  elseif (strcmp (alt, "<"))
    pval = cdf;
  else
    error ("z_test: option %s not recognized", alt);
  endif

  if (nargout == 0)
    s = strcat ("Z-test of mean(x) == %g against mean(x) %s %g,\n",
                "with known var(x) == %g:\n",
                "  pval = %g\n");
    printf (s, m, alt, m, v, pval);
  endif

endfunction
