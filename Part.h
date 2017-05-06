#ifndef PART_H
#define PART_H

#include <vector>
#include "Object.h"

class Part : public Object {
  public:
    //the constructor
    Part(float x, float y, float hsize, float vsize, float angle, float spin);
    //the destructor
    ~Part();
};

#endif