#ifndef LANDER_H
#define LANDER_H

#include <vector>
#include "Object.h"

class Lander : public Object {
  public:
    //the constructor
    Lander(float x, float y, float hsize, float vsize, float angle, float spin);
    //the destructor
    ~Lander();

    void set_thrust(float thr);
    float thrust();

  private:
    float thrust_;

};

#endif