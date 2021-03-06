## Copyright (C) 1996 Auburn University.  All rights reserved.
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
##@deftypefn {Function File} {[@var{zer}, @var{pol}, @var{k}, @var{tsam}, @var{inname}, @var{outname}] =} sys2zp (@var{sys})
## Extract zero/pole/leading coefficient information from a system data
## structure
##
## See zp2sys for parameter descriptions.
##
## @strong{Example}
## @example
## octave:1> sys=ss2sys([1 -2; -1.1,-2.1],[0;1],[1 1]);
## octave:2> [zer,pol,k] = sys2zp(sys)
## zer = 3.0000
## pol =
##   -2.6953
##    1.5953
## k = 1
## @end example
## @end deftypefn

## Author: John Ingram <ingraje@eng.auburn.edu>
## Created: July 15, 1996

function [zer, pol, k, tsam, inname, outname] = sys2zp (sys)

  if(nargin != 1)
    usage("[zer,pol,k,tsam,inname,outname] = sys2zp(sys)");
  elseif( !is_struct(sys))
    error("sysconnect: sys must be in system data structure form")
  elseif (! is_siso(sys) )
    [n, nz, m, p] = sysdimensions(sys);
    error(["system is not SISO (",num2str(m)," inputs, ...
        ", num2str(p)," outputs"]);
  endif

  ## update zero-pole form
  sys = sysupdate(sys,"zp");

  zer = sys.zer;
  pol = sys.pol;
  k = sys.k;
  tsam    = sysgettsam(sys);
  inname  = sysgetsignals(sys,"in");
  outname = sysgetsignals(sys,"out");

endfunction


