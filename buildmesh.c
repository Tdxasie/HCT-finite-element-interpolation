#include <stdio.h>
#include <stdlib.h>

#include "buildmesh.h"
#include "computing.h"
#include "containers.h"

// MESH BUILDER

void buildMesh(Point *points, size_t nbPoints, Triangle *triangles, size_t nbTri, size_t resX, size_t resY, Mesh *mesh)
{
    computeMaxDim(points, nbPoints, mesh);
    computeMinDim(points, nbPoints, mesh);

    double dx = (mesh->xmax - mesh->xmin) / (double)(resX - 1);
    double dy = (mesh->ymax - mesh->ymin) / (double)(resY - 1);

    size_t dim = resX * resY;

    Ipoint *ipoints = (Ipoint *)malloc(sizeof(Ipoint) * dim);

    int idx = 0;
    for (size_t i = 0; i < resX; i++)
    {
        for (size_t j = 0; j < resY; j++)
        {
            ipoints[idx].point.x = mesh->xmin + i * dx;
            ipoints[idx].point.y = mesh->ymin + j * dy;

            processIpoint(&(ipoints[idx]), triangles, nbTri);
            idx++;
        }
    }
    mesh->dim = dim;
    mesh->resX = resX;
    mesh->resY = resY;
    mesh->nbTri = nbTri;
    mesh->points = points;
    mesh->ipoints = ipoints;
    mesh->nbPoints = nbPoints;
    mesh->triangles = triangles;
}

void processIpoint(Ipoint *ipoint, Triangle *triangles, size_t nbTri)
{
    findTri(ipoint, triangles, nbTri);

    findMicroTri(ipoint, triangles, nbTri);
}

void findTri(Ipoint *ipoint, Triangle *triangles, size_t nbTri)
{
    int found = 0;
    for (size_t i = 0; i < nbTri; i++)
    {
        computeBarycentric(ipoint->point, triangles[i].points, ipoint);

        if (ipoint->lambda[0] > -EPSILON && ipoint->lambda[1] > -EPSILON && ipoint->lambda[2] > -EPSILON)
        {
            int idx = triangles[i].id - 1;
            ipoint->triangle = triangles[idx];

            found++;

            break;
        }
    }
    assert(found == 1);
}

void findMicroTri(Ipoint *ipoint)
{
    int found = 0;
    for (size_t i = 0; i < 3; i++)
    {
        computeBarycentric(ipoint->point, ipoint->triangle.microTriangles[i].points, ipoint);

        if (ipoint->lambda[0] > -EPSILON && ipoint->lambda[1] > -EPSILON && ipoint->lambda[2] > -EPSILON)
        {
            int idx = ipoint->triangle.microTriangles[i].id - 1;
            ipoint->microTri = ipoint->triangle.microTriangles[idx];

            found++;

            break;
        }
    }
    assert(found == 1);
}