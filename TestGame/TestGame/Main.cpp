#include "SDL/include/SDL.h"
#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

#define width 1200
#define height 600

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDL2TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect ship = { 200, 300, 50, 50 };
	int speedx = 0, speedy = 0;

	bool isRunning = true;
	SDL_Event event;

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym){
				case SDLK_LEFT:
					speedx = -5;
					break;
				case SDLK_RIGHT:
					speedx = 5;
					break;
				case SDLK_UP:
					speedy = -5;
					break;
				case SDLK_DOWN:
					speedy = 5;
					break;
				
				case SDLK_SPACE:
					;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (speedx < 0) {
						speedx = 0;	}
					break;

				case SDLK_RIGHT:
					if (speedx > 0) {
						speedx = 0;	}
					break;

				case SDLK_UP:
					if (speedy < 0) {
						speedy = 0;	}
					break;

				case SDLK_DOWN:
					if (speedy > 0) {
						speedy = 0; }
					break;
				}
			}
		}
		if (ship.x + speedx < 0 ){
			ship.x = 0;
		}
		else if (ship.x + speedx > width) {
			ship.x = width - 50;
		}
		ship.x += speedx;
		if (ship.y + speedy < 0) {
			ship.y = 0;
		}
		else if (ship.y + speedy > height) {
			ship.y = height - 50;
		}
		ship.y += speedy;

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
//
//keystate = SDL_GetKeyState(NULL);
//if (keystate[SDLK_LEFT]) {
//	rcSprite.x -= 2;
//}
//if (keystate[SDLK_RIGHT]) {
//	rcSprite.x += 2;
//}
//if (keystate[SDLK_UP]) {
//	rcSprite.y -= 2;
//}
//if (keystate[SDLK_DOWN]) {
//	rcSprite.y += 2;
//}
///* collide with edges of screen */
//if (rcSprite.x < 0) {
//	rcSprite.x = 0;
//}
//else if (rcSprite.x > SCREEN_WIDTH - SPRITE_SIZE) {
//	rcSprite.x = SCREEN_WIDTH - SPRITE_SIZE;
//}
//if (rcSprite.y < 0) {
//	rcSprite.y = 0;
//}
//else if (rcSprite.y > SCREEN_HEIGHT - SPRITE_SIZE) {
//	rcSprite.y = SCREEN_HEIGHT - SPRITE_SIZE;
//}