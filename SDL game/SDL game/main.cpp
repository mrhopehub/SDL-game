/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "assert.h"
#include "main.h"
#include "LTexture.h"
#include "Point.h"
//Screen dimension constants
const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 600;

#define NUM_OF_SUBRECT 9


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
SDL_Rect gSpriteClips[ NUM_OF_SUBRECT ];
LTexture gSpriteSheetTexture;

//每个subrect都对应texture的一个rect
int subrect_texture_index[NUM_OF_SUBRECT] = {1,6,2,7,4,5,3,0,8};
//初始化要求subrect_texture_index[::space_point.get_y() * 3 + ::space_point.get_x()] = 8;
Point space_point(2,2);
Point selected_point(0,0);

//获取mousedown的subrect的中心点
//e：鼠标事件
//center：subrect中心点
void get_subrect_center_point(SDL_Event* e, Point *center)
{
	assert(e->type == SDL_MOUSEBUTTONUP);
	int x, y;
	SDL_GetMouseState( &x, &y );
	center->set_x(x / 250);
	center->set_y(y / 200);
}
int distance(Point *point1, Point *point2)
{
	int x_dis = point1->get_x() - point2->get_x();
	int y_dis = point1->get_y() - point2->get_y();
	return x_dis * x_dis + y_dis * y_dis; 
}



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			gSpriteSheetTexture.setRenderer(gRenderer);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "11_clip_rendering_and_sprite_sheets/game.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		//初始化矩形
		for(int i = 0; i <= 2; i++)
		for(int j = 0; j <= 2; j++)
		{
			gSpriteClips[i * 3 + j].x = j * 250;
			gSpriteClips[i * 3 + j].y = i * 200;
			gSpriteClips[i * 3 + j].w = 250;
			gSpriteClips[i * 3 + j].h = 200;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if( e.type == SDL_MOUSEBUTTONUP)
					{
						::get_subrect_center_point(&e,&selected_point);
						//通过距离判断两个subrect的相邻关系
						if(::distance(&::selected_point, &::space_point) == 1)
						{
							//更新subrect_texture_index
							subrect_texture_index[::space_point.get_y() * 3 + ::space_point.get_x()] = subrect_texture_index[::selected_point.get_y() * 3 + ::selected_point.get_x()];
							::subrect_texture_index[::selected_point.get_y() * 3 + ::selected_point.get_x()] = 8;
							//更新space_point
							::space_point.set_x(::selected_point.get_x());
							::space_point.set_y(::selected_point.get_y());
						}
					}
					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );
					for(int i = 0; i <= 2; i++)
					for(int j = 0; j <= 2; j++)
					{
						int tmp = subrect_texture_index[j * 3 + i];
						gSpriteSheetTexture.render(i * 250, j*200, &gSpriteClips[tmp]);
					}
					SDL_RenderPresent( gRenderer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}