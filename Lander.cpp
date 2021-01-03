#include "Lander.h"
#define _USE_MATH_DEFINES
#include <math.h>

Lander::Lander(float x, float y, float hsize, float vsize, float angle, float spin, int maxFuel) 
    : Object(x, y, hsize, vsize, angle, spin), m_thrust(0.0), m_fuel(maxFuel), m_maxFuel(maxFuel), m_burnRate(1) {
    //save all relevant points to draw the lander (the points are defined
    //in the object-own coordinate system)
    //thruster
    newPoint( 1, 0);
    newPoint(-1, 0);
    newPoint(-3, 5);
    newPoint( 3, 5);
    newPoint( 1, 0);
    
    //fire
    newPoint( 3, 5);
    newPoint( 2, 5);
    newPoint( 0, 5, 255, 0, 0, 255, false); //8th point is not collidable (tip of flare)
    newPoint(-2, 5, 255, 0, 0);
    newPoint( 3, 5);
    newPoint( 1, 0);
    
    //stilts
    newPoint(-4, 0);
    newPoint(-7, 7);
    newPoint(-8, 7);
    newPoint(-6, 7);
    newPoint(-7, 7);
    newPoint(-4, 0);
    newPoint( 4, 0);
    newPoint( 7, 7);
    newPoint( 8, 7);
    newPoint( 6, 7);
    newPoint( 7, 7);
    newPoint( 4, 0);
    
    //bottom
    /*newPoint(-6, 0);
    newPoint( 6, 0);
    newPoint( 6,-2);
    newPoint(-6,-2);
    newPoint(-6, 0);
    newPoint(-6,-2);*/
    
    //capsula
    /*newPoint(-4, -2);
    newPoint( 4, -2);
    newPoint( 5, -3);
    newPoint( 5, -9);
    newPoint( 4,-10);
    newPoint(-4,-10);
    newPoint(-5, -9);
    newPoint(-5, -3);
    newPoint(-4, -2);*/
    newPoint(-4, 0);
    newPoint( 4, 0);
    newPoint( 5, -1);
    newPoint( 5, -7);
    newPoint( 4,-8);
    newPoint(-4,-8);
    newPoint(-5, -7);
    newPoint(-5, -1);
    newPoint(-4, 0);
}

void Lander::setThrust(float thr) {
    m_thrust = thr;
    m_fuel -= m_burnRate * thr;
    if (m_fuel < 0) m_fuel = 0;
    this->modifyPoint(-sin(phi()) * (5 + thr * 2), cos(phi()) * (5 + thr * 2), 8);
}

float Lander::getThrust() const {
    return m_thrust;
}

void Lander::setFuel(int fuel) {
    m_fuel = fuel;
}

int Lander::getFuel() const {
    return m_fuel;
}

int Lander::getMaxFuel() const {
    return m_maxFuel;
}

void Lander::setBurnRate(float burnRate) {
    m_burnRate = burnRate;
}