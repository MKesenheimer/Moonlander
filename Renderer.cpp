#include "Renderer.h"
#include <SDL2_gfxPrimitives.h>

// Draw an Object to a SDL_Renderer
void Renderer::drawObject(Object *object, SDL_Renderer *ren) {
    Point point = object->getPoint(1);
    int xp_old = (int)(point.x);
    int yp_old = (int)(point.y);
    for (int i = 1; i < object->npoints(); ++i) {
        point = object->getPoint(i);
        int xp = (int)(point.x);
        int yp = (int)(point.y);
        //filledEllipseRGBA(ren, xp, yp, 0, 0, 0, 0, 0, 255);
        lineRGBA(ren, xp, yp, xp_old, yp_old, 0, 0, 0, 255);
        xp_old = xp;
        yp_old = yp;
    }
}

#ifdef LUMAX_OUTPUT
void Renderer::addPoint(lumaxRenderer& ren, float x, float y, int r, int g, int b, float xScaling, float yScaling) {
    if (!mirrorXAxis && !mirrorYAxis)
        ren.push_back({x * xScaling, y * yScaling, r, g, b});
    else if (!mirrorXAxis && mirrorYAxis)
        ren.push_back({x * xScaling, LASERDIMENSION / 2 - (y * yScaling), r, g, b});
    else if (mirrorXAxis && !mirrorYAxis)
        ren.push_back({LASERDIMENSION / 2 - (x * xScaling), y * yScaling, r, g, b});
    else if (mirrorXAxis && mirrorYAxis)
        ren.push_back({LASERDIMENSION / 2 - (x * xScaling), LASERDIMENSION / 2 - (y * yScaling), r, g, b});
}

// TODO: use the color of the object
// Draw an Object to a SDL_Renderer
void Renderer::drawObject(Object *object, lumaxRenderer& ren, float xScaling, float yScaling) {
    Point point = object->getPoint(1);
    float xp_old = point.x;
    float yp_old = point.y;
    addPoint(ren, xp_old, yp_old, 0, 0, 0, xScaling, yScaling); // start with a dark point
    for (int i = 1; i < object->npoints(); ++i) {
        point = object->getPoint(i);
        float xp = point.x;
        float yp = point.y;
        addPoint(ren, xp, yp, 120*256, 130*256, 150*256, xScaling, yScaling);
        xp_old = xp;
        yp_old = yp;
    }
    addPoint(ren, xp_old, yp_old, 0, 0, 0, xScaling, yScaling); // end with a dark point
}

int Renderer::sendPointsToLumax(void *lumaxHandle, lumaxRenderer& ren) {
    if (!lumaxHandle) return -1;
    //lumax_verbosity |= DBG_WAITFORBUFFER;
    size_t numOfPoints = ren.size();
    TLumax_Point points[numOfPoints];
    for (int i = 0; i < numOfPoints; ++i) {
        points[i].Ch1 = static_cast<int>(ren[i].x);
        points[i].Ch2 = static_cast<int>(ren[i].y);
        points[i].Ch3 = ren[i].r;
        points[i].Ch4 = ren[i].g;
        points[i].Ch5 = ren[i].b;
        points[i].Ch8 = 0;
        points[i].Ch6 = 0;
        points[i].Ch7 = 0;
    }
    int result = Lumax_SendFrame(lumaxHandle, points, numOfPoints, 1500, 0, NULL);
    int TimeToWait, BufferChanged;
    result = Lumax_WaitForBuffer(lumaxHandle, 17, &TimeToWait, &BufferChanged);
    ren.clear();
    return 0;
}
#endif