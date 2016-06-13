#ifndef LANDER_H
#define LANDER_H

#include <vector>
#include "Object.h"

class Lander : public Object {
  public:
    //the constructor
    Lander(float x, float y, float angle, float hsize, float vsize);
    //the destructor
    ~Lander();
    
    //gives the horizontal size
    float hsize();
    //gives the verticla size
    float vsize();
    
    float xcenter();
    float ycenter();

    void set_thrust(float thr);
    float thrust();

  private:
    float hsize_;
    float vsize_;
    float thrust_;

};

#endif