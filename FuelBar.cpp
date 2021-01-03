#include "FuelBar.h"

FuelBar::FuelBar(float x, float y, float hsize, float vsize) 
    : Object(x, y, hsize, vsize, 0, 0) {
    newPoint(0,  0, 0, 255, 255);
    newPoint(1,  0, 0, 255, 255);
}