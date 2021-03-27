#ifndef COMPUTING_H
#define COMPUTING_H

#define min(x, y)                                                                                                                          \
    ({                                                                                                                                     \
        typeof(x) _x = (x);                                                                                                                \
        typeof(y) _y = (y);                                                                                                                \
        (void)(&_x == &_y);                                                                                                                \
        _x < _y ? _x : _y;                                                                                                                 \
    })

#define max(x, y)                                                                                                                          \
    ({                                                                                                                                     \
        typeof(x) _x = (x);                                                                                                                \
        typeof(y) _y = (y);                                                                                                                \
        (void)(&_x == &_y);                                                                                                                \
        _x > _y ? _x : _y;                                                                                                                 \
    })

#include "containers.h"

void computeMicroTriangles(Triangle *triangle);
void computeCoefs(Triangle *triangle, float *f, float *dxf, float *dyf, Coefs *coefs);
void computeBarycentric(Point p, Point *pointsTriangle, Ipoint *ipoint);
void computeMaxDim(Point *points, size_t nbPoints, Mesh *mesh);
void computeMinDim(Point *points, size_t nbPoints, Mesh *mesh);
void computeFunctions(testFunction f, testFunction dfx, testFunction dfy, iResult *res, Mesh *mesh);
double computeInterpolation(int id, double lambda[3], Coefs *coefs);

#endif