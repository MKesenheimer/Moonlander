#include "Collision.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Collision::Collision() {
  collided = false;
}

Collision::~Collision() {

}

bool Collision::check_collision(Object *o1, Object *o2) {
  collided = false;
  //first check: if the sum of the dimensions of the objects is less
  //then their distance from center to center -> collision
  float dim1 = dim(o1);
  float dim2 = dim(o2);
  if(dim1+dim2>=dist(o1,o2)) collided = true;
  return collided;
}

float Collision::dist(Object *o1, Object *o2) {
  std::vector<float> cen1 = center(o1);
  std::vector<float> cen2 = center(o2);
  //debug
  //std::cout<<"(x1, y1)=("<<cen1[0]<<", "<<cen1[0]<<"), (x2, y2)=("<<cen2[0]<<", "<<cen2[1]<<")"<<std::endl;
  //calculate the distance of the two objects
  float dist = sqrt(pow(cen1[0] - cen2[0],2)+pow(cen1[1] - cen2[1],2));
  return dist;
}

std::vector<float> Collision::center(Object *o) {
  std::vector<float> temp;
  
  temp.push_back(o->x());
  temp.push_back(o->y());
  
  return temp;
}

float Collision::dim(Object *o) {
  std::vector<float> r, r0;
  r0 = center(o);
  float r_abs = 0;
  float x, y;
  for(int i=1; i<o->npoints(); i++) {
    r = o->get_point(i);
    x = r[0] - r0[0];
    y = r[1] - r0[1];
    if(r_abs <= sqrt(pow(x,2)+pow(y,2))) {
      r_abs = sqrt(pow(x,2)+pow(y,2));
    }
  }
  return r_abs;
}