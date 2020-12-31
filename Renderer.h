/*
 *  Renderer.h
 *  Created by Matthias Kesenheimer on 31.12.20.
 *  Copyright 2020. All rights reserved.
 */
#pragma once
#include "Object.h"
#include <SDL.h>

class Renderer {
  public:
    static void drawObject(Object *object, SDL_Renderer *ren);
};