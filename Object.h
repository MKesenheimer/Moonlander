/*
 *  Object.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include <tuple>
#include <vector>
#include "Point.h"

// TODO: add color to each point

class Object {
public:
    // the constructor
    Object(float x, float y, float hsize = 1, float vsize = 1, float angle = 0,
        float spin = 0, int mirrorX = 1, int mirrorY = 1);
    
    // gives the x and y position in the 2 dimensional world
    float x() const;
    float y() const;
    float xcenter() const;
    float ycenter() const;
    // gives the velocity in x- and y-direction of the object
    float vx() const;
    float vy() const;
    // get the angle in respect to the y-axis
    float phi() const;
    float spin() const;
    // gives the horizontal size
    float hsize() const;
    // gives the verticla size
    float vsize() const;
    // get the number of points
    int npoints() const;
    
    // set the x and y position in the 2 dimensional world
    void setPos(float x, float y);
    void setv(float vx, float vy);
    
    // set the objects angle
    void setAngle(float angle);
    void setSpin(float spin);
    
    // update the new position of the Object
    void updatePosition(float dt);
    
    // save a new coordinate and remember the index. iscol is used to determine
    // if point should be used for collision control
    void newPoint(float x, float y, bool iscol = true);
    void newPoint(float x, float y, int r, int g, int b, int a = 255, bool iscol = true); // with color
    Point getPoint(int n) const;
    std::pair<float, float> getPointXY(int n) const;
    bool isCollidable(int n) const;
    void modifyPoint(float x, float y, int n);

private:
    float m_x;
    float m_y;
    float m_hsize;
    float m_vsize;
    float m_vx;
    float m_vy;
    float m_phi; // angle in degree (°)
    float m_oldPhi; 
    // whenever save_point() is called, we increment this numbers
    int m_npoints;
    float m_spin;
    int m_mirrorX;
    int m_mirrorY;
    std::vector<Point> m_points; // points are defined in the object coordinate system
};