/*
 *  Lander.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
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