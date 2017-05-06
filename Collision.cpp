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
  //then their distance from center to center -> enable advanced collision detection
  float dim1 = dim(o1);
  float dim2 = dim(o2);
  if(dim1+dim2>=dist(o1,o2)) {
    //advanced collision control
    if(dim1 <= dim2) {
        //start of line between two points
        std::vector<float> start = o2->get_point(0);
        for (int i=1; i<o2->npoints(); i++) {
            //end of line
            std::vector<float> end = o2->get_point(i);
            //number of points on line
            int nsteps = (int)(10*dist(start, end)/dim1);
            for (int j=0; j<nsteps; j++) {
                //build new points that lay on the line
                float x = start[0] + j*(end[0]-start[0])/nsteps;
                float y = start[1] + j*(end[1]-start[1])/nsteps;
                std::vector<float> check = {x,y};
                if(dist(o1,check) <= dim1) collided = true;
            }
            start = end;
        }
    } else {
        //start of line between two points
        std::vector<float> start = o1->get_point(0);
        for (int i=1; i<o1->npoints(); i++) {
            //end of line
            std::vector<float> end = o1->get_point(i);
            //number of points on line
            int nsteps = (int)(10*dist(start, end)/dim2);
            for (int j=0; j<nsteps; j++) {
                //build new points that lay on the line
                float x = start[0] + j*(end[0]-start[0])/nsteps;
                float y = start[1] + j*(end[1]-start[1])/nsteps;
                std::vector<float> check = {x,y};
                if(dist(o2,check) <= dim2) collided = true;
            }
            start = end;
        }
    }
  }
  return collided;
}

float Collision::dist(Object *o1, Object *o2) {
  std::vector<float> cen1 = center(o1);
  std::vector<float> cen2 = center(o2);
  //calculate the distance of the two objects
  float dist = sqrt(pow(cen1[0] - cen2[0],2)+pow(cen1[1] - cen2[1],2));
  return dist;
}

float Collision::dist(Object *o, std::vector<float> point) {
  std::vector<float> cen = center(o);
  float dist = sqrt(pow(cen[0] - point[0],2)+pow(cen[1] - point[1],2));
  return dist;
}

float Collision::dist(std::vector<float> point1, std::vector<float> point2) {
  float dist = sqrt(pow(point1[0] - point2[0],2)+pow(point1[1] - point2[1],2));
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
    if(o->is_collidable(i)) {
      r = o->get_point(i);
      x = r[0] - r0[0];
      y = r[1] - r0[1];
      if(r_abs <= sqrt(pow(x,2)+pow(y,2))) {
        r_abs = sqrt(pow(x,2)+pow(y,2));
      }
    }
  }
  return r_abs;
}