# Evaluate function
function fir_tb

disp(fir([0.5;-0.25;-0.1;0.1],
         [0;0.5;1;0.5;0;-0.5;-1;-0.5;0;0.5;1;0.5;0;-0.5;-1;-0.5],
         4,16));