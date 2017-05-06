#include "Part.h"
#define _USE_MATH_DEFINES
#include <math.h>

Part::Part(float x, float y, float hsize, float vsize, float angle, float spin) : Object(x,y,hsize,vsize,angle,spin){
    new_point(0, 0.5);
    new_point(-1, -0.5);
    new_point(1, -0.5);
    new_point(0, 0.5);
}

Part::~Part() {

}