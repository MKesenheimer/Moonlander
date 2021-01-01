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
    Lander(float x, float y, float hsize, float vsize, float angle, float spin);

    void setThrust(float thr);
    float thrust() const;

private:
    float m_thrust;
};