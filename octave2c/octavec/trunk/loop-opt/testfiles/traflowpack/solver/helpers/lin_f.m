function u = lin_f(x, a, b)
% LIN_F sample a linear function
%   u = LIN_F(x, a, b) creates a solution vector u of same length as x
%   with the values from a linear function between a and b (for x(1), resp. x(n))

% Copyright (C) 2002 Arthur van Dam, Delft, The Netherlands
% 
% This file is part of TraFlowPACK.
% 
% TraFlowPACK is free software; you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation; either version 2 of the License, or
% (at your option) any later version.
% 
% TraFlowPACK is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with TraFlowPACK; if not, write to the Free Software
% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
% 
% More info at: http://www.inro.tno.nl/five/traflow/
n = length(x);
s = x(1);
l = x(n)-s;
u = (b-a)/l * (x-s) + a;

