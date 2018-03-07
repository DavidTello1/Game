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
	SDL_Rect shot = { ship.x + 30, ship.y + 20, 20, 10 };
	int speedx = 0, speedy = 0, speed = 0, count = 0;
	bool space = false;

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
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_SPACE:
					space = true;
					count += 1;
					speed = 30;
					break;
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
			ship.x = width;
			if (space == false) {
				shot.x = width + 30;
			}
		}
		else if (ship.x + speedx > width) {
			ship.x = 0;
			if (space == false) {
				shot.x = 30;
			}
		}
		ship.x += speedx;
		if (space == false) {
			shot.x = ship.x + 30;
		}

		if (ship.y + speedy < 0) {
			ship.y = height;
			if (space == false) {
				shot.y = height + 20;
			}
		}
		else if (ship.y + speedy > height) {
			ship.y = 0;
			if (space == false) {
				shot.y = 20;
			}
		}
		ship.y += speedy;
		if (space == false) {
			shot.y = ship.y + 20;
		}
		
		if (space == true) {
			if (shot.x + speed < width + 20) {
				shot.x += speed;
			}
			else { 
				space = false; 
			}
		}

		
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);
		for (int i = 0; i < count; ++i) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &shot);
		}
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