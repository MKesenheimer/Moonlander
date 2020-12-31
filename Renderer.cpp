#include "Renderer.h"
#include <SDL2_gfxPrimitives.h>

// Draw an Object to a SDL_Renderer
void Renderer::drawObject(Object *object, SDL_Renderer *ren) {
    std::vector<float> temp;
    temp = object->get_point(1);
    int xp_old = (int)(temp[0]);
    int yp_old = (int)(temp[1]);
    for (int i=1; i<object->npoints(); ++i) {
        temp = object->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        //filledEllipseRGBA(ren, xp, yp, 0, 0, 0, 0, 0, 255);
        lineRGBA(ren, xp, yp, xp_old, yp_old, 0, 0, 0, 255);
        xp_old = xp;
        yp_old = yp;
    }
}

#ifdef LUMAX_OUTPUT
void Renderer::addPoint(lumaxRenderer& ren, int x, int y, int r, int g, int b, float xScaling, float yScaling) {
    if (!mirrorXAxis && !mirrorYAxis)
        ren.push_back({(int)(x * xScaling), (int)(y * yScaling), r, g, b});
    else if (!mirrorXAxis && mirrorYAxis)
        ren.push_back({(int)(x * xScaling), 32768 - (int)(y * yScaling), r, g, b});
    else if (mirrorXAxis && !mirrorYAxis)
        ren.push_back({32768 - (int)(x * xScaling), (int)(y * yScaling), r, g, b});
    else if (mirrorXAxis && mirrorYAxis)
        ren.push_back({32768 - (int)(x * xScaling), 32768 - (int)(y * yScaling), r, g, b});
}

// TODO: use the color of the object
// Draw an Object to a SDL_Renderer
void Renderer::drawObject(Object *object, lumaxRenderer& ren, float xScaling, float yScaling) {
    std::vector<float> temp;
    temp = object->get_point(1);
    int xp_old = (int)(temp[0]);
    int yp_old = (int)(temp[1]);
    addPoint(ren, xp_old, yp_old, 0, 0, 0, xScaling, yScaling); // start with a dark point
    for (int i = 1; i < object->npoints(); ++i) {
        temp = object->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        addPoint(ren, xp, yp, 255*256, 255*256, 150*256, xScaling, yScaling);
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
        points[i].Ch1 = ren[i].x;
        points[i].Ch2 = ren[i].y;
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