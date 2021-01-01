#include "Part.h"
#define _USE_MATH_DEFINES
#include <math.h>

Part::Part(float x, float y, float hsize, float vsize, float angle, float spin) : Object(x, y, hsize, vsize, angle, spin) {
    newPoint( 0, 0.5);
    newPoint(-1, -0.5);
    newPoint( 1, -0.5);
    newPoint( 0, 0.5);
}