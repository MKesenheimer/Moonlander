#include "Moon.h"

Moon::Moon(float x, float y, float hsize, float vsize, float angle) : Object(x, y, hsize, vsize, angle) {
    newPoint(-0.25, -1.20, 0, 255, 255);
    newPoint(-0.19, -1.10, 0, 255, 255);
    newPoint(-0.18, -1.09, 0, 255, 255);
    newPoint(-0.16, -0.95, 0, 255, 255);
    newPoint(-0.14, -0.95, 0, 255, 255);
    newPoint(-0.11, -1.10, 0, 255, 255);
    newPoint(-0.09, -0.90, 0, 255, 255);
    newPoint(-0.02, -0.95, 0, 255, 255);
    newPoint( 0.10, -0.95, 0, 255, 255);
    newPoint( 0.15, -1.09, 0, 255, 255);
    newPoint( 0.16, -1.13, 0, 255, 255);
    newPoint( 0.17, -1.10, 0, 255, 255);
    newPoint( 0.19, -1.10, 0, 255, 255);
    newPoint( 0.21, -1.09, 0, 255, 255);
    newPoint( 0.25, -1.30, 0, 255, 255);
}