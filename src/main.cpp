//============================================================================
// Name        : topsis.cpp
// Author      : Athanasios Fotos
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
