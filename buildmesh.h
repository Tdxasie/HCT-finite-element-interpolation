#ifndef BUILDMESH_H
#define BUILDMESH_H

#define EPSILON 1e-6

#include "assert.h"
#include <stdlib.h>

#include "containers.h"

void buildMesh(Point *points, size_t nbPoints, Triangle *triangles, size_t nbTri, size_t resX, size_t resY, Mesh *mesh1);
void processIpoint(Ipoint *ipoint, Triangle *triangles, size_t nbTri);
void findTri(Ipoint *ipoint);
void findMicroTri(Ipoint *ipoint, Triangle *triangles, size_t nbTri);

#endif