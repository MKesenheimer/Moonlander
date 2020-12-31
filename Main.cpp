#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include "SDLTools/Utilities.h"
#include "SDLTools/Timer.h"
#include "Lander.h"
#include "Part.h"
#include "Moon.h"
#include "Collision.h"
#include "Renderer.h"

const int SCREEN_WIDTH  = 900;
const int SCREEN_HEIGHT = 800;
const int FRAMES_PER_SECOND = 20; // Fps auf 20 festlegen

int main(int argc, char* args[]) {
    int frame = 0; // take records of frame number
    bool cap = true; // Framecap an oder ausschalten

    // Timer zum festlegen der FPS
    sdl::auxiliary::Timer fps;
    // Timer zum errechnen der weltweit vergangenen Zeit
    sdl::auxiliary::Timer worldtime;
    worldtime.start();

    // calculate the small time between two frames in ms
    int oldTime = 0;
    int newTime = 0;
    int dt = 0;

    // initialize random generator
    sdl::auxiliary::Utilities::seed(time(NULL));

    //Start up SDL and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        sdl::auxiliary::Utilities::logSDLError(std::cout, "SDL_Init");
        return -1;
    }

    // Set up our window and renderer, this time let's put our window in the center
    // of the screen
    SDL_Window *window = SDL_CreateWindow("Moonlander", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        sdl::auxiliary::Utilities::logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        sdl::auxiliary::Utilities::logSDLError(std::cout, "CreateRenderer");
        sdl::auxiliary::Utilities::cleanup(window);
        SDL_Quit();
        return -1;
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
            sdl::auxiliary::Utilities::logSDLError(std::cout, "Lumax_OpenDevice");
            sdl::auxiliary::Utilities::cleanup(window, renderer);
            SDL_Quit();
            return 1;
        }
    }

    Renderer::lumaxRenderer lumaxRenderer;

    // scaling of the laser output in respect to the SDL screen
    const float xScaling = (float)(LASERDIMENSION / (4 * SCREEN_WIDTH));
    const float yScaling = (float)(LASERDIMENSION / (4 * SCREEN_HEIGHT));
#endif

    // generate new objects
    Lander lander(SCREEN_WIDTH/8, 50, 2, 2, -M_PI/2, 0);
    lander.set_v(1.0, 0.0);
    Moon moon(SCREEN_WIDTH/2, 3*SCREEN_HEIGHT, 2*SCREEN_WIDTH, 2*SCREEN_WIDTH, 0);
    std::vector<Part> parts;
    const int nparts = 35;
    for (int i = 0; i < nparts; ++i) {
        parts.push_back(Part(0, 0, sdl::auxiliary::Utilities::frand(5, 15), sdl::auxiliary::Utilities::frand(5, 15), 
            sdl::auxiliary::Utilities::frand(0, 2*M_PI), sdl::auxiliary::Utilities::frand(-0.5, 0.5)));
        parts[i].set_v(0.0, 0.0);
    }

    // collision detection
    Collision coll;

    // game mechanics
    bool quit = false;
    bool firstcontact = true;
    float x1 = 0, y1 = 0; // position of first contact
    float x0, y0; // moon position
    float x, y; // lander position
    float vx, vy; // lander velocity
    float r; // distance moon lander
    bool destroyed = false; // if the lander is destroyed
    bool collided = false;
    bool initparts = true;

    // constants for integration
    // newtons constant of gravity
    float gamma = 1e3;
    // energy dissipation during flight (for example air drag)
    float alpha = 0.0; //1e-4
    // energy dissipation during landing
    float beta = 0.0; //1e-3
    // strength of stilt springs
    float omega = 1e-3;

    // our event structure
    SDL_Event e;
    while (!quit){
        // start the fps timer
        fps.start();

        // read any events that occured, for now we'll just quit if any event occurs
        while (SDL_PollEvent(&e)) {
            // if user closes the window
            if (e.type == SDL_QUIT)
                quit = true;
            // if user presses any key
            else if (e.type == SDL_KEYDOWN)
                if (e.key.keysym.sym == SDLK_c)
                    cap = !cap;
        }

        // handle keyboard inputs (no lags and delays!)
        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_UP]) {
            float vxf = lander.vx() + 0.03*sin(lander.phi());
            float vyf = lander.vy() - 0.03*cos(lander.phi());
            lander.set_v(vxf,vyf);
            if (lander.thrust() < 10)
                lander.set_thrust(lander.thrust() + 3);
        }
        if (!collided && keystate[SDL_SCANCODE_LEFT]) {
            // lander.set_spin(lander.spin() - 0.01);
            if (lander.phi() >= -M_PI)
                lander.set_angle(lander.phi() - 0.15);
        }
        if (!collided && keystate[SDL_SCANCODE_RIGHT]) {
            // lander.set_spin(lander.spin() + 0.01);
            if (lander.phi() <= M_PI)
                lander.set_angle(lander.phi() + 0.15);
        }

        // if no key pressed, reduce thrust
        if (lander.thrust() > 0)
            lander.set_thrust(lander.thrust() - 1);

        x0 = moon.x();
        y0 = moon.y();
        x = lander.x();
        y = lander.y();
        vx = lander.vx();
        vy = lander.vy();
        r = sqrt(pow(x - x0, 2) + pow(y - y0, 2));
        float dvx,  dvy;
        if (r <= 2000) { // atmosphere below 2000
            dvx = -gamma * (x - x0) / pow(r, 3) * dt - fabs(alpha * vx * dt);
            dvy = -gamma * (y - y0) / pow(r, 3) * dt - fabs(alpha * vy * dt);
        } else {
            dvx = -gamma * (x - x0) / pow(r, 3) * dt;
            dvy = -gamma * (y - y0) / pow(r, 3) * dt;
        }

        // collision detection
        collided = coll.check_collision(&lander,&moon);
        if (collided) {
            // remember point of first contact
            if (firstcontact) {
                x1 = x;
                y1 = y;
                firstcontact = false;

                // destroy the lander if velocity is to big
                if (sqrt(pow(vx, 2) + pow(vy, 2)) >= 1.0)
                    destroyed = true;
            }

            dvx = dvx - omega * (x - x1) * dt - fabs(beta * vx * dt);
            dvy = dvy - omega * (y - y1) * dt - fabs(beta * vy * dt);

            // stop the lander if successfully landed
            if (sqrt(pow(vx, 2) + pow(vy, 2)) <= 0.1 && lander.thrust() == 0) {
                vx = 0.0;
                vy = 0.0;
                dvx = 0.0;
                dvy = 0.0;

                // if the lander isn't upright, tilt it
                if (lander.phi() > 0.4)
                    lander.set_spin(lander.spin() + 0.001); // fall over
                if (lander.phi() < -0.4)
                    lander.set_spin(lander.spin() - 0.001); // fall over
                if (lander.phi() < 0.4 && lander.phi() > 0)
                    lander.set_spin(lander.spin() - 0.001); // bring it to an upright position
                if (lander.phi() > -0.4 && lander.phi() < 0)
                    lander.set_spin(lander.spin() + 0.001); // upright position
                if (fabs(lander.phi()) > 1.7)
                    destroyed = true; // destroy it
                if (fabs(lander.phi()) < 0.1)
                    lander.set_spin(0);
            }

            // TODO: if the lander lands on a wall

        } else {
            // reset variable which is used to remember the first contact
            firstcontact = true;
            lander.set_spin(0);
        }

        // Rendering
        SDL_RenderClear(renderer);
        // Draw the background white
        boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 255, 255, 255, 255);

        // draw the surface of the moon
        Renderer::drawObject(&moon, renderer);
#ifdef LUMAX_OUTPUT
        Renderer::drawObject(&moon, lumaxRenderer, xScaling, yScaling);
#endif

        if (!destroyed) {
            // integration
            lander.set_v(vx + dvx, vy + dvy);
            lander.update_position(dt/50);
            Renderer::drawObject(&lander, renderer);
#ifdef LUMAX_OUTPUT
            Renderer::drawObject(&lander, lumaxRenderer, xScaling, yScaling);
#endif
        } else {
            if (initparts) {
                    for (int i = 0; i < nparts; ++i) {
                        parts[i].set_pos(x, y);
                        vx = sdl::auxiliary::Utilities::frand(-10, 10);
                        vy = sdl::auxiliary::Utilities::frand(-10, 10);
                        parts[i].set_v(vx, vy);
                    }
                initparts = false;
            }
            for (int i = 0; i < nparts; ++i) {
                parts[i].update_position(dt/50);
                Renderer::drawObject(&parts[i], renderer);
#ifdef LUMAX_OUTPUT
                Renderer::drawObject(&parts[i], lumaxRenderer, xScaling, yScaling);
#endif
            }
        }

        // draw the dimensions of the objects
        //float r_abs1 = coll.dim(&lander);
        //float r_abs2 = coll.dim(&moon);
        //ellipseRGBA(renderer, lander.x(), lander.y(), r_abs1, r_abs1, 0, 0, 0, 255);
        //ellipseRGBA(renderer, moon.x(), moon.y(), r_abs2, r_abs2, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
#ifdef LUMAX_OUTPUT
        Renderer::sendPointsToLumax(lumaxHandle, lumaxRenderer);
#endif

        // Timer related stuff
        oldTime = newTime;
        newTime = worldtime.getTicks();
        if (newTime > oldTime) {
            dt = newTime - oldTime; // small time between two frames in ms
        }
        if (dt == 0) dt = 1;

        // increment the frame number
        frame++;
        // apply the fps cap
        if ((cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND) ) {
            SDL_Delay((1000/FRAMES_PER_SECOND) - fps.getTicks() );
        }

        // update the window caption
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

    // Destroy the various items
    sdl::auxiliary::Utilities::cleanup(renderer, window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
