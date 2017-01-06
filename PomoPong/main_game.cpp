#include<iostream>
#include<string>

#include "main_game.h"


void error(std:: string error_text)
{
	std::cout << error_text<<std::endl;
	std::cout << "Press any key ... ";
	char c;
	std::cin >> c;
	SDL_Quit();
	exit(1);
}


bool is_in_rect(int x,int y,SDL_Rect rect)
{
	if (x < rect.x) return 0;
	if (x > rect.x + rect.w) return 0;
	if (y < rect.y) return 0;
	if (y > rect.y + rect.h) return 0;
	return 1;
}


int main_game::get_font_size(int width_percentage,int height_percentage,std::string text)
{
	int reference_width = 70;   //width of a single character when size 100
	int text_length = text.length();
	SDL_GetWindowSize(_window, &_window_width, &_window_height);
	int size = ((_window_width * width_percentage) / 100) / text_length;
	size = size * 100 / reference_width;
	int size2 = ((_window_width * height_percentage) / 100);
	size2 = size2 * 100 / reference_width;
	if (size < size2) return size;
	return size2;
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


void main_game::draw_background()
{
	SDL_RenderCopy(_render_target, _background_image, NULL, NULL);
}


void main_game::draw_texture(SDL_Texture *texture, SDL_Rect texture_rect)
{
	SDL_RenderCopy(_render_target, texture, NULL, &texture_rect);
}


SDL_Texture* main_game:: create_text_texture(std::string text_to_display, std::string type_of_font,int text_size,SDL_Color text_color)
{
	SDL_Texture* text_texture;
	SDL_Surface* text_surface;
	text_surface = nullptr;
	_font = nullptr;
	_font = TTF_OpenFont(type_of_font.c_str(),text_size);
	if (_font == NULL)
		error("Can`t load font !");
	text_surface = TTF_RenderText_Solid(_font, text_to_display.c_str(), text_color);
	if (text_surface == NULL)
		error("Can`t create text surface !");
	text_texture = nullptr;
	text_texture = SDL_CreateTextureFromSurface(_render_target, text_surface);
	if (text_texture == NULL)
		error("Can`t create text texture !");
	SDL_FreeSurface(text_surface);
	return text_texture;
}


main_game::main_game() //class constructor
{
	_window = nullptr;
	_background_image = nullptr;
	_render_target = nullptr;
	_window_width = 1000;
	_window_height = 700;
	_game_state = game_state::PLAY;
}


main_game::~main_game() //class destructor
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
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		error("Can`t initialize SDL !");
	if(TTF_Init() < 0)
		error("Can`t initialize SDL_TTF !");
	_window = SDL_CreateWindow("Pomo Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,_window_width,_window_height,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (_window == nullptr)
		error("Cannot create window !");
	_windowID = SDL_GetWindowID(_window);
	_render_target = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	_background_image = load_texture("background.bmp", _render_target);
	if (_background_image == NULL)
		error("Cannot load background image !");
	_logo_string = "Pomo Pong";
	_font_type1 = "Capture_it.ttf";
	_font_type2 = "Capture_it_2.ttf";
	_primary_color = { 0,255,0,1 };
	_secondary_color = {255 ,0,0,1};
}


void main_game::game_loop()
{
	//game_intro();
	_game_state = game_state::MENU;  // delete this after deletin comment game_intro
	while (_game_state != game_state::EXIT)
	{
		game_menu();
	}
}


void main_game::game_menu()
{
	SDL_Texture *logo, *play, *options,*quit;
	SDL_Rect logo_rect, play_rect,options_rect, quit_rect;
	int logo_size, play_size,options_size, quit_size;
	logo_size = get_font_size(80, 20, _logo_string);
	play_size = get_font_size(40, 5, "play");
	options_size = get_font_size(40, 5, "options");
	quit_size = get_font_size(40, 5, "quit");
	options_size = get_font_size(40, 20, "options");
	logo = create_text_texture(_logo_string, _font_type1, logo_size, _primary_color);
	play = create_text_texture("play", _font_type1, play_size, _primary_color);
	quit = create_text_texture("quit", _font_type1, quit_size, _primary_color);
	options = create_text_texture("options", _font_type1, options_size, _primary_color);
	SDL_QueryTexture(logo, NULL, NULL, &logo_rect.w, &logo_rect.h);
	SDL_QueryTexture(play, NULL, NULL, &play_rect.w, &play_rect.h);
	SDL_QueryTexture(quit, NULL, NULL, &quit_rect.w, &quit_rect.h);
	SDL_QueryTexture(options, NULL, NULL, &options_rect.w, &options_rect.h);
	logo_rect.x = (_window_width - logo_rect.w) / 2;
	logo_rect.y = (int)(_window_height * 0.8 - logo_rect.h) / 2;

	while (logo_rect.y > 0.01 * _window_height)
	{
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		SDL_RenderPresent(_render_target);
		logo_rect.y -= 1;
	}
	int space = (int)(_window_height - logo_rect.y - logo_rect.h - 0.05 * _window_height)/3;
	//std::cout << space << " =  ("<<_window_height<<" - "<<logo_rect.y <<" - "<<logo_rect.h<<"- -"<<0.1 * _window_height<<") / 3"<< std::endl;

	play_rect.x = (_window_width - play_rect.w) / 2;
	play_rect.y = logo_rect.y + logo_rect.h + (space - play_rect.h) / 2;
	//std::cout << play_rect.x << "  " << play_rect.y << std::endl;
	options_rect.x = (_window_width - options_rect.w) / 2;
	options_rect.y = logo_rect.y + logo_rect.h + space + (space - options_rect.h) / 2;

	quit_rect.x = (_window_width - quit_rect.w) / 2;
	quit_rect.y = logo_rect.y + logo_rect.h + 2 * space + (space - quit_rect.h) / 2;

	
	SDL_Event input;
	bool hovering_on_play = false;
	bool hovering_on_options = false;
	bool hovering_on_quit = false;
	int index;
	while (_game_state == game_state::MENU)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				std::cout << input.motion.x << "   " << input.motion.y << std::endl;
				if (is_in_rect(input.motion.x, input.motion.y, play_rect))
					if (!hovering_on_play)
					{
						hovering_on_play = true;
						for (index = 1;index <= 16; index += 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							play = create_text_texture("play", _font_type1, play_size + index, _secondary_color);
							SDL_QueryTexture(play, NULL, NULL, &play_rect.w, &play_rect.h);
							play_rect.x = (_window_width - play_rect.w) / 2;
							play_rect.y = logo_rect.y + logo_rect.h + (space - play_rect.h) / 2;
							draw_texture(play, play_rect);
							draw_texture(options, options_rect);
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, play_rect))
					if (hovering_on_play)
					{
						hovering_on_play = false;
						for (index = 16;index > 0; index -= 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							play = create_text_texture("play", _font_type1, play_size + index, _primary_color);
							SDL_QueryTexture(play, NULL, NULL, &play_rect.w, &play_rect.h);
							play_rect.x = (_window_width - play_rect.w) / 2;
							play_rect.y = logo_rect.y + logo_rect.h + (space - play_rect.h) / 2;
							draw_texture(play, play_rect);
							draw_texture(options, options_rect);
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, options_rect))
					if (!hovering_on_options)
					{
						hovering_on_options = true;
						for (index = 1;index <= 16; index += 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(play, play_rect);
							options = create_text_texture("options", _font_type1, options_size + index, _secondary_color);
							SDL_QueryTexture(options, NULL, NULL, &options_rect.w, &options_rect.h);
							options_rect.x = (_window_width - options_rect.w) / 2;
							options_rect.y = logo_rect.y + logo_rect.h + space + (space - options_rect.h) / 2;
							draw_texture(options, options_rect);
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, options_rect))
					if (hovering_on_options)
					{
						hovering_on_options = false;
						for (index = 16;index > 0; index -= 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(play, play_rect);
							options = create_text_texture("options", _font_type1, options_size + index, _primary_color);
							SDL_QueryTexture(options, NULL, NULL, &options_rect.w, &options_rect.h);
							options_rect.x = (_window_width - options_rect.w) / 2;
							options_rect.y = logo_rect.y + logo_rect.h + space + (space - options_rect.h) / 2;
							draw_texture(options, options_rect);
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, quit_rect))
					if (!hovering_on_quit)
					{
						hovering_on_quit = true;
						for (index = 1;index <= 16; index += 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(play, play_rect);
							draw_texture(options, options_rect);
							quit = create_text_texture("quit", _font_type1, quit_size + index, _secondary_color);
							SDL_QueryTexture(quit, NULL, NULL, &quit_rect.w, &quit_rect.h);
							quit_rect.x = (_window_width - quit_rect.w) / 2;
							quit_rect.y = logo_rect.y + logo_rect.h + 2 * space + (space - quit_rect.h) / 2;
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, quit_rect))
					if (hovering_on_quit)
					{
						hovering_on_quit = false;
						for (index = 16;index > 0; index -= 2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(play, play_rect);
							draw_texture(options, options_rect);
							quit = create_text_texture("quit", _font_type1, quit_size + index, _primary_color);
							SDL_QueryTexture(quit, NULL, NULL, &quit_rect.w, &quit_rect.h);
							quit_rect.x = (_window_width - quit_rect.w) / 2;
							quit_rect.y = logo_rect.y + logo_rect.h + 2 * space + (space - quit_rect.h) / 2;
							draw_texture(quit, quit_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, play_rect))
				{
					_game_state = game_state::PLAY_MENU;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, options_rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, quit_rect))
				{
					_game_state = game_state::EXIT;
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < 1000) SDL_SetWindowSize(_window, 1000, _window_height);
						if (_window_height < 700) SDL_SetWindowSize(_window, _window_width, 700);
						logo_size = get_font_size(80, 20, _logo_string);
						play_size = get_font_size(40, 5, "play");
						options_size = get_font_size(40, 5, "options");
						quit_size = get_font_size(40, 5, "quit");
						logo = create_text_texture(_logo_string, _font_type1, logo_size, _primary_color);
						play = create_text_texture("play", _font_type1, play_size, _primary_color);
						quit = create_text_texture("quit", _font_type1, quit_size, _primary_color);
						options = create_text_texture("options", _font_type1, options_size, _primary_color);
						SDL_QueryTexture(logo, NULL, NULL, &logo_rect.w, &logo_rect.h);
						SDL_QueryTexture(play, NULL, NULL, &play_rect.w, &play_rect.h);
						SDL_QueryTexture(quit, NULL, NULL, &quit_rect.w, &quit_rect.h);
						SDL_QueryTexture(options, NULL, NULL, &options_rect.w, &options_rect.h);
						logo_rect.x = (_window_width - logo_rect.w) / 2;
						logo_rect.y = (int)(_window_height * 0.8 - logo_rect.h) / 2;
						while (logo_rect.y > 0.01 * _window_height)
							logo_rect.y -= 1;
						space = (int)(_window_height - logo_rect.y - logo_rect.h - 0.05 * _window_height) / 3;
						play_rect.x = (_window_width - play_rect.w) / 2;
						play_rect.y = logo_rect.y + logo_rect.h + (space - play_rect.h) / 2;
						options_rect.x = (_window_width - options_rect.w) / 2;
						options_rect.y = logo_rect.y + logo_rect.h + space + (space - options_rect.h) / 2;
						quit_rect.x = (_window_width - quit_rect.w) / 2;
						quit_rect.y = logo_rect.y + logo_rect.h + 2 * space + (space - quit_rect.h) / 2;
					}
				break;
			default:
				break;
			}
			SDL_RenderClear(_render_target);
			draw_background();
			draw_texture(logo, logo_rect);
			draw_texture(play, play_rect);
			draw_texture(options, options_rect);
			draw_texture(quit, quit_rect);
			SDL_RenderPresent(_render_target);
		}
	}
	SDL_DestroyTexture(logo);
	SDL_DestroyTexture(play);
	SDL_DestroyTexture(quit);
}


void main_game::game_intro()
{
	SDL_RenderClear(_render_target);
	draw_background();
	SDL_Texture*logo = nullptr;
	SDL_Color logo_color = { 0,0,0,1 };
	int logo_size = get_font_size(80,20,_logo_string);
	std::string logo_string = "Pomo Pong";
	logo = create_text_texture(logo_string,_font_type1, logo_size, logo_color);
	SDL_Rect logo_rect;
	SDL_QueryTexture(logo, NULL, NULL, &logo_rect.w, &logo_rect.h);
	logo_rect.x = (_window_width - logo_rect.w) / 2;
	logo_rect.y = (int)(_window_height * 0.8 - logo_rect.h) / 2;
	SDL_RenderPresent(_render_target);
	
	SDL_Texture*brand = nullptr;
	std::string brand_string = "by Pomo Alex";
	int brand_size = get_font_size(40,20, brand_string);
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	if (brand == NULL)
		error("Can`t create start logo !");
	SDL_Rect brand_rect;
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	brand_rect.x = (int)((_window_width) * 0.8 - brand_rect.w);
	brand_rect.y = logo_rect.y + logo_rect.h;

	Uint8 index = 255;
	int counter = index;
	int speed = 50;
	int delay_time = 20;
	while (counter >= 0)
	{
		logo_color = { 0,index,255,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	index = 1;
	counter = index;
	brand_string = "b";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter <= 0)
	{
		logo_color = { index,0,255,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index += speed;
		counter += speed;
		SDL_Delay(delay_time);
	}
	index = 254;
	counter = index;
	brand_string = "by";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter >= 0)
	{
		logo_color = { 255,0,index,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	index = 1;
	counter = index;
	brand_string = "by P";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter <= 255)
	{
		logo_color = { 255,index,0,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index += speed;
		counter += speed;
		SDL_Delay(delay_time);
	}
	index = 254;
	counter = index;
	brand_string = "by Po";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter >= 0)
	{
		logo_color = { index,255,0,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	index = 1;
	counter = index;
	brand_string = "by Pom";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter <= 255)
	{
		logo_color = { 0,255,index,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index += speed;
		counter += speed;
		SDL_Delay(delay_time);
	}
	index = 254;
	counter = index;
	brand_string = "by Pomo";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter >= 0)
	{
		logo_color = { 0,index,255,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	index = 1;
	counter = index;
	brand_string = "by Pomo A";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter <= 255)
	{
		logo_color = { index,0,255,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index += speed;
		counter += speed;
		SDL_Delay(delay_time);
	}
	index = 254;
	counter = index;
	brand_string = "by Pomo Al";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter >= 0)
	{
		logo_color = { 255,0,index,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	index = 1;
	counter = index;
	brand_string = "by Pomo Ale";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter <= 255)
	{
		logo_color = { 255,index,0,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index += speed;
		counter += speed;
		SDL_Delay(delay_time);
	}
	index = 254;
	counter = index;
	brand_string = "by Pomo Alex";
	brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
	SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
	while (counter >=0)
	{
		logo_color = { index,255,0,1 };
		logo = create_text_texture(_logo_string, _font_type1, logo_size, logo_color);
		brand = create_text_texture(brand_string, _font_type1, brand_size, logo_color);
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo, logo_rect);
		draw_texture(brand, brand_rect);
		SDL_RenderPresent(_render_target);
		index -= speed;
		counter -= speed;
		SDL_Delay(delay_time);
	}
	SDL_Delay(1000);
	SDL_Rect start_button_rect;
	int start_button_size = get_font_size(20,20, "START");
	SDL_Texture* start_button = create_text_texture("Start", _font_type1, start_button_size, _primary_color);
	SDL_QueryTexture(start_button, NULL, NULL, &start_button_rect.w, &start_button_rect.h);
	start_button_rect.x = (_window_width - start_button_rect.w) / 2;
	start_button_rect.y = _window_height - (_window_height - brand_rect.y - brand_rect.h + start_button_rect.h) / 2;
	draw_texture(start_button, start_button_rect);
	SDL_RenderPresent(_render_target);
	SDL_Event input;
	bool hovering_on_start = false;
	while (_game_state != game_state::MENU && _game_state != game_state::EXIT)
	{
		int verify_w = _window_width, verify_h = _window_height;
		while (SDL_PollEvent(&input)) // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_WINDOWEVENT:
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < 1000) SDL_SetWindowSize(_window, 1000, _window_height);
						if (_window_height < 700) SDL_SetWindowSize(_window, _window_width, 700);
						logo_size = get_font_size(80,20, _logo_string);
						brand_size = get_font_size(40,20, brand_string);
						start_button_size = get_font_size(20,20, "START");
						logo = create_text_texture(_logo_string, _font_type1, logo_size, _primary_color);
						brand = create_text_texture(brand_string, _font_type1, brand_size, _primary_color);
						start_button = create_text_texture("start", _font_type1, start_button_size, _primary_color);
						SDL_QueryTexture(logo, NULL, NULL, &logo_rect.w, &logo_rect.h);
						SDL_QueryTexture(brand, NULL, NULL, &brand_rect.w, &brand_rect.h);
						SDL_QueryTexture(start_button, NULL, NULL, &start_button_rect.w, &start_button_rect.h);
						logo_rect.x = (_window_width - logo_rect.w) / 2;
						logo_rect.y = (int)(_window_height * 0.8 - logo_rect.h) / 2;
						brand_rect.x = (int)((_window_width) * 0.8 - brand_rect.w);
						brand_rect.y = logo_rect.y + logo_rect.h;
						start_button_rect.x = (_window_width - start_button_rect.w) / 2;
						start_button_rect.y = _window_height - (_window_height - brand_rect.y - brand_rect.h + start_button_rect.h) / 2;
					}
				break;
			case SDL_MOUSEMOTION:
				std::cout << input.motion.x << " here " << input.motion.y << std::endl;
				if (is_in_rect(input.motion.x, input.motion.y, start_button_rect))
					if (!hovering_on_start)
					{
						hovering_on_start = true;
						for (index = 1;index <= 16; index+=2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(brand, brand_rect);
							start_button = create_text_texture("START", _font_type1, start_button_size + index, { 255,0,0,1 });
							SDL_QueryTexture(start_button, NULL, NULL, &start_button_rect.w, &start_button_rect.h);
							start_button_rect.x = (_window_width - start_button_rect.w) / 2;
							start_button_rect.y = _window_height - (_window_height - brand_rect.y - brand_rect.h + start_button_rect.h) / 2;
							draw_texture(start_button, start_button_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, start_button_rect))
					if (hovering_on_start)
					{
						hovering_on_start = false;
						for (index = 16;index > 0; index-=2)
						{
							SDL_RenderClear(_render_target);
							draw_background();
							draw_texture(logo, logo_rect);
							draw_texture(brand, brand_rect);
							start_button = create_text_texture("START", _font_type1, start_button_size + index, _primary_color);
							SDL_QueryTexture(start_button, NULL, NULL, &start_button_rect.w, &start_button_rect.h);
							start_button_rect.x = (_window_width - start_button_rect.w) / 2;
							start_button_rect.y = _window_height - (_window_height - brand_rect.y - brand_rect.h + start_button_rect.h) / 2;
							draw_texture(start_button, start_button_rect);
							SDL_RenderPresent(_render_target);
						}
					}
				break;
			case SDL_MOUSEBUTTONUP:
				if (is_in_rect(input.motion.x, input.motion.y, start_button_rect))
				{
					_game_state = game_state::MENU;
					SDL_Delay(70);
				}
				break;
			}
			SDL_RenderClear(_render_target);
			draw_background();
			draw_texture(logo, logo_rect);
			draw_texture(brand, brand_rect);
			draw_texture(start_button, start_button_rect);
			SDL_RenderPresent(_render_target);
		}
	}
	SDL_DestroyTexture(start_button);
	SDL_DestroyTexture(brand);
	SDL_DestroyTexture(logo);
}
