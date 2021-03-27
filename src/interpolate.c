#include <stdio.h>
#include <stdlib.h>

#include "computing.h"
#include "containers.h"
#include "interpolate.h"

// INTERPOLATION MODULE

void interpolate(testFunction f, testFunction dfx, testFunction dfy, Mesh *mesh, iResult *res)
{
    computeFunctions(f, dfx, dfy, res, mesh);

    Coefs coefs;

    double S;

    res->out = (double *)malloc(sizeof(double) * mesh->dim);
    res->err = (double *)malloc(sizeof(double) * mesh->dim);

    for (size_t i = 0; i < mesh->dim; i++)
    {
        computeCoefs(&(mesh->ipoints[i].triangle), res->fx, res->dfx, res->dfy, &coefs);

        S = computeInterpolation(mesh->ipoints[i].microTri.id, mesh->ipoints[i].lambda, &coefs);

        res->out[i] = S;

        res->err[i] = f(mesh->ipoints[i].point.x, mesh->ipoints[i].point.x) - S;
    }
}

void printCoefs(Coefs *coefs)
{
    for (size_t i = 0; i < 3; i++)
    {
        printf("%f    %f    %f    %f    %f    %f    %f    %f    %f\n", coefs->a[i], coefs->b[i], coefs->c[i], coefs->d[i], coefs->e[i],
               coefs->g[i], coefs->p[i], coefs->q[i], coefs->u[i]);
    }
}