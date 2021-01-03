#include "Character.h"
#include <cmath>
#include <map>
#include <tuple>
#include <iostream>
#include <array>
#include <vector>

constexpr float ysize = 1.0;
constexpr float xsize = 0.5;
constexpr int ysteps = 5;
constexpr int xsteps = 4;
constexpr float ydelta = ysize / (ysteps - 1);
constexpr float xdelta = xsize / (xsteps - 1);

typedef std::vector<int> v;
const std::array<std::array<std::array<v, xsteps>, ysteps>, 36> mat = {
    // '0'
    v({0}), v({8}), v({1, 9}), v({0}),
    v({7}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({0}), v({3}),
    v({0}), v({5, 10}), v({4}), v({0}),
    
    // '1'
    v({0}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({1}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({3}),

    // '2'
    v({0}), v({2}), v({3}), v({0}),
    v({1}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({5}), v({0}), v({0}), v({6}),

    // '3'
    v({1}), v({0}), v({2}), v({0}),
    v({0}), v({0}), v({0}), v({3}),
    v({0}), v({5}), v({4, 6}), v({0}),
    v({0}), v({0}), v({0}), v({7}),
    v({9}), v({0}), v({8}), v({0}),

    // 4
    v({0}), v({0}), v({2}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({3}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({1}), v({0}),

    // 5
    v({2}), v({0}), v({0}), v({1}),
    v({0}), v({0}), v({0}), v({0}),
    v({3}), v({0}), v({4}), v({0}),
    v({0}), v({0}), v({0}), v({5}),
    v({7}), v({0}), v({6}), v({0}),

    // '6'
    v({0}), v({2}), v({0}), v({1}),
    v({3}), v({0}), v({0}), v({0}),
    v({9}), v({0}), v({8}), v({0}),
    v({4}), v({0}), v({0}), v({7}),
    v({0}), v({5}), v({6}), v({0}),

    // '7'
    v({1}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({3}), v({0}),

    // '8'
    v({0}), v({1, 11}), v({2}), v({0}),
    v({10}), v({0}), v({0}), v({3}),
    v({0}), v({4}), v({9}), v({0}),
    v({5}), v({0}), v({0}), v({8}),
    v({0}), v({6}), v({7}), v({0}),

    // '9'
    v({0}), v({6}), v({5}), v({0}),
    v({7}), v({0}), v({0}), v({4}),
    v({0}), v({8}), v({0}), v({9}),
    v({0}), v({0}), v({0}), v({3}),
    v({1}), v({0}), v({2}), v({0}),

    // 'a'
    v({0}), v({2}), v({3}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({6}), v({5}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({1}), v({0}), v({0}), v({4}),

    // 'b'
    v({1, 10}), v({0}), v({2}), v({0}),
    v({0}), v({0}), v({0}), v({3}),
    v({5}), v({0}), v({4, 6}), v({0}),
    v({0}), v({0}), v({0}), v({7}),
    v({9}), v({0}), v({8}), v({0}),

    // 'c'
    v({0}), v({2}), v({0}), v({1}),
    v({3}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({4}), v({0}), v({0}), v({0}),
    v({0}), v({5}), v({0}), v({6}),

    // 'd'
    v({1, 7}), v({0}), v({6}), v({0}),
    v({0}), v({0}), v({0}), v({5}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({4}),
    v({2}), v({0}), v({3}), v({0}),

    // 'e'
    v({2}), v({0}), v({0}), v({1}),
    v({0}), v({0}), v({0}), v({0}),
    v({3, 5}), v({0}), v({4}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({0}), v({7}),

    // 'f'
    v({2}), v({0}), v({0}), v({1}),
    v({0}), v({0}), v({0}), v({0}),
    v({3, 5}), v({0}), v({4}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({0}), v({0}),

    // 'G'
    v({0}), v({2}), v({0}), v({1}),
    v({3}), v({0}), v({0}), v({0}),
    v({0}), v({9}), v({0}), v({8}),
    v({4}), v({0}), v({0}), v({7}),
    v({0}), v({5}), v({6}), v({0}),

    // 'h'
    v({1}), v({0}), v({0}), v({6}),
    v({0}), v({0}), v({0}), v({0}),
    v({3}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({2}), v({0}), v({0}), v({5}),

    // 'i'
    v({1}), v({0}), v({3}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({4}), v({5}),

    // 'J'
    v({1}), v({0}), v({3}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({4}), v({0}),
    v({0}), v({5}), v({0}), v({0}),

    // 'K'
    v({1}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({3, 5}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({2}), v({0}), v({0}), v({6}),

    // 'L'
    v({1}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({2}), v({0}), v({0}), v({3}),

    // 'M'
    v({2}), v({0}), v({0}), v({5}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({3}), v({4}), v({0}),
    v({1}), v({0}), v({0}), v({6}),

    // 'N'
    v({2}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({1}), v({0}), v({0}), v({3}),

    // 'O'
    v({0}), v({8}), v({1, 9}), v({0}),
    v({7}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({0}), v({3}),
    v({0}), v({5}), v({4}), v({0}),

    // 'P'
    v({2}), v({0}), v({3}), v({0}),
    v({0}), v({0}), v({0}), v({4}),
    v({6}), v({0}), v({5}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({1}), v({0}), v({0}), v({0}),

    // 'Q'
    v({0}), v({4}), v({3}), v({0}),
    v({5}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({6}), v({0}), v({10}), v({9}),
    v({0}), v({7}), v({8}), v({1}),

    // 'R'
    v({2}), v({0}), v({3}), v({0}),
    v({0}), v({0}), v({0}), v({4}),
    v({6}), v({0}), v({5}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({1}), v({0}), v({0}), v({7}),

    // 'S'
    v({0}), v({2}), v({0}), v({1}),
    v({3}), v({0}), v({0}), v({0}),
    v({0}), v({4}), v({5}), v({0}),
    v({0}), v({0}), v({0}), v({6}),
    v({8}), v({0}), v({7}), v({0}),

    // 'T'
    v({1}), v({0}), v({3}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({4}), v({0}),

    // 'U'
    v({1}), v({0}), v({0}), v({6}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({2}), v({0}), v({0}), v({5}),
    v({0}), v({3}), v({4}), v({0}),

    // 'V'
    v({1}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({2}), v({3}), v({0}),

    // 'W'
    v({1}), v({0}), v({0}), v({6}),
    v({0}), v({3}), v({4}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({2}), v({0}), v({0}), v({5}),

    // 'X'
    v({1}), v({0}), v({0}), v({4}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({3}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({5}), v({0}), v({0}), v({2}),

    // 'Y'
    v({0}), v({1}), v({0}), v({5}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({2, 4}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({3}), v({0}),

    // 'Z'
    v({1}), v({0}), v({0}), v({2}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({0}), v({0}), v({0}), v({0}),
    v({3}), v({0}), v({0}), v({4})
};

Character::Character(float x, float y, float hsize, float vsize, char c, int r, int g, int b) 
    : Object(x, y, hsize, vsize, 0, 0, 1, 1) {
    int selector = 0;
    switch (c) {
    case '0':
        selector = 0;
        break;
    case '1':
        selector = 1;
        break;
    case '2':
        selector = 2;
        break;
    case '3':
        selector = 3;
        break;
    case '4':
        selector = 4;
        break;
    case '5':
        selector = 5;
        break;
    case '6':
        selector = 6;
        break;
    case '7':
        selector = 7;
        break;
    case '8':
        selector = 8;
        break;
    case '9':
        selector = 9;
        break;
    case 'a':
    case 'A':
        selector = 10;
        break;
    case 'b':
    case 'B':
        selector = 11;
        break;
    case 'c':
    case 'C':
        selector = 12;
        break;
    case 'd':
    case 'D':
        selector = 13;
        break;
    case 'e':
    case 'E':
        selector = 14;
        break;
    case 'f':
    case 'F':
        selector = 15;
        break;
    case 'g':
    case 'G':
        selector = 16;
        break;
    case 'h':
    case 'H':
        selector = 17;
        break;
    case 'i':
    case 'I':
        selector = 18;
        break;
    case 'j':
    case 'J':
        selector = 19;
        break;
    case 'k':
    case 'K':
        selector = 20;
        break;
    case 'l':
    case 'L':
        selector = 21;
        break;
    case 'm':
    case 'M':
        selector = 22;
        break;
    case 'n':
    case 'N':
        selector = 23;
        break;
    case 'o':
    case 'O':
        selector = 24;
        break;
    case 'p':
    case 'P':
        selector = 25;
        break;
    case 'q':
    case 'Q':
        selector = 26;
        break;
    case 'r':
    case 'R':
        selector = 27;
        break;
    case 's':
    case 'S':
        selector = 28;
        break;
    case 't':
    case 'T':
        selector = 29;
        break;
    case 'u':
    case 'U':
        selector = 30;
        break;
    case 'v':
    case 'V':
        selector = 31;
        break;
    case 'w':
    case 'W':
        selector = 32;
        break;
    case 'x':
    case 'X':
        selector = 33;
        break;
    case 'y':
    case 'Y':
        selector = 34;
        break;
    case 'z':
    case 'Z':
        selector = 35;
        break;
    default:
        break;
    }

    // fo through all numbers of the matrix and add the respective position
    // as a new point in a map. The map is ordered.
    std::map<int, std::pair<float, float>> points;
    for (int i = 0; i < ysteps; ++i)
        for (int j = 0; j < xsteps; ++j)
            for (int k = 0; k < mat[selector][i][j].size(); ++k)
                if (mat[selector][i][j][k] != 0)
                    points.emplace(mat[selector][i][j][k], std::pair<float, float>(j * xdelta, i * ydelta));

    for (int i = 1; i <= points.size(); ++i)
            newPoint(points[i].first, points[i].second, r, g, b);
}