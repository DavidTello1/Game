#include "SDL/include/SDL.h"
#include "SDL_Image/include/SDL_image.h"
#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

#define width 1200
#define height 600

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window *window = SDL_CreateWindow("SDL2TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect ship = { 200, 300, 50, 50 };
	SDL_Rect shot = { ship.x + 30, ship.y + 20, 20, 10 };

	SDL_Surface *Background = IMG_Load("Images/background.png");
	SDL_Texture *BgTexture = SDL_CreateTextureFromSurface(renderer, Background);
	SDL_Surface *Image = IMG_Load("Images/ship.png");
	SDL_Texture *ImgTexture = SDL_CreateTextureFromSurface(renderer, Image);
	SDL_Surface *Laser = IMG_Load("Images/shot.png");
	SDL_Texture *ShotTexture = SDL_CreateTextureFromSurface(renderer, Laser);


	int speedx = 0, speedy = 0, speed = 0;
	bool left = false, right = false, up = false, down = false, space = false;

	bool isRunning = true;
	SDL_Event event;

	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					left = true;
					break;
				case SDLK_RIGHT:
					right = true;
					break;
				case SDLK_UP:
					up = true;
					break;
				case SDLK_DOWN:
					down = true;
					break;

				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_SPACE:
					space = true;
					speed = 20;
					break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					left = false;
					if (speedx < 0) {
						speedx = 0;
					}
					break;

				case SDLK_RIGHT:
					right = false;
					if (speedx > 0) {
						speedx = 0;
					}
					break;

				case SDLK_UP:
					up = false;
					if (speedy < 0) {
						speedy = 0;
					}
					break;

				case SDLK_DOWN:
					down = false;
					if (speedy > 0) {
						speedy = 0;
					}
					break;
				}
			}
		}

		if (left == true && right == true) {
			speedx = 0;
		}
		else if (left == true) {
			speedx = -5;
		}
		else if (right == true) {
			speedx = 5;
		}
		if (up== true && down == true){
			speedy = 0;
		}
		else if (up == true) {
			speedy = -5;
		}
		else if (down == true) {
			speedy = 5;
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
		ship.x += speedx;
		ship.y += speedy;
		if (space == false) {
			shot.x = ship.x + 30;
			shot.y = ship.y + 20;
		}
		else if (space == true) {
			if (shot.x + speed < width + 20) {
				shot.x += speed;
			}
			else { 
				space = false; 
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, BgTexture, NULL, NULL);
		SDL_RenderCopy(renderer, ImgTexture, NULL, &ship);
		SDL_RenderCopy(renderer, ShotTexture, NULL, &shot);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(BgTexture);
	SDL_FreeSurface(Background);
	SDL_DestroyTexture(ImgTexture);
	SDL_FreeSurface(Image);
	SDL_DestroyTexture(ShotTexture);
	SDL_FreeSurface(Laser);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}