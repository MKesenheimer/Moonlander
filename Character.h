/*
 *  characters.h
 *  Created by Matthias Kesenheimer on 02.01.21.
 *  Copyright 2021. All rights reserved.
 */
#pragma once
#include "GameLibrary/object.h"

class Character : public object {
  public:
    Character(float x, float y, float hsize, float vsize, char c, int r, int g, int b);
};