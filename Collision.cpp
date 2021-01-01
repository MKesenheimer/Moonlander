#include "Collision.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

bool Collision::checkCollision(const Object& o1, const Object& o2) {
    bool collided = false;
    //first check: if the sum of the dimensions of the objects is less
    //then their distance from center to center -> enable advanced collision detection
    const float dim1 = dim(o1);
    const float dim2 = dim(o2);
    if(dim1 + dim2 >= dist(o1, o2)) {
        //advanced collision control
        if(dim1 <= dim2) {
            //start of line between two points
            std::pair<float, float> start = o2.getPoint(0);
            for (int i=1; i<o2.npoints(); ++i) {
                //end of line
                std::pair<float, float> end = o2.getPoint(i);
                //number of points on line
                const int nsteps = (int)(10 * dist(start, end) / dim1);
                for (int j = 0; j < nsteps; ++j) {
                    //build new points that lay on the line
                    float x = start.first + j * (end.first - start.first) / nsteps;
                    float y = start.second + j * (end.second - start.second) / nsteps;
                    std::pair<float, float> check = {x, y};
                    if(dist(o1, check) <= dim1) collided = true;
                }
                start = end;
            }
        } else {
            //start of line between two points
            std::pair<float, float> start = o1.getPoint(0);
            for (int i = 1; i < o1.npoints(); ++i) {
                //end of line
                std::pair<float, float> end = o1.getPoint(i);
                //number of points on line
                const int nsteps = (int)(10 * dist(start, end) / dim2);
                for (int j = 0; j < nsteps; ++j) {
                    //build new points that lay on the line
                    float x = start.first + j * (end.first - start.first) / nsteps;
                    float y = start.second + j * (end.second - start.second) / nsteps;
                    std::pair<float, float> check = {x, y};
                    if(dist(o2, check) <= dim2) collided = true;
                }
                start = end;
            }
        }
    }
    return collided;
}

float Collision::dist(const Object& o1, const Object& o2) {
    const std::pair<float, float>& cen1 = center(o1);
    const std::pair<float, float>& cen2 = center(o2);
    //calculate the distance of the two objects
    const float dist = sqrt(pow(cen1.first - cen2.first, 2) + pow(cen1.second - cen2.second, 2));
    return dist;
}

float Collision::dist(const Object& o, const std::pair<float, float>& point) {
    const std::pair<float, float>& cen = center(o);
    const float dist = sqrt(pow(cen.first - point.first, 2) + pow(cen.second - point.second, 2));
    return dist;
}

float Collision::dist(const std::pair<float, float>& point0, const std::pair<float, float>& point1) {
    const float dist = sqrt(pow(point0.first - point1.first, 2) + pow(point0.second - point1.second, 2));
    return dist;
}

std::pair<float, float> Collision::center(const Object& o) {
    const std::pair<float, float> temp = {o.x(), o.y()};
    return temp;
}

float Collision::dim(const Object& o) {
    std::pair<float, float> r, r0 = center(o);
    float rAbs = 0;
    float x, y;
    for(int i = 1; i < o.npoints(); ++i) {
        if(o.isCollidable(i)) {
            r = o.getPoint(i);
            x = r.first - r0.first;
            y = r.second - r0.second;
            if(rAbs <= sqrt(pow(x, 2) + pow(y, 2)))
                rAbs = sqrt(pow(x, 2) + pow(y, 2));
        }
    }
    return rAbs;
}