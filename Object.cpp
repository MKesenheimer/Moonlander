#include "Object.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

Object::Object(float x, float y, float angle) {
  x_ = x;
  y_ = y;
  old_phi_ = 0.0;
  phi_ = angle;
  npoints_ = 0;
  new_point(x_,y_);
}

float Object::x() {
  return x_;
}

float Object::y() {
  return y_;
}

float Object::vx() {
  return vx_;
}

float Object::vy() {
  return vy_;
}

float Object::phi() {
  return phi_;
}

float Object::npoints() {
  return npoints_;
}

void Object::set_pos(float x, float y) {
  x_ = x;
  y_ = y;
}

void Object::set_v(float vx, float vy) {
  vx_ = vx;
  vy_ = vy;
}

void Object::set_angle(float angle) {
  phi_ = angle;
  //this algorithm rotates the object every time it is called
  //so we have to rotate only by the difference of  the old and the
  //new angle
  float dphi = (phi_ - old_phi_); //angle in rad /360.0*(2*M_PI)
  for (int i=0; i<3*npoints_; i+=3) {
    //the position in the unrotated coordinate system
    float xfs = points[i+1] - x_;
    float yfs = points[i+2] - y_;
    
    //rotate back into the world coordinate system
    points[i+1] = cos(dphi)*xfs - sin(dphi)*yfs + x_;
    points[i+2] = sin(dphi)*xfs + cos(dphi)*yfs + y_;
  }
  old_phi_ = phi_;
}

void Object::new_point(float x, float y) {
  if (npoints_ == 0) {
    points.push_back(npoints_);
    points.push_back(x);
    points.push_back(y);
    npoints_++;
  }
  else {
    points.push_back(npoints_);
    //move all object points into the world coordinate system
    points.push_back(x_ + x);
    points.push_back(y_ + y);
    npoints_++;
  }
}

std::vector<float> Object::get_point(int n) {
  std::vector<float> retv;
  for (int i=0; i<=3*n; i+=3) {
    if ((int)points[i] == n) {
      retv.push_back(points[i+1]);
      retv.push_back(points[i+2]);
      return retv;
    }
  }
  std::cout<<"an error occured in Object.cpp: n="<<n<<" is not a valid index"<<std::endl;
  return points;
}

void Object::modify_point(float x, float y, int n) {
  for (int i=0; i<=3*n; i+=3) {
    if ((int)points[i] == n) {
      points[i+1] = x_ + x;
      points[i+2] = y_ + y;
    }
  }
}

void Object::update_position(float dt) {
  x_ = x_ + vx_*dt;
  y_ = y_ + vy_*dt;
  update_all_points(dt);
}

void Object::update_all_points(float dt) {
  set_angle(phi_);
  for (int i=0; i<3*npoints_; i+=3) {
    points[i+1] = points[i+1] + vx_*dt;
    points[i+2] = points[i+2] + vy_*dt;
  }
}

Object::~Object() {

}