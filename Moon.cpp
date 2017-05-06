#include "Moon.h"

Moon::Moon(float x, float y, float hsize, float vsize, float angle) : Object(x,y,hsize,vsize,angle){
  new_point(-0.25,-1.20);
  new_point(-0.19,-1.10);
  new_point(-0.18,-1.09);
  new_point(-0.16,-0.95);
  new_point(-0.14,-0.95);
  new_point(-0.11,-1.10);
  new_point(-0.09,-0.90);
  new_point(-0.02,-0.95);
  new_point( 0.10,-0.95);
  new_point( 0.15,-1.09);
  new_point( 0.16,-1.13);
  new_point( 0.17,-1.10);
  new_point( 0.19,-1.10);
  new_point( 0.21,-1.09);
  new_point( 0.25,-1.30);
}

Moon::~Moon() {

}