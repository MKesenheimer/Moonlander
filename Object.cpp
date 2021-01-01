#include "Object.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

Object::Object(float x, float y, float hsize, float vsize, float angle, float spin) :
    m_x(x), m_y(y), m_hsize(hsize), m_vsize(vsize), m_phi(angle), m_oldPhi(0.0), m_npoints(0), m_spin(spin) {
    newPoint(m_x, m_y);
}

float Object::x() const {
    return m_x;
}

float Object::y() const {
    return m_y;
}

float Object::xcenter() const {
    return m_x;
}

float Object::ycenter() const {
    return m_y;
}

float Object::vx() const {
    return m_vx;
}

float Object::vy() const {
    return m_vy;
}

float Object::phi() const {
    return m_phi;
}

float Object::spin() const {
    return m_spin;
}

float Object::hsize() const {
    return m_hsize;
}

float Object::vsize() const {
    return m_vsize;
}

int Object::npoints() const {
    return m_npoints;
}

void Object::setPos(float x, float y) {
    m_x = x;
    m_y = y;
}

void Object::setv(float vx, float vy) {
    m_vx = vx;
    m_vy = vy;
}

void Object::setAngle(float angle) {
    m_phi = angle;
    // this algorithm rotates the object every time it is called
    // so we have to rotate only by the difference of  the old and the
    // new angle
    float dphi = (m_phi - m_oldPhi); //angle in rad /360.0*(2*M_PI)
    for (int i = 0; i < m_npoints; ++i) {
        // rotate all m_points
        float xfs = m_points[i].x;
        float yfs = m_points[i].y;
        m_points[i].x = std::cos(dphi) * xfs - std::sin(dphi) * yfs;
        m_points[i].y = std::sin(dphi) * xfs + std::cos(dphi) * yfs;
    }
    m_oldPhi = m_phi;
}

void Object::setSpin(float spin) {
    m_spin = spin;
}

void Object::newPoint(float x, float y, bool iscol) {
    newPoint(x, y, 0, 0, 0, iscol);
}

void Object::newPoint(float x, float y, int r, int g, int b, bool iscol) {
    m_points.push_back(Point());
    if (m_npoints == 0) {
        // center of object
        m_points[0].x = x;
        m_points[0].y = y;
    } else {
        // move all object m_points back into the world coordinate system
        m_points[m_npoints].x = m_hsize * x;
        m_points[m_npoints].y = m_vsize * y;
    }
    m_points[m_npoints].r = r;
    m_points[m_npoints].g = g;
    m_points[m_npoints].b = b;
    m_points[m_npoints].iscollidable = iscol;
    m_npoints++;
}

std::pair<float, float> Object::getPointXY(int n) const {
    std::pair<float, float> retv;
    if (n >= 0 && n < m_npoints) {
        retv.first = m_points[n].x + m_x;
        retv.second = m_points[n].y + m_y;
        return retv;
    }
    std::cout << "an error occured in Object.cpp: n = " << n << " is not a valid index" << std::endl;
    retv.first = 0;
    retv.second = 0;
    return retv;
}

Point Object::getPoint(int n) const {
    if (n >= 0 && n < m_npoints)
        return m_points[n];
    std::cout << "an error occured in Object.cpp: n = " << n << " is not a valid index" << std::endl;
    return Point();
}


bool Object::isCollidable(int n) const {
    if (n >= 0 && n < m_npoints)
        return m_points[n].iscollidable;
    std::cout << "an error occured in Object.cpp: n = " << n << " is not a valid index" << std::endl;
    return true;
}

void Object::modifyPoint(float x, float y, int n) {
    if (n >= 0 && n < m_npoints) {
        m_points[n].x = m_hsize * x;
        m_points[n].y = m_vsize * y;
    }
}

void Object::updatePosition(float dt) {
    m_x = m_x + m_vx * dt;
    m_y = m_y + m_vy * dt;
    m_phi = m_phi + m_spin * dt;
    setAngle(m_phi);
}