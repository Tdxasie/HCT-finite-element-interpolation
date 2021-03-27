#ifndef CONTAINERS_H
#define CONTAINERS_H

typedef float (*testFunction)(float, float);

typedef struct
{
    double x;
    double y;
} Point;

typedef struct
{
    int id;
    Point points[3];
} MicroTriangle;

typedef struct
{
    int id;
    int edges[3];
    Point omega;
    Point points[3];
    MicroTriangle microTriangles[3];
} Triangle;

typedef struct
{
    double lambda[3];
    Point point;
    Triangle triangle;
    MicroTriangle microTri;
} Ipoint;

typedef struct
{
    size_t nbPoints;
    size_t nbTri;
    size_t dim;
    size_t resX;
    size_t resY;

    double xmin;
    double xmax;
    double ymin;
    double ymax;

    Point *points;
    Ipoint *ipoints;
    Triangle *triangles;
} Mesh;

typedef struct
{
    float *fx;
    float *dfx;
    float *dfy;

    double *out;
    double *err;
} iResult;

typedef struct
{
    float a[3];
    float b[3];
    float c[3];
    float d[3];
    float e[3];
    float p[3];
    float q[3];
    float g[3];
    float u[3];
} Coefs;

#endif