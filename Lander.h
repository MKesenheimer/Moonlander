/*
 *  Lander.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include "GameLibrary/Object.h"

class Lander : public Object {
public:
    Lander(float x, float y, float vx, float vy, float hsize, float vsize, float angle, float spin, int maxFuel = 2000);

    void setThrust(float thr);

    float getThrust() const;

    void setFuel(int fuel);

    int getFuel() const;

    int getMaxFuel() const;

    void setBurnRate(float burnRate);

private:
    float m_thrust;
    int m_fuel;
    int m_maxFuel;
    float m_burnRate;
};