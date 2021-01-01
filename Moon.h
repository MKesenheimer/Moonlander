/*
 *  Moon.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include <vector>
#include "Object.h"

class Moon : public Object {
public:
    Moon(float x, float y, float hsize, float vsize, float angle);
};