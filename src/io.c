#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "computing.h"
#include "containers.h"
#include "io.h"

// Fonction pour gérer l'I/O

// Fonction pour lire les données des points de HCTR

int lecHctr(char *filename, size_t *nbPoints, Point **points)
{
    FILE *fichier = NULL;
    fichier = fopen(filename, "r");

    if (fichier == NULL)
    {
        printf("lecHctr : Erreur lors de l'ouverture du fichier \n");
        return 1;
    }

    int tmp;
    fscanf(fichier, "%ld %d", nbPoints, &tmp);

    *points = (Point *)malloc(sizeof(Point) * (*nbPoints));

    for (size_t i = 0; i < *nbPoints; i++)
    {
        fscanf(fichier, "%lf %lf", &(*points)[i].x, &(*points)[i].y);
    }

    fclose(fichier);
    return 0;
}

// Fonction pour lire les points de la triangulation créée de Delaunay (de Matlab)

int lecTriang(char *filename, Point *points, size_t *nbTriangles, Triangle **triangles)
{
    FILE *fichier = NULL;

    fichier = fopen(filename, "r");

    if (fichier == NULL)
    {
        printf("lecTriang : Erreur lors de l'ouverture du fichier \n");
        return 1;
    }

    int tmp;
    fscanf(fichier, "%ld\t%d\t%d", nbTriangles, &tmp, &tmp); // le nombre de triangles est dans la première ligne

    *triangles = (Triangle *)malloc(sizeof(Triangle) * (*nbTriangles));

    int edge;
    double x, y;
    Point omega;

    for (size_t i = 0; i < *nbTriangles; i++) // iterate on triangles
    {
        omega.x = 0.;
        omega.y = 0.;
        for (size_t j = 0; j < 3; j++) // iterate on edges
        {
            fscanf(fichier, "%d", &edge);

            (*triangles)[i].edges[j] = edge;

            x = points[edge - 1].x;
            y = points[edge - 1].y;

            // on cacule omega dans la foulée

            omega.x += x;
            omega.y += y;

            (*triangles)[i].points[j].x = x;
            (*triangles)[i].points[j].y = y;
        }
        omega.x /= 3.;
        omega.y /= 3.;

        (*triangles)[i].id = 0;     // id of triangles start at 1
        (*triangles)[i].id = i + 1; // id of triangles start at 1
        (*triangles)[i].omega = omega;

        computeMicroTriangles(&(*triangles)[i]);
    }

    fclose(fichier);
    return 0;
}

int makeRESFile(char *filename, Mesh *mesh, iResult *res)
{
    FILE *fichier = NULL;

    fichier = fopen(filename, "w");

    if (fichier == NULL)
    {
        printf("makeRESFile : Erreur lors de l'ouverture du fichier \n");
        return 1;
    }

    for (size_t i = 0; i < mesh->nbTri; i++)
    {
        fprintf(fichier, "%ld\t", i + 1);
        for (size_t j = 0; j < 3; j++)
        {
            fprintf(fichier, "%d\t", mesh->triangles[i].edges[j]);
        }
        fprintf(fichier, "\n");
    }

    for (size_t i = 0; i < mesh->nbPoints; i++)
    {
        fprintf(fichier, "%ld\t%lf\t%lf\t%f\t%f\t%f\n", i + 1, mesh->points[i].x, mesh->points[i].y, res->fx[i], res->dfx[i], res->dfy[i]);
    }

    double minE = res->err[0], maxE = res->err[0];
    for (size_t i = 0; i < mesh->dim; i++)
    {
        minE = min(minE, res->err[i]);
        maxE = max(maxE, res->err[i]);
    }
    fprintf(fichier, "%lf\t%lf", minE, maxE);

    fclose(fichier);

    return 0;
}

int makePlotFile(char *filename, Mesh *mesh, iResult *res)
{
    FILE *fichier = NULL;

    fichier = fopen(filename, "w");

    if (fichier == NULL)
    {
        printf("makeRESFile : Erreur lors de l'ouverture du fichier \n");
        return 1;
    }

    fprintf(fichier, "%ld\t%ld\t0\n", mesh->resX, mesh->resY);

    for (size_t i = 0; i < mesh->dim; i++)
    {
        fprintf(fichier, "%lf\t%lf\t%lf\n", mesh->ipoints[i].point.x, mesh->ipoints[i].point.y, res->out[i]);
    }

    fclose(fichier);
    return 0;
}

void freeMesh(Mesh *mesh)
{
    free(mesh->ipoints);
}

void freeiRes(iResult *res)
{
    free(res->out);
    free(res->err);
    free(res->fx);
    free(res->dfx);
    free(res->dfy);
}
