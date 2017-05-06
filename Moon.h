#ifndef MOON_H
#define MOON_H

#include <vector>
#include "Object.h"

class Moon : public Object {
  public:
    //the constructor
    Moon(float x, float y, float hsize, float vsize, float angle);
    //the destructor
    ~Moon();
};

#endif