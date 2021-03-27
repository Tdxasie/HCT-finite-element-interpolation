clear
close all

data = load('hctr.pts');

x = data(2:end, 1);
y = data(2:end, 2);

tri = delaunay(x, y);

figure(1)
triplot(tri, x, y);

tri = [ size(tri) 0; tri];

writematrix(tri, 'tri.txt', 'Delimiter', 'tab');

!gcc -o computer.exe main.c io.c computing.c buildmesh.c interpolate.c -lm
!computer.exe

figure(2);
data = load('plot_f.txt');
m = data(1,1);
n = data(1,2);
X = reshape(data(2:end, 1), m, n);
Y = reshape(data(2:end, 2), m, n);
Z = reshape(data(2:end, 3), m, n);

surf(X, Y, Z);

figure(3);
data = load('plot_g.txt');
m = data(1,1);
n = data(1,2);
X = reshape(data(2:end, 1), m, n);
Y = reshape(data(2:end, 2), m, n);
Z = reshape(data(2:end, 3), m, n);

surf(X, Y, Z);
