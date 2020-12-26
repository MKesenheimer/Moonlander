#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "SDL2_own.h"
#include "Main.h"
#include "Timer.h"
#include "Lander.h"
#include "Part.h"
#include "Moon.h"
#include "cleanup.h"
#include "Functions.h"
#include "Collision.h"

#define LUMAX_OUTPUT
#ifdef LUMAX_OUTPUT
extern "C" {
#include "lumax.h"
}
#endif

using namespace::std;

const int SCREEN_WIDTH  = 900;
const int SCREEN_HEIGHT = 800;
const int FRAMES_PER_SECOND = 20;            //Fps auf 20 festlegen


// TODO: entfernen, das ist doch blöd
//Log an SDL error with some error message to the output stream of our choice
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}
//Log an SDL debug output with some message to the output stream of our choice
void logSDLDebug(std::ostream &os, const std::string &msg){
    os << " [DEBUG]: " << msg << std::endl;
}
void logSDLDebug(std::ostream &os, const int msg){
    os << " [DEBUG]: " << msg << std::endl;
}

//Loads an image into a texture on the rendering device
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == NULL){
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

//Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
//width and height
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

//Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
//the texture's width and height
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

// Draw an Object to a SDL_Renderer
void drawObject(Object *object, SDL_Renderer *ren) {
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
struct point {
    int x, y;
    int r, g, b;
};
// TODO, nicht global definieren, sondern als Argument übergeben
std::vector<point> points;

// TODO: 32768 global und einheitlich definieren
// TODO: Spiegeln und skalieren als Parameter übergeben
// Draw an Object to a SDL_Renderer
void drawObject(Object *object) {
    const float xScaling = (float)(32768 / (2 * SCREEN_WIDTH));
    const float yScaling = (float)(32768 / (2 * SCREEN_HEIGHT));
    //const int mid = 
    std::vector<float> temp;
    temp = object->get_point(1);
    int xp_old = (int)(temp[0]);
    int yp_old = (int)(temp[1]);
    point p0{32768 - (int)(xp_old * xScaling), 32768 - (int)(yp_old * yScaling), 0, 0, 0};
    points.push_back(p0);
    for (int i = 1; i < object->npoints(); ++i) {
        temp = object->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        point p1{32768 - (int)(xp * xScaling), 32768 - (int)(yp * yScaling), 255*256, 255*256, 150*256};
        points.push_back(p1);
        xp_old = xp;
        yp_old = yp;
    }
    point p2{32768 - (int)(xp_old * xScaling), 32768 - (int)(yp_old * yScaling), 0, 0, 0};
    points.push_back(p2);
}

int sendPointsToLumax(void *lumaxHandle) {
    if (!lumaxHandle) return -1;
    //lumax_verbosity |= DBG_WAITFORBUFFER;
    size_t numOfPoints = points.size();
    TLumax_Point Points[numOfPoints];
    for (int i = 0; i < numOfPoints; ++i) {
        Points[i].Ch1 = points[i].x;
        Points[i].Ch2 = points[i].y;
        Points[i].Ch3 = points[i].r;
        Points[i].Ch4 = points[i].g;
        Points[i].Ch5 = points[i].b;
        Points[i].Ch8 = 0;
        Points[i].Ch6 = 0;
        Points[i].Ch7 = 0;
    }
    int result = Lumax_SendFrame(lumaxHandle, Points, numOfPoints, 1500, 0, NULL);
    int TimeToWait, BufferChanged;
    result = Lumax_WaitForBuffer(lumaxHandle, 17, &TimeToWait, &BufferChanged);
    points.clear();
    return 0;
}
#endif

int main(int argc, char* args[]) {
    int frame = 0; //take records of frame number
    bool cap = true; //Framecap an oder ausschalten

    //Timer zum festlegen der FPS
    Timer fps;
    //Timer zum errechnen der weltweit vergangenen Zeit
    Timer worldtime;
    worldtime.start();

    //calculate the small time between two frames in ms
    int oldTime = 0;
    int newTime = 0;
    int dt = 0;

    //initialize random generator
    seed(time(NULL));

    //Start up SDL and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    //Setup our window and renderer, this time let's put our window in the center
    //of the screen
    SDL_Window *window = SDL_CreateWindow("Moonlander", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

#ifdef LUMAX_OUTPUT
    // open Lumax device
    int NumOfCards = Lumax_GetPhysicalDevices();
    void* lumaxHandle = NULL;
    printf("Number of MiniLumax devices: %i\n", NumOfCards);
    if (NumOfCards > 0)
    {
        lumaxHandle = Lumax_OpenDevice(1, 0);
        printf("Lumax_OpenDevice returned handle: 0x%lx\n", (unsigned long)lumaxHandle);
        if (lumaxHandle == NULL){
            logSDLError(std::cout, "Lumax_OpenDevice");
            cleanup(window, renderer);
            SDL_Quit();
            return 1;
        }
    }
#endif

    //generate new objects
    Lander lander(SCREEN_WIDTH/8, 50, 2, 2, -M_PI/2, 0);
    lander.set_v(1.0, 0.0);
    //debug
    //Lander lander(SCREEN_WIDTH/2,550,2,2,-M_PI/2);
    //lander.set_v(0.0,2.0);
    Moon moon(SCREEN_WIDTH/2, 3*SCREEN_HEIGHT, 2*SCREEN_WIDTH, 2*SCREEN_WIDTH, 0);
    std::vector<Part> parts;
    const int nparts = 35;
    for (int i = 0; i < nparts; ++i) {
      parts.push_back(Part(0, 0, frand(5,15), frand(5,15), frand(0,2*M_PI), frand(-0.5, 0.5)));
      parts[i].set_v(0.0, 0.0);
    }

    //collision detection
    Collision coll;

    //game mechanics
    bool quit = false;
    bool firstcontact = true;
    float x1 = 0, y1 = 0; //position of first contact
    float x0, y0; //moon position
    float x, y; //lander position
    float vx, vy; //lander velocity
    float r; //distance moon lander
    bool destroyed = false; //if the lander is destroyed
    bool collided = false;
    bool initparts = true;

    //constants for integration
    //newtons constant of gravity
    float gamma = 1e3;
    //energy dissipation during flight (for example air drag)
    float alpha = 0.0; //1e-4
    //energy dissipation during landing
    float beta = 0.0; //1e-3
    //strength of stilt springs
    float omega = 1e-3;

    //Our event structure
    SDL_Event e;

    while (!quit){
        //start the fps timer
        fps.start();

        //Read any events that occured, for now we'll just quit if any event occurs
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT){
                quit = true;
            }
            //If user presses any key
            else if (e.type == SDL_KEYDOWN){
                //do something
                if (e.key.keysym.sym == SDLK_c ) {
                    cap = !cap;
                }
            }
        }

        // handle keyboard inputs (no lags and delays!)
        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_UP]) {
            float vxf = lander.vx() + 0.03*sin(lander.phi());
            float vyf = lander.vy() - 0.03*cos(lander.phi());
            lander.set_v(vxf,vyf);
            if (lander.thrust() < 10) {
                lander.set_thrust(lander.thrust() + 3);
            }
        }
        if (!collided && keystate[SDL_SCANCODE_LEFT]) {
            //lander.set_spin(lander.spin() - 0.01);
            if (lander.phi() >= -M_PI) {
                lander.set_angle(lander.phi() - 0.15);
            }
        }
        if (!collided && keystate[SDL_SCANCODE_RIGHT]) {
            //lander.set_spin(lander.spin() + 0.01);
            if (lander.phi() <= M_PI) {
                lander.set_angle(lander.phi() + 0.15);
            }
        }

        //if no key pressed, reduce thrust
        if (lander.thrust() > 0) {
            lander.set_thrust(lander.thrust() - 1);
        }

        x0 = moon.x();
        y0 = moon.y();
        x = lander.x();
        y = lander.y();
        vx = lander.vx();
        vy = lander.vy();
        r = sqrt(pow(x-x0,2)+pow(y-y0,2));
        float dvx,  dvy;
        if (r <= 2000) { //atmosphere below 2000
            dvx = -gamma*(x-x0)/pow(r,3)*dt - fabs(alpha*vx*dt);
            dvy = -gamma*(y-y0)/pow(r,3)*dt - fabs(alpha*vy*dt);
        } else {
            dvx = -gamma*(x-x0)/pow(r,3)*dt;
            dvy = -gamma*(y-y0)/pow(r,3)*dt;
        }

        //collision detection
        collided = coll.check_collision(&lander,&moon);
        if (collided) {
            //remember point of first contact
            if (firstcontact) {
                x1 = x;
                y1 = y;
                firstcontact = false;

                //destroy the lander if velocity is to big
                if (sqrt(pow(vx,2)+pow(vy,2)) >= 1.0) {
                    destroyed = true;
                }
            }

            dvx = dvx - omega*(x-x1)*dt - fabs(beta*vx*dt);
            dvy = dvy - omega*(y-y1)*dt - fabs(beta*vy*dt);

            //stop the lander if successfully landed
            if (sqrt(pow(vx,2)+pow(vy,2)) <= 0.1 and lander.thrust() == 0) {
                vx = 0.0;
                vy = 0.0;
                dvx = 0.0;
                dvy = 0.0;

                //if the lander isn't upright, tilt it
                //print(lander.phi());
                if (lander.phi() > 0.4) {
                    lander.set_spin(lander.spin() + 0.001); //fall over
                }
                if (lander.phi() < -0.4) {
                    lander.set_spin(lander.spin() - 0.001); //fall over
                }
                if (lander.phi() < 0.4 && lander.phi() > 0) { //bring it to an upright position
                    lander.set_spin(lander.spin() - 0.001);
                }
                if (lander.phi() > -0.4 && lander.phi() < 0) { //upright position
                    lander.set_spin(lander.spin() + 0.001);
                }
                if (fabs(lander.phi()) > 1.7) { //destroy it
                    destroyed = true;
                }
                if (fabs(lander.phi()) < 0.1) {
                    lander.set_spin(0);
                }
            }

            //TODO: if the lander lands on a wall

        } else {
            //reset variable which is used to remember the first contact
            firstcontact = true;
            lander.set_spin(0);
        }

        //Rendering
        SDL_RenderClear(renderer);
        //Draw the background white
        boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 255, 255, 255, 255);

        //draw the surface of the moon
        drawObject(&moon, renderer);
#ifdef LUMAX_OUTPUT
        drawObject(&moon);
#endif

        if (!destroyed) {
            //integration
            lander.set_v(vx + dvx, vy + dvy);
            lander.update_position(dt/50);
            drawObject(&lander, renderer);
#ifdef LUMAX_OUTPUT
            drawObject(&lander);
#endif
        } else {
            if (initparts) {
                    for (int i = 0; i < nparts; ++i) {
                        parts[i].set_pos(x,y);
                        vx = frand(-10, 10);
                        vy = frand(-10, 10);
                        parts[i].set_v(vx, vy);
                    }
                initparts = false;
            }
            for (int i = 0; i < nparts; ++i) {
                parts[i].update_position(dt/50);
                drawObject(&parts[i], renderer);
#ifdef LUMAX_OUTPUT
                drawObject(&parts[i]);
#endif
            }
        }

        //draw the dimensions of the objects
        //float r_abs1 = coll.dim(&lander);
        //float r_abs2 = coll.dim(&moon);
        //ellipseRGBA(renderer, lander.x(), lander.y(), r_abs1, r_abs1, 0, 0, 0, 255);
        //ellipseRGBA(renderer, moon.x(), moon.y(), r_abs2, r_abs2, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
#ifdef LUMAX_OUTPUT
        sendPointsToLumax(lumaxHandle);
#endif

        // Timer related stuff
        oldTime = newTime;
        newTime = worldtime.getTicks();
        if (newTime > oldTime) {
            dt = newTime - oldTime; // small time between two frames in ms
        }
        if (dt == 0) dt = 1;

        //increment the frame number
        frame++;
        //apply the fps cap
        if ((cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND) ) {
            SDL_Delay((1000/FRAMES_PER_SECOND) - fps.getTicks() );
        }

        //update the window caption
        if (worldtime.getTicks() > 1000 ) {
            std::stringstream caption;
            caption << "Moonlander, aktuelle FPS: " << 1000.f*frame/worldtime.getTicks();
            SDL_SetWindowTitle(window,caption.str().c_str());
            worldtime.start();
            frame = 0;
        }
    }

#ifdef LUMAX_OUTPUT
    Lumax_StopFrame(lumaxHandle);
#endif

    //Destroy the various items
    cleanup(renderer, window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
