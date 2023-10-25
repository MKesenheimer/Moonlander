#include "moon.h"

Moon::Moon(float x, float y, float hsize, float vsize, float angle) : object(x, y, 0, 0, hsize, vsize, angle) {
    newPoint(-0.50, -0.80, 0, 255, 255);
    newPoint(-0.39, -0.60, 0, 255, 255);
    newPoint(-0.36, -0.62, 0, 255, 255);
    newPoint(-0.32, -0.10, 0, 255, 255);
    newPoint(-0.28, -0.10, 0, 255, 255);
    newPoint(-0.22, -0.62, 0, 255, 255);
    newPoint(-0.18, -0.05, 0, 255, 255);
    newPoint(-0.04, -0.10, 0, 255, 255);
    newPoint( 0.20, -0.10, 0, 255, 255);
    newPoint( 0.30, -0.62, 0, 255, 255);
    newPoint( 0.32, -0.55, 0, 255, 255);
    newPoint( 0.34, -0.60, 0, 255, 255);
    newPoint( 0.38, -0.60, 0, 255, 255);
    newPoint( 0.42, -0.57, 0, 255, 255);
    newPoint( 0.50, -0.90, 0, 255, 255);
}