#ifndef MOON_H
#define MOON_H

#include <vector>
#include "Object.h"

class Moon : public Object {
  public:
    //the constructor
    Moon(float x, float y, float angle, float hsize, float vsize);
    //the destructor
    ~Moon();
    
    //gives the horizontal size
    float hsize();
    //gives the verticla size
    float vsize();

  private:
    float hsize_;
    float vsize_;

};

#endif