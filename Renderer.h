/*
 *  Renderer.h
 *  Created by Matthias Kesenheimer on 31.12.20.
 *  Copyright 2020. All rights reserved.
 */
#pragma once
#include <SDL.h>
#include "Object.h"
#include "Point.h"

#define LUMAX_OUTPUT
#ifdef LUMAX_OUTPUT
extern "C" {
#include "lumax.h"
}
// maximum of possible points for the laser output
const int LASERDIMENSION = 65536;
const bool mirrorXAxis = true;
const bool mirrorYAxis = true;
#endif

class Renderer {
  public:
    static void drawObject(Object *object, SDL_Renderer *ren);

#ifdef LUMAX_OUTPUT
    typedef std::vector<Point> lumaxRenderer;

    static void drawObject(Object *object, lumaxRenderer& ren, float xScaling, float yScaling);

    static int sendPointsToLumax(void *lumaxHandle, lumaxRenderer& ren);

  private:
    static void addPoint(lumaxRenderer& ren, float x, float y, int r, int g, int b, float xScaling, float yScaling);
#endif
};