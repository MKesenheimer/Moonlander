#include "Lander.h"
#define _USE_MATH_DEFINES
#include <math.h>

Lander::Lander(float x, float y, float hsize, float vsize, float angle, float spin) : Object(x,y,hsize,vsize,angle,spin){
  thrust_ = 0;
  
  //save all relevant points to draw the lander (the points are defined
  //in the object-own coordinate system)
  //thruster
  new_point( 1, 0);
  new_point(-1, 0);
  new_point(-3, 5);
  new_point( 3, 5);
  new_point( 1, 0);
  
  //fire
  new_point( 3, 5);
  new_point( 2, 5);
  new_point( 0, 5, false); //8th point is not collidable (tip of flare)
  new_point(-2, 5);
  new_point( 3, 5);
  new_point( 1, 0);
  
  //stilts
  new_point(-4, 0);
  new_point(-7, 7);
  new_point(-8, 7);
  new_point(-6, 7);
  new_point(-7, 7);
  new_point(-4, 0);
  new_point( 4, 0);
  new_point( 7, 7);
  new_point( 8, 7);
  new_point( 6, 7);
  new_point( 7, 7);
  new_point( 4, 0);
  
  //bottom
  new_point(-6, 0);
  new_point( 6, 0);
  new_point( 6,-2);
  new_point(-6,-2);
  new_point(-6, 0);
  new_point(-6,-2);
  
  //capsula
  new_point(-4, -2);
  new_point( 4, -2);
  new_point( 5, -3);
  new_point( 5, -9);
  new_point( 4,-10);
  new_point(-4,-10);
  new_point(-5, -9);
  new_point(-5, -3);
  new_point(-4, -2);
}

float Lander::thrust() {
  return thrust_;
}

void Lander::set_thrust(float thr) {
  thrust_ = thr;
  this->modify_point(-sin(phi())*(5+thr*2),cos(phi())*(5+thr*2),8);
}

Lander::~Lander() {

}