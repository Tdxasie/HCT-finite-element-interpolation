#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "containers.h"

void interpolate(testFunction f, testFunction dfx, testFunction dfy, Mesh *mesh, iResult *res);
void printCoefs(Coefs *coefs);

#endif