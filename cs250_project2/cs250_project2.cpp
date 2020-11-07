/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <sstream>

#include <vector>
#include <set>

#include "Client.h"

using namespace std;

/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int /*argc*/, char* /*argv*/[])
{
    srand(unsigned(time(0)));

    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    const char* title = "CS250: Project #2";
    int width = 600, height = 600;
    SDL_Window* window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // GLEW: get function bindings (if possible)
    glewInit();
    if (!GLEW_VERSION_2_0)
    {
        cout << "needs OpenGL version 3.0 or better" << endl;
        return -1;
    }

    // animation loop
    try
    {
        bool done = false;
        Client* client = new Client(window);
        Uint32 ticks_last = SDL_GetTicks();
        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
				case SDL_MOUSEBUTTONDOWN:
					client->MouseDown(event);
					break;

				case SDL_MOUSEBUTTONUP:
					client->MouseUp();
					break;

				case SDL_MOUSEMOTION:
					client->MouseMotion(event);
					break;

                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                        client->resize(event.window.data1, event.window.data2);
                    break;
                }
            }
            Uint32 ticks = SDL_GetTicks();
            double dt = 0.001 * (ticks - ticks_last);
            ticks_last = ticks;
            client->draw(dt);
            SDL_GL_SwapWindow(window);
        }
        delete client;
    }

    catch (exception& e)
    {
        cout << e.what() << endl;
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}
