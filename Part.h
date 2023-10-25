/*
 *  Part.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include "GameLibrary/object.h"

class Part : public object {
  public:
    Part(float x, float y, float vx, float vy, float hsize, float vsize, float angle, float spin);
};