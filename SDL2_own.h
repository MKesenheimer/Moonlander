#ifndef SDL2_OWN_H
#define SDL2_OWN_H

// Draw a curled line between two points on the screen renderer with a given amplitude and a number of half waves.
int curlRGBA(SDL_Renderer * renderer, const Sint16 x1, const Sint16 y1, const Sint16 x2, const Sint16 y2,
             const Sint16 amp,const Sint16 num_half_waves, int steps, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


#endif