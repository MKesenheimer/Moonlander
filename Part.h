/*
 *  Part.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include "Object.h"

class Part : public Object {
  public:
    Part(float x, float y, float hsize, float vsize, float angle, float spin);
};