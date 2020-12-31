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