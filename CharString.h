/*
 *  String.h
 *  Created by Matthias Kesenheimer on 03.01.21.
 *  Copyright 2021. All rights reserved.
 */
#pragma once
#include "Object.h"
#include "Character.h"
#include <string.h>

class CharString : public Object {
  public:
    CharString(float x, float y, float hsize, float vsize, float angle, const std::string& str, int r, int g, int b);
};