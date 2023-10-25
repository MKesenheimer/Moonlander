/*
 *  charstring.h
 *  Created by Matthias Kesenheimer on 03.01.21.
 *  Copyright 2021. All rights reserved.
 */
#pragma once
#include "GameLibrary/object.h"
#include "character.h"
#include <string>

class CharString : public object {
  public:
    CharString(float x, float y, float hsize, float vsize, float angle, const std::string& str, int r, int g, int b);
};
