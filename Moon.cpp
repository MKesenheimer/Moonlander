#include "Moon.h"

Moon::Moon(float x, float y, float angle, float hsize, float vsize) : Object(x,y,angle){
  hsize_ = hsize;
  vsize_ = vsize;
  
  new_point( 0, 1*hsize_);
}

float Moon::hsize() {
  return hsize_;
}

float Moon::vsize() {
  return vsize_;
}

Moon::~Moon() {

}