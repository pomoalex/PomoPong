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


void main_game::update_object(object &_object, int height_percentage, std::string font_type, SDL_Color color)
{
	_object.size = get_font_size(1000, height_percentage, _object.text);
	_object.texture = create_text_texture(_object.text, font_type, _object.size, color);
	SDL_QueryTexture(_object.texture, NULL, NULL, &_object.rect.w, &_object.rect.h);
}


void main_game::draw_background()
{
	SDL_RenderCopy(_render_target, _background_image, NULL, NULL);
}


void main_game::draw_texture(SDL_Texture *texture, SDL_Rect texture_rect)
{
	SDL_RenderCopy(_render_target, texture, NULL, &texture_rect);
}


void main_game::draw_game_intro()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(logo.texture, logo.rect);
	draw_texture(brand.texture, brand.rect);
	draw_texture(start.texture, start.rect);
	SDL_RenderPresent(_render_target);
}


void main_game::draw_game_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(logo.texture, logo.rect);
	draw_texture(play.texture, play.rect);
	draw_texture(options.texture, options.rect);
	draw_texture(quit.texture, quit.rect);
	SDL_RenderPresent(_render_target);
}


void main_game::draw_play_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(play.texture, play.rect);
	draw_texture(one_player.texture, one_player.rect);
	draw_texture(two_players.texture, two_players.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
}


void main_game::draw_options_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(options.texture, options.rect);
	draw_texture(option1.texture, option1.rect);
	draw_texture(option2.texture, option2.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
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
		error("Can`t create text surface ! ");
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
	SDL_DestroyTexture(start.texture);
	SDL_DestroyTexture(brand.texture);
	SDL_DestroyTexture(logo.texture);
	SDL_DestroyTexture(play.texture);
	SDL_DestroyTexture(options.texture);
	SDL_DestroyTexture(quit.texture);
	SDL_DestroyTexture(play.texture);
	SDL_DestroyTexture(one_player.texture);
	SDL_DestroyTexture(two_players.texture);
	SDL_Quit();
}


void main_game::play_game()
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
	logo.text = "Pomo Pong";
	start.text = "start";
	play.text = "play";
	options.text = "options";
	quit.text = "quit";
	back.text = "back";
	one_player.text = "one player";
	two_players.text = "player vs player";
	option1.text = "option 1";
	option2.text = "option 2";
	_font_type1 = "Capture_it.ttf";
	_font_type2 = "Capture_it_2.ttf";
	_primary_color = { 0,255,0,100 };
	_secondary_color = {255 ,0,0,1};
	hover_animation_speed = 4;
}


void main_game::game_loop()
{
    game_intro();
	//_game_state = game_state::MENU;  // delete this after deletin comment game_intro
	while (_game_state != game_state::EXIT)
	{
		switch (_game_state)
		{
		case game_state::MENU :
			game_menu();
			break;
		case game_state::PLAY_MENU:
			game_play_menu();
			break;
		case game_state::OPTIONS_MENU:
			game_options_menu();
			break;
		}
	}
}


void main_game::game_intro()
{
	SDL_Color logo_color = { 0,0,0,1 };
	update_object(logo, 10, _font_type1, logo_color);
	logo.rect.x = (_window_width - logo.rect.w) / 2;
	logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;
	brand.text = "by Pomo Alex";
	
	update_object(brand, 3, _font_type1, logo_color);
	brand.rect.x = (int)((_window_width) * 0.8 - brand.rect.w);
	brand.rect.y = logo.rect.y + logo.rect.h;

	start.text = " ";					// start button appears after logo entry

	Uint8 index ;
	int counter ;
	int speed = 50;
	int delay_time = 20;
	brand.text = " ";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	for (counter = index = 254;counter >= 0;counter -= speed,index -=speed)
	{
		logo_color = { 0,index,255,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "b";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { index,0,255,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 255,0,index,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by P";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 255,index,0,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Po";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { index,255,0,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pom";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 0,255,index,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed) 
	{
		logo_color = { 0,index,255,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo A";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { index,0,255,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Al";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 255,0,index,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Ale";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 255,index,0,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Alex";
	brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { index,255,0,1 };
		logo.texture = create_text_texture(logo.text, _font_type1, logo.size, logo_color);
		brand.texture = create_text_texture(brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	SDL_Delay(1000);

	start.text = "start";					//from now on it`ll be shown

	update_object(start, 5, _font_type1, _primary_color);
	start.rect.x = (_window_width - start.rect.w) / 2;
	start.rect.y = _window_height - (_window_height - brand.rect.y - brand.rect.h + start.rect.h) / 2;
	draw_game_intro();
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
						update_object(logo, 10, _font_type1, _primary_color);
						update_object(brand, 3, _font_type1, _primary_color);
						update_object(start, 5, _font_type1, _primary_color);
						logo.rect.x = (_window_width - logo.rect.w) / 2;
						logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;
						brand.rect.x = (int)((_window_width) * 0.8 - brand.rect.w);
						brand.rect.y = logo.rect.y + logo.rect.h;
						start.rect.x = (_window_width - start.rect.w) / 2;
						start.rect.y = _window_height - (_window_height - brand.rect.y - brand.rect.h + start.rect.h) / 2;
					}
				break;
			case SDL_MOUSEMOTION:
				std::cout << input.motion.x << " here " << input.motion.y << std::endl;
				if (is_in_rect(input.motion.x, input.motion.y, start.rect))
					if (!hovering_on_start)
					{
						hovering_on_start = true;
						for (index = 1;index <= 16; index+= hover_animation_speed)
						{
							start.texture = create_text_texture(start.text, _font_type1, start.size + index, _secondary_color);
							SDL_QueryTexture(start.texture, NULL, NULL, &start.rect.w, &start.rect.h);
							start.rect.x = (_window_width - start.rect.w) / 2;
							start.rect.y = _window_height - (_window_height - brand.rect.y - brand.rect.h + start.rect.h) / 2;
							draw_game_intro();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, start.rect))
					if (hovering_on_start)
					{
						hovering_on_start = false;
						for (index = 16;index > 0; index-= hover_animation_speed)
						{
							start.texture = create_text_texture(start.text, _font_type1, start.size + index, _primary_color);
							SDL_QueryTexture(start.texture, NULL, NULL, &start.rect.w, &start.rect.h);
							start.rect.x = (_window_width - start.rect.w) / 2;
							start.rect.y = _window_height - (_window_height - brand.rect.y - brand.rect.h + start.rect.h) / 2;
							draw_game_intro();
						}
					}
				break;
			case SDL_MOUSEBUTTONUP:
				if (is_in_rect(input.motion.x, input.motion.y, start.rect))
				{
					_game_state = game_state::MENU;
					SDL_Delay(70);
				}
				break;
			}
			draw_game_intro();
		}
	}
}


void main_game::game_menu()
{
	update_object(logo, 10, _font_type1, _primary_color);
	update_object(play, 5, _font_type1, _primary_color);
	update_object(options, 5, _font_type1, _primary_color);
	update_object(quit, 5, _font_type1, _primary_color);
	logo.rect.x = (_window_width - logo.rect.w) / 2;
	logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;

	while (logo.rect.y > 0.01 * _window_height)
	{
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(logo.texture, logo.rect);
		SDL_RenderPresent(_render_target);
		logo.rect.y -= 1;
	}
	int space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height)/3;

	play.rect.x = (_window_width - play.rect.w) / 2;
	play.rect.y = logo.rect.y + logo.rect.h + (space - play.rect.h) / 2;
	
	options.rect.x = (_window_width - options.rect.w) / 2;
	options.rect.y = logo.rect.y + logo.rect.h + space + (space - options.rect.h) / 2;

	quit.rect.x = (_window_width - quit.rect.w) / 2;
	quit.rect.y = logo.rect.y + logo.rect.h + 2 * space + (space - quit.rect.h) / 2;

	SDL_Event input;
	bool hovering_on_play = false;
	bool hovering_on_options = false;
	bool hovering_on_quit = false;
	int index;
	draw_game_menu();
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
				if (is_in_rect(input.motion.x, input.motion.y, play.rect))
					if (!hovering_on_play)
					{
						hovering_on_play = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							play.texture = create_text_texture(play.text, _font_type1, play.size + index, _secondary_color);
							SDL_QueryTexture(play.texture, NULL, NULL, &play.rect.w, &play.rect.h);
							play.rect.x = (_window_width - play.rect.w) / 2;
							play.rect.y = logo.rect.y + logo.rect.h + (space - play.rect.h) / 2;
							draw_game_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, play.rect))
					if (hovering_on_play)
					{
						hovering_on_play = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							play.texture = create_text_texture(play.text, _font_type1, play.size + index, _primary_color);
							SDL_QueryTexture(play.texture, NULL, NULL, &play.rect.w, &play.rect.h);
							play.rect.x = (_window_width - play.rect.w) / 2;
							play.rect.y = logo.rect.y + logo.rect.h + (space - play.rect.h) / 2;
							draw_game_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, options.rect))
					if (!hovering_on_options)
					{
						hovering_on_options = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							options.texture = create_text_texture("options", _font_type1, options.size + index, _secondary_color);
							SDL_QueryTexture(options.texture, NULL, NULL, &options.rect.w, &options.rect.h);
							options.rect.x = (_window_width - options.rect.w) / 2;
							options.rect.y = logo.rect.y + logo.rect.h + space + (space - options.rect.h) / 2;
							draw_game_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, options.rect))
					if (hovering_on_options)
					{
						hovering_on_options = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							options.texture = create_text_texture("options", _font_type1, options.size + index, _primary_color);
							SDL_QueryTexture(options.texture, NULL, NULL, &options.rect.w, &options.rect.h);
							options.rect.x = (_window_width - options.rect.w) / 2;
							options.rect.y = logo.rect.y + logo.rect.h + space + (space - options.rect.h) / 2;
							draw_game_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, quit.rect))
					if (!hovering_on_quit)
					{
						hovering_on_quit = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							quit.texture = create_text_texture(quit.text, _font_type1, quit.size + index, _secondary_color);
							SDL_QueryTexture(quit.texture, NULL, NULL, &quit.rect.w, &quit.rect.h);
							quit.rect.x = (_window_width - quit.rect.w) / 2;
							quit.rect.y = logo.rect.y + logo.rect.h + 2 * space + (space - quit.rect.h) / 2;
							draw_game_menu();
						}
					} 
				if (!is_in_rect(input.motion.x, input.motion.y, quit.rect))
					if (hovering_on_quit)
					{
						hovering_on_quit = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							quit.texture = create_text_texture(quit.text, _font_type1, quit.size + index, _primary_color);
							SDL_QueryTexture(quit.texture, NULL, NULL, &quit.rect.w, &quit.rect.h);
							quit.rect.x = (_window_width - quit.rect.w) / 2;
							quit.rect.y = logo.rect.y + logo.rect.h + 2 * space + (space - quit.rect.h) / 2;
							draw_game_menu();
						}
					}
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, play.rect))
				{
					_game_state = game_state::PLAY_MENU;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, options.rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, quit.rect))
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
						update_object(logo, 10, _font_type1, _primary_color);
						update_object(play, 5, _font_type1, _primary_color);
						update_object(options, 5, _font_type1, _primary_color);
						update_object(quit, 5, _font_type1, _primary_color);
						logo.rect.x = (_window_width - logo.rect.w) / 2;
						logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;
						while (logo.rect.y > 0.01 * _window_height)
							logo.rect.y -= 1;
						space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height) / 3;
						play.rect.x = (_window_width - play.rect.w) / 2;
						play.rect.y = logo.rect.y + logo.rect.h + (space - play.rect.h) / 2;
						options.rect.x = (_window_width - options.rect.w) / 2;
						options.rect.y = logo.rect.y + logo.rect.h + space + (space - options.rect.h) / 2;
						quit.rect.x = (_window_width - quit.rect.w) / 2;
						quit.rect.y = logo.rect.y + logo.rect.h + 2 * space + (space - quit.rect.h) / 2;
					}
				break;
			default:
				break;
			}
			draw_game_menu();
		}
	}
}


void main_game::game_play_menu()
{
	update_object(play, 10, _font_type1, _primary_color);
	int space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height) / 3;
	play.rect.x = (_window_width - play.rect.w) / 2;
	play.rect.y = logo.rect.y + logo.rect.h + (space - play.rect.h) / 2;
	while (play.rect.y > 0.01 * _window_height)
	{
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(play.texture, play.rect);
		SDL_RenderPresent(_render_target);
		play.rect.y -= 1;
	}
	update_object(one_player, 5, _font_type1, _primary_color);
	update_object(two_players, 5, _font_type1, _primary_color);
	update_object(back, 5, _font_type1, _primary_color);
	space = (int)(_window_height - play.rect.y - play.rect.h - 0.05 * _window_height) / 3;
	one_player.rect.x = (_window_width - one_player.rect.w) / 2;
	one_player.rect.y = play.rect.y + play.rect.h + (space - one_player.rect.h) / 2;
	two_players.rect.x = (_window_width - two_players.rect.w) / 2;
	two_players.rect.y = play.rect.y + play.rect.h + space + (space - two_players.rect.h) / 2;
	back.rect.x = (_window_width - back.rect.w) / 2;
	back.rect.y = play.rect.y + play.rect.h + 2 * space + (space - back.rect.h) / 2;

	draw_play_menu();

	bool hovering_on_one_player = false;
	bool hovering_on_two_players = false;
	bool hovering_on_back = false;
	int index;
	SDL_Event input;
	while (_game_state == game_state::PLAY_MENU)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				std::cout << input.motion.x << " play_menu " << input.motion.y << std::endl;
				if (is_in_rect(input.motion.x, input.motion.y, one_player.rect))
					if (!hovering_on_one_player)
					{
						hovering_on_one_player = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							one_player.texture = create_text_texture(one_player.text, _font_type1, one_player.size + index, _secondary_color);
							SDL_QueryTexture(one_player.texture, NULL, NULL, &one_player.rect.w, &one_player.rect.h);
							one_player.rect.x = (_window_width - one_player.rect.w) / 2;
							one_player.rect.y = play.rect.y + play.rect.h + (space - one_player.rect.h) / 2;
							draw_play_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, one_player.rect))
					if (hovering_on_one_player)
					{
						hovering_on_one_player = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							one_player.texture = create_text_texture(one_player.text, _font_type1, one_player.size + index, _primary_color);
							SDL_QueryTexture(one_player.texture, NULL, NULL, &one_player.rect.w, &one_player.rect.h);
							one_player.rect.x = (_window_width - one_player.rect.w) / 2;
							one_player.rect.y = play.rect.y + play.rect.h + (space - one_player.rect.h) / 2;
							draw_play_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, two_players.rect))
					if (!hovering_on_two_players)
					{
						hovering_on_two_players = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							two_players.texture = create_text_texture(two_players.text, _font_type1, two_players.size + index, _secondary_color);
							SDL_QueryTexture(two_players.texture, NULL, NULL, &two_players.rect.w, &two_players.rect.h);
							two_players.rect.x = (_window_width - two_players.rect.w) / 2;
							two_players.rect.y = play.rect.y + play.rect.h + space + (space - two_players.rect.h) / 2;
							draw_play_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, two_players.rect))
					if (hovering_on_two_players)
					{
						hovering_on_two_players = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							two_players.texture = create_text_texture(two_players.text, _font_type1, two_players.size + index, _primary_color);
							SDL_QueryTexture(two_players.texture, NULL, NULL, &two_players.rect.w, &two_players.rect.h);
							two_players.rect.x = (_window_width - two_players.rect.w) / 2;
							two_players.rect.y = play.rect.y + play.rect.h + space + (space - two_players.rect.h) / 2;
							draw_play_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
					if (!hovering_on_back)
					{
						hovering_on_back = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							back.texture = create_text_texture(back.text, _font_type1, back.size + index, _secondary_color);
							SDL_QueryTexture(back.texture, NULL, NULL, &back.rect.w, &back.rect.h);
							back.rect.x = (_window_width - back.rect.w) / 2;
							back.rect.y = play.rect.y + play.rect.h + 2 * space + (space - back.rect.h) / 2;
							draw_play_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, back.rect))
					if (hovering_on_back)
					{
						hovering_on_back = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							back.texture = create_text_texture(back.text, _font_type1, back.size + index, _primary_color);
							SDL_QueryTexture(back.texture, NULL, NULL, &back.rect.w, &back.rect.h);
							back.rect.x = (_window_width - back.rect.w) / 2;
							back.rect.y = play.rect.y + play.rect.h + 2 * space + (space - back.rect.h) / 2;
							draw_play_menu();
						}
					}
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, one_player.rect))
				{
					_game_state = game_state::ONE_PLAYER;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, two_players.rect))
				{
					_game_state = game_state::TWO_PLAYERS;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::MENU;
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
						update_object(play, 10, _font_type1, _primary_color);
						update_object(one_player, 5, _font_type1, _primary_color);
						update_object(two_players, 5, _font_type1, _primary_color);
						update_object(back, 5, _font_type1, _primary_color);
						play.rect.x = (_window_width - play.rect.w) / 2;
						play.rect.y = (int)(_window_height * 0.8 - play.rect.h) / 2;
						while (play.rect.y > 0.01 * _window_height)
							play.rect.y -= 1;
						space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height) / 3;
						one_player.rect.x = (_window_width - one_player.rect.w) / 2;
						one_player.rect.y = play.rect.y + play.rect.h + (space - one_player.rect.h) / 2;
						two_players.rect.x = (_window_width - two_players.rect.w) / 2;
						two_players.rect.y = play.rect.y + play.rect.h + space + (space - two_players.rect.h) / 2;
						back.rect.x = (_window_width - back.rect.w) / 2;
						back.rect.y = play.rect.y + play.rect.h + 2 * space + (space - back.rect.h) / 2;
					}
				break;
			default:
				break;
			}
			draw_play_menu();
		}
	}
}


void main_game::game_options_menu()
{
	update_object(options, 10, _font_type1, _primary_color);
	int space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height) / 3;
	options.rect.x = (_window_width - options.rect.w) / 2;
	options.rect.y = logo.rect.y + logo.rect.h + (space - options.rect.h) / 2;
	while (options.rect.y > 0.01 * _window_height)
	{
		SDL_RenderClear(_render_target);
		draw_background();
		draw_texture(options.texture, options.rect);
		SDL_RenderPresent(_render_target);
		options.rect.y -= 1;
	}
	update_object(option1, 5, _font_type1, _primary_color);
	update_object(option2, 5, _font_type1, _primary_color);
	update_object(back, 5, _font_type1, _primary_color);
	space = (int)(_window_height - options.rect.y - options.rect.h - 0.05 * _window_height) / 3;
	option1.rect.x = (_window_width - option1.rect.w) / 2;
	option1.rect.y = options.rect.y + options.rect.h + (space - option1.rect.h) / 2;
	option2.rect.x = (_window_width - option2.rect.w) / 2;
	option2.rect.y = options.rect.y + options.rect.h + space + (space - option2.rect.h) / 2;
	back.rect.x = (_window_width - back.rect.w) / 2;
	back.rect.y = options.rect.y + options.rect.h + 2 * space + (space - back.rect.h) / 2;

	draw_options_menu();

	bool hovering_on_option1 = false;
	bool hovering_on_option2 = false;
	bool hovering_on_back = false;
	int index;
	SDL_Event input;
	while (_game_state == game_state::OPTIONS_MENU)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				std::cout << input.motion.x << " options_menu " << input.motion.y << std::endl;
				if (is_in_rect(input.motion.x, input.motion.y, option1.rect))
					if (!hovering_on_option1)
					{
						hovering_on_option1 = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							option1.texture = create_text_texture(option1.text, _font_type1, option1.size + index, _secondary_color);
							SDL_QueryTexture(option1.texture, NULL, NULL, &option1.rect.w, &option1.rect.h);
							option1.rect.x = (_window_width - option1.rect.w) / 2;
							option1.rect.y = options.rect.y + options.rect.h + (space - option1.rect.h) / 2;
							draw_options_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, option1.rect))
					if (hovering_on_option1)
					{
						hovering_on_option1 = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							option1.texture = create_text_texture(option1.text, _font_type1, option1.size + index, _primary_color);
							SDL_QueryTexture(option1.texture, NULL, NULL, &option1.rect.w, &option1.rect.h);
							option1.rect.x = (_window_width - option1.rect.w) / 2;
							option1.rect.y = options.rect.y + options.rect.h + (space - option1.rect.h) / 2;
							draw_options_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, option2.rect))
					if (!hovering_on_option2)
					{
						hovering_on_option2 = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							option2.texture = create_text_texture(option2.text, _font_type1, option2.size + index, _secondary_color);
							SDL_QueryTexture(option2.texture, NULL, NULL, &option2.rect.w, &option2.rect.h);
							option2.rect.x = (_window_width - option2.rect.w) / 2;
							option2.rect.y = options.rect.y + options.rect.h + space + (space - option2.rect.h) / 2;
							draw_options_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, option2.rect))
					if (hovering_on_option2)
					{
						hovering_on_option2 = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							option2.texture = create_text_texture(option2.text, _font_type1, option2.size + index, _primary_color);
							SDL_QueryTexture(option2.texture, NULL, NULL, &option2.rect.w, &option2.rect.h);
							option2.rect.x = (_window_width - option2.rect.w) / 2;
							option2.rect.y = options.rect.y + options.rect.h + space + (space - option2.rect.h) / 2;
							draw_options_menu();
						}
					}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
					if (!hovering_on_back)
					{
						hovering_on_back = true;
						for (index = 1;index <= 16; index += hover_animation_speed)
						{
							back.texture = create_text_texture(back.text, _font_type1, back.size + index, _secondary_color);
							SDL_QueryTexture(back.texture, NULL, NULL, &back.rect.w, &back.rect.h);
							back.rect.x = (_window_width - back.rect.w) / 2;
							back.rect.y = options.rect.y + options.rect.h + 2 * space + (space - back.rect.h) / 2;
							draw_options_menu();
						}
					}
				if (!is_in_rect(input.motion.x, input.motion.y, back.rect))
					if (hovering_on_back)
					{
						hovering_on_back = false;
						for (index = 16;index > 0; index -= hover_animation_speed)
						{
							back.texture = create_text_texture(back.text, _font_type1, back.size + index, _primary_color);
							SDL_QueryTexture(back.texture, NULL, NULL, &back.rect.w, &back.rect.h);
							back.rect.x = (_window_width - back.rect.w) / 2;
							back.rect.y = options.rect.y + options.rect.h + 2 * space + (space - back.rect.h) / 2;
							draw_options_menu();
						}
					}
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, option1.rect))
				{
					_game_state = game_state::OPTION1;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, option2.rect))
				{
					_game_state = game_state::OPTION2;
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::MENU;
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
						update_object(options, 10, _font_type1, _primary_color);
						update_object(option1, 5, _font_type1, _primary_color);
						update_object(option2, 5, _font_type1, _primary_color);
						update_object(back, 5, _font_type1, _primary_color);
						options.rect.x = (_window_width - options.rect.w) / 2;
						options.rect.y = (int)(_window_height * 0.8 - options.rect.h) / 2;
						while (options.rect.y > 0.01 * _window_height)
							options.rect.y -= 1;
						space = (int)(_window_height - logo.rect.y - logo.rect.h - 0.05 * _window_height) / 3;
						option1.rect.x = (_window_width - option1.rect.w) / 2;
						option1.rect.y = options.rect.y + options.rect.h + (space - option1.rect.h) / 2;
						option2.rect.x = (_window_width - option2.rect.w) / 2;
						option2.rect.y = options.rect.y + options.rect.h + space + (space - option2.rect.h) / 2;
						back.rect.x = (_window_width - back.rect.w) / 2;
						back.rect.y = options.rect.y + options.rect.h + 2 * space + (space - back.rect.h) / 2;
					}
				break;
			default:
				break;
			}
			draw_options_menu();
		}
	}
}
