x = ones (3);
x(1,1) = 0;
all (all (rand (3) + 1) == [1, 1, 1]) == 1 && all (all (x) == [0, 1, 1]) == 1
