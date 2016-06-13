#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include "Object.h"

class Collision {
  public:
    Collision();
    ~Collision();
    
    //this function takes two lists of points and checks if the
    //the objects formed by the points has collided
    //Note: the points should form an convex object at best
    bool check_collision(Object *o1, Object *o2);

    //calculates the center of the object formed by the points
    // returns the x- and y- coordinate in a vector
    std::vector<float> center(Object *o);
    
    //returns the length of the largest distance from center
    //this assumed to be the dimension of the object
    float dim(Object *o);
    
    //calculate the distance of two objects
    float dist(Object *o1, Object *o2);
    
  private:
    bool collided;

};

#endif