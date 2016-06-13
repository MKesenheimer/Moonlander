#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include "SDL2_own.h"
#include "Main.h"
#include "Timer.h"
#include "Lander.h"
#include "Moon.h"
#include "cleanup.h"
#include "Functions.h"
#include "Collision.h"

using namespace::std;

const int SCREEN_WIDTH  = 900;
const int SCREEN_HEIGHT = 800;
const int FRAMES_PER_SECOND = 20;			//Fps auf 20 festlegen

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/*
 * Log an SDL debug output with some message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The message to write, format will be msg [DEBUG]: SDL_GetError()
 */
void logSDLDebug(std::ostream &os, const std::string &msg){
	os << " [DEBUG]: " << msg << std::endl;
}
void logSDLDebug(std::ostream &os, const int msg){
	os << " [DEBUG]: " << msg << std::endl;
}

/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or NULL if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == NULL){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param w The width of the texture to draw
 * @param h The height of the texture to draw
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

// Draw a Lander to an SDL_Rederer
void drawLander(Lander *lander, SDL_Renderer *ren) {
    //draw the other points
    std::vector<float> temp;
    temp = lander->get_point(1);
    int xp_old = (int)(temp[0]);
    int yp_old = (int)(temp[1]);
    for(int i=1; i<lander->npoints(); i++) {
        temp = lander->get_point(i);
        int xp = (int)(temp[0]);
        int yp = (int)(temp[1]);
        filledEllipseRGBA(ren, xp, yp, 0, 0, 0, 0, 0, 255);
        lineRGBA(ren, xp, yp, xp_old, yp_old, 0, 0, 0, 255);
        xp_old = xp;
        yp_old = yp;
    }
}

// Draw the surface of the Moon to an SDL_Rederer
void drawMoon(Moon *moon, SDL_Renderer *ren) {

    // x and y marks the top
    int x = (int)(moon->x());
    int y = (int)(moon->y());
    
    int hsize = (int)(moon->hsize());
    int vsize = (int)(moon->vsize());
    
    ellipseRGBA(ren, x, y, hsize, vsize, 0, 0, 0, 255);
}

int main( int argc, char* args[] ) {

	int frame = 0; //Buch führen bei welchem Frame das Programm gerade ist
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

    //generate a new object
    Lander lander(SCREEN_WIDTH/8,50,-M_PI/2,2,2);
    lander.set_v(2.0,0.0);

    Moon moon(SCREEN_WIDTH/2,3*SCREEN_HEIGHT,0.0,1800,1800);
    std::cout<<"Center of moon: x="<<moon.x()<<", y="<<moon.y()<<std::endl;
    
    //collision detection
    Collision coll;
    
	//Our event structure
	SDL_Event e;
	//For tracking if we want to quit
	bool quit = false;

    //constants for integration
    //newtons constant of gravity
    float gamma = 1e3;
    //air friction
    float beta = 0;
    
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
			if (e.type == SDL_KEYDOWN){
                //do something
				if( e.key.keysym.sym == SDLK_c ) {
                    cap = !cap;
				}
                if ( e.key.keysym.sym == SDLK_DOWN) {
                    // do nothing
                }
                if ( e.key.keysym.sym == SDLK_UP) {
                    gamma = 1e3;
                    float vxf = lander.vx() + 0.03*sin(lander.phi());
                    float vyf = lander.vy() - 0.03*cos(lander.phi());
                    lander.set_v(vxf,vyf);
                    if(lander.thrust() < 10) {
                        lander.set_thrust(lander.thrust() + 3);
                    }
                }
                if ( e.key.keysym.sym == SDLK_LEFT) {
                    if(lander.phi() >= -2*M_PI/2) {
                        lander.set_angle(lander.phi() - 0.2);
                    }
                }
                if ( e.key.keysym.sym == SDLK_RIGHT) {
                    if(lander.phi() <= 2*M_PI/2) {
                        lander.set_angle(lander.phi() + 0.2);
                    }
                }
			}
			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN){
				//do something
			}
		}
        
        //if no key pressed, reduce thrust
        if(lander.thrust() > 0) {
            lander.set_thrust(lander.thrust() - 1);
        }
        
        //center of gravity
        float x0 = moon.x();
        float y0 = moon.y();
        float x = lander.x();
        float y = lander.y();
        float vx = lander.vx();
        float vy = lander.vy();
        //integration
        float r = sqrt(pow(x-x0,2)+pow(y-y0,2));
        float dvx = -gamma*(x-x0)/pow(r,3)*dt - beta*vx;
        float dvy = -gamma*(y-y0)/pow(r,3)*dt - beta*vy;
        lander.set_v(vx+dvx,vy+dvy);

        //collision detection
        if(coll.check_collision(&lander,&moon) and gamma != 0) {
            std::cout<<"Collision!"<<std::endl;
            lander.set_v(0,0);
            //gamma = 0;
        }
        
        //Rendering
		SDL_RenderClear(renderer);
		//Draw the background white
        boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 255, 255, 255, 255);
    
        //draw one lander
        drawLander(&lander, renderer);
        
        //draw the surface of the moon
        drawMoon(&moon, renderer);

        //draw the dimensions of the objects
        //float r_abs1 = coll.dim(&lander);
        //float r_abs2 = coll.dim(&moon);
        //ellipseRGBA(renderer, lander.x(), lander.y(), r_abs1, r_abs1, 0, 0, 0, 255);
        //ellipseRGBA(renderer, moon.x(), moon.y(), r_abs2, r_abs2, 0, 0, 0, 255);
        
		//update the screen and all objects
        lander.update_position(dt/50);
		SDL_RenderPresent(renderer);
        
        // Timer related stuff
        oldTime = newTime;
        newTime = worldtime.getTicks();
        if (newTime > oldTime) {
            dt = newTime - oldTime; // small time between two frames in ms
        }
        if(dt == 0) dt = 1;
        
        //increment the frame number
        frame++;
        //apply the fps cap
		if( (cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND) ) {
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.getTicks() );
		}
            
        //update the window caption
		if( worldtime.getTicks() > 1000 ) {
			std::stringstream caption;
			caption << "Moonlander, aktuelle FPS: " << 1000.f*frame/worldtime.getTicks();
            SDL_SetWindowTitle(window,caption.str().c_str());
			worldtime.start();
            frame = 0;
		}
	}
    
	//Destroy the various items
	cleanup(renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}