#include<iostream>
#include<string>

#include "main_game.h"


void error(std:: string error_text)
{
	std::cout << error_text<<std::endl;
	std::cout << "Press any key ... ";
	bool aux;
	std::cin >> aux;
	SDL_Quit();
	exit(1);
}


SDL_Texture *load_texture(std::string file_path, SDL_Renderer *render_target)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = SDL_LoadBMP(file_path.c_str());
	if (surface == NULL)
		error("Cannot load background image !");
	texture = SDL_CreateTextureFromSurface(render_target,surface);
	if (texture == NULL)
		error("Cannot load background image !");
	SDL_FreeSurface(surface);
	return texture;
}

main_game::main_game()
{
	_window = nullptr;
	_background_image = nullptr;
	_render_target = nullptr;
	_window_width = 1024;
	_window_height = 768;
	_game_state = game_state::PLAY;
}


main_game::~main_game()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyTexture(_background_image);
	SDL_DestroyRenderer(_render_target);
	SDL_Quit();
}


void main_game::play()
{
	init_game();
	game_loop();
}


void main_game::init_game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Pomo Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,_window_width,_window_height,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (_window == nullptr)
		error("Cannot create window !");
	_render_target = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	_background_image = load_texture("background.bmp", _render_target);
	if (_background_image == NULL)
		error("Cannot load background image !");
}


void main_game::game_loop()
{
	while (_game_state != game_state::EXIT)
	{
		process_input();
	}
}


void main_game::process_input()
{
	SDL_Event input;
	while (SDL_PollEvent(&input)) // while there is some input 
	{
		SDL_RenderClear(_render_target);
		SDL_RenderCopy(_render_target, _background_image, NULL, NULL);
		SDL_RenderPresent(_render_target);
		switch (input.type)
		{
		case SDL_QUIT:
			_game_state = game_state::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << input.motion.x << "   " << input.motion.y <<std:: endl;
			break;
		default:
			break;
		}
	}
}