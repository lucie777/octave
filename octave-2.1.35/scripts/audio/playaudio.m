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
## @deftypefn {Function File} {} playaudio (@var{name}, @var{ext})
## @deftypefnx {Function File} {} playaudio (@var{x})
## Plays the audio file @file{@var{name}.@var{ext}} or the audio data
## stored in the vector @var{x}.
## @end deftypefn
## @seealso{lin2mu, mu2lin, loadaudio, saveaudio, setaudio, and record}

## Author: AW <Andreas.Weingessel@ci.tuwien.ac.at>
## Created: 11 April 1994
## Adapted-By: jwe

function playaudio (name, ext)

  usage_msg = "playaudio (name, ext)  or  playaudio (X)";

  if (nargin == 1 && is_vector (name) && ! isstr (name))
    ## play a vector
    [nr, nc] = size (name);
    if (nc != 1)
      if (nr == 1)
        name = name';
        nr = nc;
      else
        error ("playaudio: X must be a vector");
      endif
    endif
    X = name + 127;
    unwind_protect
      file = tmpnam ();
      num = fopen (file, "wb");
      c = fwrite (num, X, "uchar");
      fclose (num);
      system (sprintf ("cat %s > /dev/dsp", file));
    unwind_protect_cleanup
      unlink (file);
    end_unwind_protect
  elseif (nargin >= 1 && isstr (name))
    ## play a file
    if (nargin == 1)
      name = [name, ".lin"];
    elseif (nargin == 2)
      name = [name, ".", ext];
    else
      usage (usage_msg);
    endif
    if (strcmp (ext, "lin") || strcmp (ext, "raw"))
      system (sprintf ("cat %s > /dev/dsp", name));
    elseif (strcmp (ext, "mu") || strcmp (ext, "au")
	    || strcmp (ext, "snd") || strcmp (ext, "ul"))
      system (sprintf ("cat %s > /dev/audio", name));
    else
      error ("playaudio does not support given extension");
    endif
  else
    usage (usage_msg);
  endif

endfunction
