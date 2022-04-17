/*
 *  Characters.h
 *  Created by Matthias Kesenheimer on 02.01.21.
 *  Copyright 2021. All rights reserved.
 */
#pragma once
#include "GameLibrary/Object.h"

class Character : public Object {
  public:
    Character(float x, float y, float hsize, float vsize, char c, int r, int g, int b);
};