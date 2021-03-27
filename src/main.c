#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "buildmesh.h"
#include "containers.h"
#include "interpolate.h"
#include "io.h"

// DEFINITION OF THE TEST FUNCTIONS

float f(float x1, float x2)
{
    return exp(x1 + x2);
}

float dfx(float x1, float x2)
{
    return exp(x1 + x2);
}

float dfy(float x1, float x2)
{
    return exp(x1 + x2);
}

float g(float x1, float x2)
{
    return x2 * x2 * x2 - 2 * x1 * x2 * x2 - 5 * x1 * x1 * x2 + 10 * x1 * x2 + 1.;
}

float dgx(float x1, float x2)
{
    return (-2) * x2 * x2 - 10 * x1 * x2 + 10 * x2;
}

float dgy(float x1, float x2)
{
    return 3 * x2 * x2 - 4 * x1 * x2 - 5 * x1 * x1 + 10 * x1;
}

// PROGRAM STARTS HERE

int main(int argc, char const *argv[])
{
    char *pointsFilename = "hctr.pts";
    char *triangleFilename = "tri.txt";

    size_t resX = 100, resY = 100; // resolution of interpolation, could be read from argv

    size_t nbPoints, nbTri;

    Point *points;
    Triangle *triangles;

    lecHctr(pointsFilename, &nbPoints, &points);

    lecTriang(triangleFilename, points, &nbTri, &triangles);

    Mesh mesh;

    buildMesh(points, nbPoints, triangles, nbTri, resX, resY, &mesh);

    iResult resf;
    iResult resg;

    interpolate(&f, &dfx, &dfy, &mesh, &resf);
    interpolate(&g, &dgx, &dgy, &mesh, &resg);

    makeRESFile("hctr.res", &mesh, &resf);

    makePlotFile("plot_f.txt", &mesh, &resf);
    makePlotFile("plot_g.txt", &mesh, &resg);

    free(points);
    free(triangles);
    freeMesh(&mesh);

    freeiRes(&resf);
    freeiRes(&resg);

    return 0;
}
