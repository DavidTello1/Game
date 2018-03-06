#include "SDL/include/SDL.h"
#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

#define width 1200
#define height 600

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDL2TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect ship = { 0, 0, 60, 60 };
	int speed = 10;
	bool isRunning = true;
	SDL_Event event;

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym){
				case SDLK_LEFT:
					if (ship.x - speed > -10) {
						ship.x -= speed;
					}
					break;
				case SDLK_RIGHT:
					if (ship.x + speed < 1200-50) {
						ship.x += speed;
					}
					break;
				case SDLK_UP:
					if (ship.y - speed > -10) {
						ship.y -= speed;
					}
					break;
				case SDLK_DOWN:
					if (ship.y + speed < 600-50) {
						ship.y += speed;
					}
					break;
				}
			}
		/*	if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					
					break;
				case SDLK_RIGHT:
					
					break;
				case SDLK_UP:
					if (ship.y < 0)
						ship.y = 0;
					break;
				case SDLK_DOWN:
					if (ship.y > 0)
						ship.y = 0;
					break;
				default:
					break;
				}
				break;
			}*/
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &ship);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}