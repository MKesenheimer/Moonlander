#include "part.h"

Part::Part(float x, float y, float vx, float vy, float hsize, float vsize, float angle, float spin)
    : object(x, y, vx, vy, hsize, vsize, angle, spin) {
    newPoint( 0,  0.5, 255, 255, 0);
    newPoint(-1, -0.5, 255, 255, 0);
    newPoint( 1, -0.5, 255, 255, 0);
    newPoint( 0,  0.5, 255, 255, 0);
}