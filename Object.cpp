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
    for (int i=0; i<m_npoints; ++i) {
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
    if (m_npoints == 0) {
        // center of object
        m_points.push_back(point());
        m_points[0].index = m_npoints;
        m_points[0].x = x;
        m_points[0].y = y;
        m_points[0].iscollidable = iscol;
        m_npoints++;
    }
    else {
        m_points.push_back(point());
        m_points[m_npoints].index = m_npoints;
        // move all object m_points back into the world coordinate system
        m_points[m_npoints].x = m_hsize * x;
        m_points[m_npoints].y = m_vsize * y;
        m_points[m_npoints].iscollidable = iscol;
        m_npoints++;
    }
}

std::pair<float, float> Object::getPoint(int n) const {
    std::pair<float, float> retv;
    for (int i = 0; i <= n; ++i) {
        if (m_points[i].index == n) {
            retv.first = m_points[i].x + m_x;
            retv.second = m_points[i].y + m_y;
            return retv;
        }
    }
    std::cout << "an error occured in Object.cpp: n = " << n << " is not a valid index" << std::endl;
    retv.first = 0;
    retv.second = 0;
    return retv;
}

bool Object::isCollidable(int n) const {
    for (int i=0; i<=n; ++i) {
        if (m_points[i].index == n)
            return m_points[i].iscollidable;
    }
    std::cout << "an error occured in Object.cpp: n = " << n << " is not a valid index" << std::endl;
    return true;
}

void Object::modifyPoint(float x, float y, int n) {
    for (int i = 0; i <= n; ++i) {
        if (m_points[i].index == n) {
            m_points[i].x = m_hsize * x;
            m_points[i].y = m_vsize * y;
        }
    }
}

void Object::updatePosition(float dt) {
    m_x = m_x + m_vx * dt;
    m_y = m_y + m_vy * dt;
    m_phi = m_phi + m_spin * dt;
    setAngle(m_phi);
}