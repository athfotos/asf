//============================================================================
// Name        : topsis.cpp
// Author      : Athanasios Fotos
// Version     :
// Copyright   : Copy Everything
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "Asf.h"

using namespace std;

int main(void) {

    Asf *calc = new Asf();
    calc->doCrazyMath();
    delete calc;

    return 0;
}
