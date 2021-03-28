#include "assert.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "computing.h"
#include "containers.h"

// COMPUTING MODULE

// Calcul des 19 coefficients

void computeCoefs(Triangle *triangle, float *f, float *dxf, float *dyf, Coefs *coefs)
{
    size_t j, k;
    int edge_i, edge_j, edge_k; // edge_j;

    for (size_t i = 0; i < 3; i++) // compute a
    {
        edge_i = triangle->edges[i] - 1;
        coefs->a[i] = f[edge_i];
    }

    for (size_t j = 0; j < 3; j++) // compute p
    {
        k = (j + 1) % 3;
        edge_j = triangle->edges[j] - 1;
        coefs->p[j] =
            dxf[edge_j] * (triangle->points[k].x - triangle->points[j].x) + dyf[edge_j] * (triangle->points[k].y - triangle->points[j].y);
    }

    for (size_t i = 0; i < 3; i++) // compute q
    {
        j = (i + 1) % 3;
        k = (i + 2) % 3;
        edge_k = triangle->edges[k] - 1;
        coefs->q[k] =
            dxf[edge_k] * (triangle->points[j].x - triangle->points[k].x) + dyf[edge_k] * (triangle->points[j].y - triangle->points[k].y);
    }

    for (size_t i = 0; i < 3; i++) // compute b
    {
        coefs->b[i] = coefs->a[i] + coefs->p[i] / 3.;
    }

    for (size_t i = 0; i < 3; i++) // compute c
    {
        coefs->c[i] = coefs->a[i] + coefs->q[i] / 3.;
    }

    for (size_t i = 0; i < 3; i++) // compute d
    {
        coefs->d[i] = (coefs->a[i] + coefs->b[i] + coefs->c[i]) / 3.;
    }

    for (size_t i = 0; i < 3; i++) // compute u
    {
        j = (i + 1) % 3;
        k = (i + 2) % 3;
        coefs->u[i] = 2. *
                      ((triangle->points[k].x - triangle->omega.x) * (triangle->points[k].x - triangle->points[j].x) +
                       (triangle->points[k].y - triangle->omega.y) * (triangle->points[k].y - triangle->points[j].y)) /
                      ((triangle->points[k].x - triangle->points[j].x) * (triangle->points[k].x - triangle->points[j].x) +
                       (triangle->points[k].y - triangle->points[j].y) * (triangle->points[k].y - triangle->points[j].y));
    }

    for (size_t i = 0; i < 3; i++) // compute g
    {
        j = (i + 1) % 3;
        k = (i + 2) % 3;
        coefs->g[i] = (2. * (coefs->d[k] + coefs->d[j]) + (4. - 3. * coefs->u[i]) * coefs->c[k] + (coefs->u[i] - 2.) * coefs->a[k] +
                       (3. * coefs->u[i] - 2.) * coefs->b[j] - coefs->u[i] * coefs->a[j]) /
                      4.;
    }

    for (size_t i = 0; i < 3; i++) // compute e
    {
        j = (i + 1) % 3;
        k = (i + 2) % 3;
        coefs->e[k] = (coefs->d[k] + coefs->g[i] + coefs->g[j]) / 3.;
    }
}

void computeBarycentric(Point p, Point *pointsTriangle, Ipoint *ipoint)
{
    size_t j, k;
    for (size_t i = 0; i < 3; i++)
    {
        j = (i + 1) % 3;
        k = (j + 1) % 3;

        ipoint->lambda[i] = ((pointsTriangle[k].x - pointsTriangle[j].x) * (p.y - pointsTriangle[j].y) -
                             (pointsTriangle[k].y - pointsTriangle[j].y) * (p.x - pointsTriangle[j].x)) /
                            ((pointsTriangle[k].x - pointsTriangle[j].x) * (pointsTriangle[i].y - pointsTriangle[j].y) -
                             (pointsTriangle[k].y - pointsTriangle[j].y) * (pointsTriangle[i].x - pointsTriangle[j].x));
    }
}

void computeMicroTriangles(Triangle *triangle)
{
    int j, k;
    for (int i = 0; i < 3; i++)
    {
        triangle->microTriangles[i].id = i + 1;

        // i : 0 1 2
        // j : 1 2 0
        // k : 2 0 1
        j = (i + 1) % 3;
        k = (i + 2) % 3;

        triangle->microTriangles[i].points[0] = triangle->omega;
        triangle->microTriangles[i].points[1] = triangle->points[j];
        triangle->microTriangles[i].points[2] = triangle->points[k];
    }
}

void computeMaxDim(Point *points, size_t nbPoints, Mesh *mesh)
{
    assert(nbPoints > 0);
    double maxX = points[0].x;
    double maxY = points[0].y;
    for (size_t i = 1; i < nbPoints; i++)
    {
        maxX = max(maxX, points[i].x);
        maxY = max(maxY, points[i].y);
    }
    mesh->xmax = maxX;
    mesh->ymax = maxY;
}

void computeMinDim(Point *points, size_t nbPoints, Mesh *mesh)
{
    assert(nbPoints > 0);
    double minX = points[0].x;
    double minY = points[0].y;
    for (size_t i = 1; i < nbPoints; i++)
    {
        minX = min(minX, points[i].x);
        minY = min(minY, points[i].y);
    }
    mesh->xmin = minX;
    mesh->ymin = minY;
}

void computeFunctions(testFunction f, testFunction dfx, testFunction dfy, iResult *res, Mesh *mesh)
{
    res->fx = (float *)malloc(sizeof(float) * mesh->nbPoints);
    res->dfx = (float *)malloc(sizeof(float) * mesh->nbPoints);
    res->dfy = (float *)malloc(sizeof(float) * mesh->nbPoints);

    for (size_t i = 0; i < mesh->nbPoints; i++)
    {
        res->fx[i] = f(mesh->points[i].x, mesh->points[i].y);
        res->dfx[i] = dfx(mesh->points[i].x, mesh->points[i].y);
        res->dfy[i] = dfy(mesh->points[i].x, mesh->points[i].y);
    }
}

double computeInterpolation(int id, double lambda[3], Coefs *coefs)
{
    double w = (coefs->e[0] + coefs->e[1] + coefs->e[2]) / 3.;

    int i = id - 1;
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;

    double S = coefs->a[k] * lambda[2] * lambda[2] * lambda[2] + coefs->a[j] * lambda[1] * lambda[1] * lambda[1] +
               coefs->b[j] * 3 * lambda[1] * lambda[1] * lambda[2] + coefs->c[k] * 3 * lambda[1] * lambda[2] * lambda[2] +
               coefs->d[j] * 3 * lambda[0] * lambda[1] * lambda[1] + coefs->d[k] * 3 * lambda[0] * lambda[2] * lambda[2] +
               coefs->e[j] * 3 * lambda[0] * lambda[0] * lambda[1] + coefs->e[k] * 3 * lambda[0] * lambda[0] * lambda[2] +
               coefs->g[i] * 6 * lambda[0] * lambda[1] * lambda[2] + w * lambda[0] * lambda[0] * lambda[0];
    return S;
}