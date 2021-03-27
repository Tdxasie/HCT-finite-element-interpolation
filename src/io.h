#ifndef IO_H
#define IO_H

#include <stdlib.h>

#include "containers.h"

int lecHctr(char *filename, size_t *nbPoints, Point **points);
int lecTriang(char *filename, Point *points, size_t *nbTriangles, Triangle **triangles);
int makeRESFile(char *filename, Mesh *mesh, iResult *res);
int makePlotFile(char *filename, Mesh *mesh, iResult *res);
void freeMesh(Mesh *mesh);
void freeiRes(iResult *res);

#endif