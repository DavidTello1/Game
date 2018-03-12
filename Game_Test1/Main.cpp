#include <stdlib.h>
#include <time.h>

#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
//#include "SDL_mixer\include\SDL_mixer.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
//#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SHIP_SPEED 4
#define NUM_SHOTS 3
#define SHOT_SPEED 7
#define SHIP_WIDTH 128
#define SHIP_HEIGHT 64
#define NUM_ROCKS 7

struct projectile
{
	int x, y;
	bool alive;
};

struct meteorite 
{
	int x, y, width, height, speed, path, /*movement,*/ color, damage;
	bool alive;
};

struct star
{
	int x, y, width, height, speed, path, color;
	bool alive;
};

struct globals
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* background = nullptr;
	SDL_Texture* ship = nullptr;
	SDL_Texture* shot = nullptr;
	SDL_Texture* special = nullptr;
	SDL_Texture* asteroid_red = nullptr;
	SDL_Texture* asteroid_brown = nullptr;
	SDL_Texture* asteroid_grey = nullptr;
	int SCROLL_SPEED = 2;
	int SCORE = 0;
	int life = 100;
	int ROCKS = NUM_ROCKS;
	int background_width = 0;
	int ship_x = 0;
	int ship_y = 0;
	int last_shot = 0;
	int power = 0;
	int score_flag = 0;
	bool fire, up, down, left, right, supershot, gameover;
	//Mix_Music* music = nullptr;
	//Mix_Chunk* fx_shoot = nullptr;
	int scroll = 0;
	projectile shots[NUM_SHOTS];
	projectile super;
	meteorite rocks[NUM_ROCKS];
} g;

void Start()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	g.window = SDL_CreateWindow("Mini Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	IMG_Init(IMG_INIT_PNG);
	g.background = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/background.png"));
	g.ship = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/ship.png"));
	g.shot = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/shot.png"));
	g.special = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/special.png"));
	g.asteroid_brown = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/meteorite_brown.PNG"));
	g.asteroid_red = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/meteorite_red.PNG"));
	g.asteroid_grey = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("Assets/meteorite_grey.PNG"));
	SDL_QueryTexture(g.background, nullptr, nullptr, &g.background_width, nullptr);

	/*Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	g.music = Mix_LoadMUS("Assets/music.ogg");
	Mix_PlayMusic(g.music, -1);
	g.fx_shoot = Mix_LoadWAV("Assets/laser.wav");*/

	g.ship_x = 20;
	g.ship_y = SCREEN_HEIGHT / 2 - SHIP_HEIGHT / 2;
	g.fire = g.up = g.down = g.left = g.right = g.supershot = g.gameover = false;
	srand(time(NULL));
}

void Finish()
{
	/*Mix_FreeMusic(g.music);
	Mix_FreeChunk(g.fx_shoot);
	Mix_CloseAudio();
	Mix_Quit();*/
	SDL_DestroyTexture(g.shot);
	SDL_DestroyTexture(g.ship);
	SDL_DestroyTexture(g.background);
	SDL_DestroyTexture(g.special);
	SDL_DestroyTexture(g.asteroid_brown);
	SDL_DestroyTexture(g.asteroid_red);
	SDL_DestroyTexture(g.asteroid_grey);
	IMG_Quit();
	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_Quit();
}

bool CheckInput()
{
	bool ret = true;
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP: g.up = false;	break;
			case SDLK_DOWN:	g.down = false;	break;
			case SDLK_LEFT:	g.left = false;	break;
			case SDLK_RIGHT: g.right = false; break;
			}
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP: g.up = true; break;
			case SDLK_DOWN: g.down = true; break;
			case SDLK_LEFT: g.left = true; break;
			case SDLK_RIGHT: g.right = true; break;
			case SDLK_ESCAPE: ret = false; break;
			case SDLK_SPACE: g.fire = (event.key.repeat == 0); break;
			case SDLK_LALT: if (g.power >= 20) g.supershot = true; break;
			}
		}
		else if (event.type == SDL_QUIT)
			ret = false;

		if (g.gameover == true) {
			ret = false;
		}
	}
	return ret;
}

void MoveStuff()
{
	for (int i = 0; i < g.ROCKS; ++i) {
		if (g.rocks[i].alive == false) {
			g.rocks[i].alive = true;

			g.rocks[i].color = rand() % 3;
			g.rocks[i].speed = (rand() % 3) + 3;
			g.rocks[i].x = SCREEN_WIDTH;

			g.rocks[i].width = rand() % 3;
			if (g.rocks[i].width == 0) {
				g.rocks[i].width = 32;
				g.rocks[i].height = 32;
				g.rocks[i].damage = 5;
				g.rocks[i].y = g.rocks[i].width * (rand() % 16);
			}
			else if (g.rocks[i].width == 1) {
				g.rocks[i].width = 48;
				g.rocks[i].height = 48;
				g.rocks[i].damage = 10;
				g.rocks[i].y = g.rocks[i].width * (rand() % 11);
			}
			else if (g.rocks[i].width == 2) {
				g.rocks[i].width = 64;
				g.rocks[i].height = 64;
				g.rocks[i].damage = 20;
				g.rocks[i].y = g.rocks[i].width * (rand() % 8);
			}

			if (g.SCORE >= 100 && g.SCORE < 250 && g.score_flag == 0) {
				g.rocks[i].speed++;
				g.ROCKS++;
				g.score_flag = 1;
			}
			else if (g.SCORE >= 250 && g.SCORE < 500 && g.score_flag == 1) {
				g.rocks[i].speed += 2;
				g.ROCKS += 2;
				g.score_flag = 2;
			}
			else if (g.SCORE >= 500 && g.score_flag == 2) {
				g.rocks[i].speed += 3;
				g.ROCKS += 3;
				g.score_flag = 3;
			}
		}
	}
	for (int i = 0; i < g.ROCKS; ++i)
	{
		if (g.rocks[i].alive)
		{
			if (g.rocks[i].x > -g.rocks[i].width) {
				g.rocks[i].x -= g.rocks[i].speed;
				if (abs(g.ship_x - g.rocks[i].x) < SHIP_WIDTH && abs(g.ship_y - g.rocks[i].y) < SHIP_HEIGHT) {
					if (g.rocks[i].color == 1) {
						g.rocks[i].damage *= 2;
					}
					g.life-=g.rocks[i].damage;
					g.rocks[i].alive = false;
					if (g.life <= 0) {
						g.gameover = true;
					}
				}
			}
			else {
				g.rocks[i].alive = false;
				g.SCORE++;
			}
		}
	}
	

	//-------------------------

	if (g.up && g.ship_y - SHIP_SPEED > 0) g.ship_y -= SHIP_SPEED;
	if (g.down && g.ship_y + SHIP_SPEED < SCREEN_HEIGHT - SHIP_HEIGHT) g.ship_y += SHIP_SPEED;
	if (g.left && g.ship_x - SHIP_SPEED > 0) g.ship_x -= SHIP_SPEED;
	if (g.right && g.ship_x + SHIP_SPEED < SCREEN_WIDTH - SHIP_WIDTH)	g.ship_x += SHIP_SPEED;

	if (g.fire)
	{
		//Mix_PlayChannel(-1, g.fx_shoot, 0);
		g.fire = false;
		if (g.super.alive == false) 
		{
			if (g.last_shot > NUM_SHOTS) 
			{
				for (int i = 0; i < NUM_SHOTS; ++i) {
					if (g.shots[i].alive == false)
						g.last_shot = 0;
				}
			}
			g.shots[g.last_shot].alive = true;
			g.shots[g.last_shot].x = g.ship_x + SHIP_WIDTH / 2;
			g.shots[g.last_shot].y = g.ship_y + SHIP_HEIGHT / 2 - 16;
			g.last_shot++;
			g.super.alive = false;
		}
	}

	for (int i = 0; i < NUM_SHOTS; ++i)
	{
		if (g.shots[i].alive)
		{
			if (g.shots[i].x < SCREEN_WIDTH) {
				g.shots[i].x += SHOT_SPEED;
				for (int j = 0; j < g.ROCKS; ++j) {
					if (abs(g.shots[i].x - g.rocks[j].x) < g.rocks[j].width && abs(g.shots[i].y - g.rocks[j].y) < g.rocks[j].height) {
						if (g.rocks[j].color != 1) {
							g.rocks[j].alive = false;
							g.shots[i].alive = false;
							g.power++;
							g.SCORE += 5;
							if (g.rocks[j].color == 0) {
								if (g.rocks[j].height == 32) {
									g.life++;
								}
								else if (g.rocks[j].height == 48) {
									g.life += 3;
								}
								else if (g.rocks[j].height == 64) {
									g.life += 5;
								}
							}
						}
						else {
							g.shots[i].alive = false;
						}
					}
				}
			}
			else
				g.shots[i].alive = false;
		}
	}

	if (g.supershot)
	{
		//Mix_PlayChannel(-1, g.fx_shoot, 0);
		g.supershot = false;
		g.power -= 20;

		if (g.super.alive == false) {
			g.super.alive = true;
			g.super.x = g.ship_x + SHIP_WIDTH / 2;
			g.super.y = g.ship_y + SHIP_HEIGHT/ 2 - 24;
		}
	}
	if (g.super.alive)
	{
		if (g.super.x < SCREEN_WIDTH) {
			g.super.x += SHOT_SPEED;
			for (int j = 0; j < g.ROCKS; ++j) {
				if (abs(g.super.x - g.rocks[j].x) < 96 && abs(g.super.y - g.rocks[j].y) < 48) {
						g.rocks[j].alive = false;
						g.SCORE += 5;
						g.power++;
				}
			}
		}
		else
			g.super.alive = false;
	}
}

void Draw()
{
	SDL_Rect target;

	g.scroll += g.SCROLL_SPEED;
	if (g.scroll >= g.background_width)
		g.scroll = 0;

	target = { -g.scroll, 0, g.background_width, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background, nullptr, &target);
	target.x += g.background_width;
	SDL_RenderCopy(g.renderer, g.background, nullptr, &target);

	target = { g.ship_x, g.ship_y, SHIP_WIDTH, SHIP_HEIGHT };
	SDL_RenderCopy(g.renderer, g.ship, nullptr, &target);

	for (int i = 0; i < NUM_SHOTS; ++i)
	{
		if (g.shots[i].alive)
		{
			target = { g.shots[i].x, g.shots[i].y, 32, 32 };
			SDL_RenderCopy(g.renderer, g.shot, nullptr, &target);
		}
	}

	if (g.super.alive) 
	{
		target = { g.super.x, g.super.y, 96, 48 };
		SDL_RenderCopy(g.renderer, g.special, nullptr, &target);
	}

	for (int i = 0; i < g.ROCKS; ++i) 
	{
		if (g.rocks[i].alive)
		{
			if (g.rocks[i].color == 0)
			{
				target = { g.rocks[i].x, g.rocks[i].y, g.rocks[i].width, g.rocks[i].height };
				SDL_RenderCopy(g.renderer, g.asteroid_grey, nullptr, &target);
			}
			else if (g.rocks[i].color == 1) 
			{
				target = { g.rocks[i].x, g.rocks[i].y, g.rocks[i].width, g.rocks[i].height };
				SDL_RenderCopy(g.renderer, g.asteroid_red, nullptr, &target);
			}
			else if (g.rocks[i].color == 2)
			{
				target = { g.rocks[i].x, g.rocks[i].y, g.rocks[i].width, g.rocks[i].height };
				SDL_RenderCopy(g.renderer, g.asteroid_brown, nullptr, &target);
			}
		}
	}

	SDL_RenderPresent(g.renderer);
}

int main(int argc, char* args[])
{
	Start();

	while (CheckInput())
	{
		MoveStuff();
		Draw();
	}

	Finish();

	return(g.SCORE);
}