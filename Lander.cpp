#include "Lander.h"
#define _USE_MATH_DEFINES
#include <math.h>

Lander::Lander(float x, float y, float angle, float hsize, float vsize) : Object(x,y,angle){
  hsize_ = hsize;
  vsize_ = vsize;
  thrust_ = 0;

  //save all relevant points to draw the lander (the points are defined
  //in the object-own coordinate system)
  //thruster
  new_point( 1*vsize_, 5*hsize_);
  new_point(-1*vsize_, 5*hsize_);
  new_point(-3*vsize_,10*hsize_);
  new_point( 3*vsize_,10*hsize_);
  new_point( 1*vsize_, 5*hsize_);
  
  //fire
  new_point( 3*vsize_,10*hsize_);
  new_point( 2*vsize_,10*hsize_);
  new_point( 0,       10*hsize_); //8th point
  new_point(-2*vsize_,10*hsize_);
  new_point( 3*vsize_,10*hsize_);
  new_point( 1*vsize_, 5*hsize_);
  
  //stilts
  new_point(-4*vsize_, 5*hsize_);
  new_point(-7*vsize_, 12*hsize_);
  new_point(-8*vsize_, 12*hsize_);
  new_point(-6*vsize_, 12*hsize_);
  new_point(-7*vsize_, 12*hsize_);
  new_point(-4*vsize_, 5*hsize_);
  new_point( 4*vsize_, 5*hsize_);
  new_point( 7*vsize_, 12*hsize_);
  new_point( 8*vsize_, 12*hsize_);
  new_point( 6*vsize_, 12*hsize_);
  new_point( 7*vsize_, 12*hsize_);
  new_point( 4*vsize_, 5*hsize_);
  
  //bottom
  new_point(-6*vsize_, 5*hsize_);
  new_point( 6*vsize_, 5*hsize_);
  new_point( 6*vsize_, 3*hsize_);
  new_point(-6*vsize_, 3*hsize_);
  new_point(-6*vsize_, 5*hsize_);
  new_point(-6*vsize_, 3*hsize_);
  
  //capsula
  new_point(-4*vsize_, 3*hsize_);
  new_point( 4*vsize_, 3*hsize_);
  new_point( 5*vsize_, 2*hsize_);
  new_point( 5*vsize_,-4*hsize_);
  new_point( 4*vsize_,-5*hsize_);
  new_point(-4*vsize_,-5*hsize_);
  new_point(-5*vsize_,-4*hsize_);
  new_point(-5*vsize_, 2*hsize_);
  new_point(-4*vsize_, 3*hsize_);
}

float Lander::hsize() {
  return hsize_;
}

float Lander::vsize() {
  return vsize_;
}

float Lander::xcenter() {
  return this->x();
}

float Lander::ycenter() {
  return this->y();
}

float Lander::thrust() {
  return thrust_;
}

void Lander::set_thrust(float thr) {
  thrust_ = thr;
  this->modify_point(-sin(phi())*(10+thr)*vsize_,cos(phi())*(10+thr)*hsize_,8);
}

Lander::~Lander() {

}