#include "CharString.h"
#include <iostream>

CharString::CharString(float x, float y, float hsize, float vsize, float angle, const std::string& str, int r, int g, int b)
    : Object(x, y, hsize, vsize, angle, 0) {
    float dist = 0;
    for (const auto& c : str) {
        if (c != ' ') {
            Character chr(0, 0, 1, 1, c, r, g, b);
            newPoint(chr.getPoint(1).x + dist, chr.getPoint(1).y, 0, 0, 0, 0);
            for (int i = 1; i < chr.npoints(); ++i)
                newPoint(chr.getPoint(i).x + dist, chr.getPoint(i).y, r, g, b);
        }
        dist += 0.8;
    }
}