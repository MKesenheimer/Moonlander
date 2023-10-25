/*
 *  moon.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include "GameLibrary/object.h"

class Moon : public object {
public:
    Moon(float x, float y, float hsize, float vsize, float angle);
};