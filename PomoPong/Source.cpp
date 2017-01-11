#include "source.h"
#include"input.h"
#include<Windows.h>
#include<random>
#include "frequence_time.h"
#include<math.h>
#include<sstream>



bool collided_with_paddle1()
{
	if ((int)_ball.x - _ball.radius == paddle1.x + paddle1.width) 
	{
		paddle1.update_rect();
		if (_ball.y > paddle1.y && _ball.y < paddle1.y + paddle1.height) return 1;
	}
	return 0;
}


bool collided_with_paddle2()
{
	if ((int)_ball.x + _ball.radius == paddle2.x)
	{
		paddle2.update_rect();
		if (_ball.y > paddle2.y && _ball.y < paddle2.y + paddle2.height) return 1;
	}
	return 0;
}


void set_player_name()
{
	player1_name = "player 1";
	switch (_game_state)
	{
	case game_state::SINGLEPLAYER:
		switch (_game_difficulty)
		{
		case game_difficulty::EASY:
			player2_name = "easy pomo ai";
			break;
		case game_difficulty::MEDIUM:
			player2_name = "medium pomo ai";
			break;
		case game_difficulty::HARD:
			player2_name = "hard pomo ai";
			break;
		}
		break;
	case game_state::MULTIPLAYER:
		player2_name = "player 2";
		break;
	}
}


int rate(int difficulty)
{
	double completion_percentage = (100 * ((_field.x + _field.width) - _ball.x) / (_field.width)  ) / difficulty;
	if ((int)completion_percentage == 0)
		return (int)completion_percentage + 1;
	return (int)completion_percentage;
}


void error(std::string error_text)
{
	std::cout << error_text << std::endl;
	std::cout << "Press any key ... ";
	char c;
	std::cin >> c;
	SDL_Quit();
	exit(1);
}


double get_random_number(int min_bound , int max_bound)
{
	std::random_device rd;
	std::default_random_engine generator(rd()); // rd() provides a random seed
	std::uniform_real_distribution<double> distribution(min_bound, max_bound);

	return distribution(generator);
}


int get_random_sign()
{
	std::random_device rd;
	std::default_random_engine generator(rd()); // rd() provides a random seed
	std::uniform_real_distribution<double> distribution(-1, 1);
	if (distribution(generator) > 0)
		return  1;
	return -1;
}


bool is_in_rect(int x, int y, SDL_Rect rect)
{
	if (x < rect.x) return 0;
	if (x > rect.x + rect.w) return 0;
	if (y < rect.y) return 0;
	if (y > rect.y + rect.h) return 0;
	return 1;
}


int get_font_size(int height_percentage, std::string text)
{
	int reference_height = 65;   //height of a single character when size 100
	int text_length = text.length();
	int size = ((_window_height * height_percentage) / 100);
	if (size * text_length > _window_width * 0.9 )
	{
		size = (int)((0.9 * _window_width) / text_length);
	}
	size = size * 100 / reference_height;
	return size;
}


SDL_Texture *load_texture(std::string file_path, SDL_Renderer *render_target)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = SDL_LoadBMP(file_path.c_str());
	if (surface == NULL)
		error("Cannot load background image !");
	texture = SDL_CreateTextureFromSurface(render_target, surface);
	if (texture == NULL)
		error("Cannot load background image !");
	SDL_FreeSurface(surface);
	return texture;
}


void update_object(object &_object, int height_percentage, std::string font_type, SDL_Color color)
{
	_object.size = get_font_size(height_percentage, _object.text);
	create_text_texture(_object.texture,_object.text, font_type, _object.size, color);
	SDL_QueryTexture(_object.texture, NULL, NULL, &_object.rect.w, &_object.rect.h);
}


void update_object_and_position(object &_object, int height_percentage, std::string font_type, SDL_Color color, position_of_object position, SDL_Rect top_object_rect, int number_of_objects, int object_number)
{
	update_object(_object, height_percentage, font_type, color);
	switch (position)
	{
	case position_of_object::TOP:
		update_top_object_position(_object.rect);
		break;
	case position_of_object::MIDDLE:
		update_middle_object_position(top_object_rect, _object.rect, number_of_objects, object_number);
		break;
	}
}


void update_middle_object_position(SDL_Rect top_object_rect, SDL_Rect &object_rect, int number_of_objects, int object_number)
{
	int space = (int)(_window_height - top_object_rect.y - top_object_rect.h - 0.02 * _window_height) / number_of_objects;
	object_rect.x = (_window_width - object_rect.w) / 2;
	object_rect.y = top_object_rect.y + top_object_rect.h + (object_number - 1) * space + (space - object_rect.h) / 2;
}


void update_top_object_position(SDL_Rect &rect)
{
	rect.x = (_window_width - rect.w) / 2;
	rect.y = (int)(0.02 * _window_height);
}


void update_score()
{
	std::stringstream auxiliary;
	auxiliary << player1_name<<" : " << player1_score;
	score1.size = get_font_size(4, auxiliary.str());
	create_text_texture(score1.texture, auxiliary.str(), _font_type1, score1.size, _primary_color);
	auxiliary.str("");
	auxiliary << player2_name << " : " << player2_score;
	score2.size = get_font_size(4, auxiliary.str());
	create_text_texture(score2.texture, auxiliary.str(), _font_type1, score2.size, _primary_color);
	SDL_QueryTexture(score1.texture, NULL, NULL, &score1.rect.w, &score1.rect.h);
	SDL_QueryTexture(score2.texture, NULL, NULL, &score2.rect.w, &score2.rect.h);
	score1.rect.x = (int)(0.05 * _window_width);
	score1.rect.y = logo.rect.y + logo.rect.h + (int)(0.02 * _window_height);
	score2.rect.x = (int)(0.95 * _window_width - score2.rect.w);
	score2.rect.y = logo.rect.y + logo.rect.h + (int)(0.02 * _window_height);
}


void reset_field()
{
	_field.x = (_window_width - _field.width) / 2;
	_field.y = (int)(_window_height * 0.95 - _field.height);
	_field.update_rect();
}


void reset_paddle1()
{
	paddle1.x = _field.x + 2 * paddle1.width;
	paddle1.y = _field.y + (_field.height - paddle1.height) / 2;
	paddle1.reset();
}


void reset_paddle2()
{
	paddle2.x = _field.x + _field.width - 3 * paddle2.width;
	paddle2.y = _field.y + (_field.height - paddle2.height) / 2;
	paddle2.reset();
}


void reset_ball()
{
	_ball.x = _field.x + _field.width / 2;
	_ball.y = _field.y + _field.height / 2;
	_ball.radius = 10;
	_ball.reset();
}


void resize_play_field()
{
	double x_ball_to_field = _ball.x - _field.x;
	double y_ball_to_field = _ball.y - _field.y;
	double x_paddle1_to_field = paddle1.x - _field.x;
	double y_paddle1_to_field = paddle1.y - _field.y;
	double x_paddle2_to_field = paddle2.x - _field.x;
	double y_paddle2_to_field = paddle2.y - _field.y;
	reset_field();
	_ball.x = _field.x + x_ball_to_field;
	_ball.y = _field.y + y_ball_to_field;
	paddle1.x = _field.x + x_paddle1_to_field;
	paddle1.y = _field.y + y_paddle1_to_field;
	paddle2.x = _field.x + x_paddle2_to_field;
	paddle2.y = _field.y + y_paddle2_to_field;
}


void reset_play_field()
{
	update_object_and_position(logo, 7, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	player1_score = 0;
	player2_score = 0;
	update_score();
	reset_field();
	reset_paddle1();
	reset_paddle2();
	reset_ball();
	_field.update_rect();
	_play_state = play_state::FINISHED;
}


void draw_background()
{
	SDL_RenderCopy(_render_target, _background_image, NULL, NULL);
}


void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect)
{
	SDL_RenderCopy(_render_target, texture, NULL, &texture_rect);
}


void draw_game_intro()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(logo.texture, logo.rect);
	draw_texture(brand.texture, brand.rect);
	draw_texture(start.texture, start.rect);
	SDL_RenderPresent(_render_target);
}


void draw_game_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(logo.texture, logo.rect);
	draw_texture(play.texture, play.rect);
	draw_texture(options.texture, options.rect);
	draw_texture(quit.texture, quit.rect);
	SDL_RenderPresent(_render_target);
}


void draw_play_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(play.texture, play.rect);
	draw_texture(singleplayer.texture, singleplayer.rect);
	draw_texture(multiplayer.texture, multiplayer.rect);
	draw_texture(instructions.texture,instructions.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
}


void draw_instructions()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(instructions.texture, instructions.rect);
	draw_texture(instruction1.texture, instruction1.rect);
	draw_texture(instruction2.texture, instruction2.rect);
	draw_texture(instruction3.texture, instruction3.rect);
	draw_texture(instruction4.texture, instruction4.rect);
	draw_texture(instruction5.texture, instruction5.rect);
	draw_texture(instruction6.texture, instruction6.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
}


void draw_difficulty_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(difficulty.texture, difficulty.rect);
	draw_texture(easy.texture, easy.rect);
	draw_texture(medium.texture, medium.rect);
	draw_texture(hard.texture, hard.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
}


void draw_playing()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(logo.texture, logo.rect);
	draw_texture(score1.texture, score1.rect);
	draw_texture(score2.texture, score2.rect);
	_field.draw(_render_target);
	paddle1.draw(_render_target);
	paddle2.draw(_render_target);
	_ball.draw(_render_target);
	SDL_RenderPresent(_render_target);
}


void draw_options_menu()
{
	SDL_RenderClear(_render_target);
	draw_background();
	draw_texture(options.texture, options.rect);
	draw_texture(option1.texture, option1.rect);
	draw_texture(option2.texture, option2.rect);
	draw_texture(back.texture, back.rect);
	SDL_RenderPresent(_render_target);
}


void hover_effect(object &_object, SDL_Event input, SDL_Rect top_object_rect, int number_of_objects, int object_number, std::function<void()>draw_function)
{
	if (is_in_rect(input.motion.x, input.motion.y, _object.rect))
		if (!_object.hovering)
		{
			_object.hovering = true;
			for (int index = 1;index <= hover_animation_size; index += hover_animation_speed)
			{
				create_text_texture(_object.texture,_object.text, _font_type1, _object.size + index, _secondary_color);
				SDL_QueryTexture(_object.texture, NULL, NULL, &_object.rect.w, &_object.rect.h);
				update_middle_object_position(top_object_rect, _object.rect, number_of_objects, object_number);
				draw_function();	
			}
		}
	if (!is_in_rect(input.motion.x, input.motion.y, _object.rect))
		if (_object.hovering)
		{
			_object.hovering = false;
			for (int index = hover_animation_size;index > 0; index -= hover_animation_speed)
			{
				create_text_texture(_object.texture,_object.text, _font_type1, _object.size + index, _primary_color);
				SDL_QueryTexture(_object.texture, NULL, NULL, &_object.rect.w, &_object.rect.h);
				update_middle_object_position(top_object_rect, _object.rect, number_of_objects, object_number);
				draw_function();
			}
		}
}


void create_text_texture(SDL_Texture* &text_texture,std::string text_to_display, std::string type_of_font, int text_size, SDL_Color text_color)
{
	SDL_Surface* text_surface;
	text_surface = nullptr;
	_font = nullptr;
	_font = TTF_OpenFont(type_of_font.c_str(), text_size);
	if (_font == NULL)
		error("Can`t load font !");
	text_surface = TTF_RenderText_Solid(_font, text_to_display.c_str(), text_color);
	if (text_surface == NULL)
		error("Can`t create text surface ! ");
	SDL_DestroyTexture(text_texture);
	text_texture = nullptr;
	text_texture = SDL_CreateTextureFromSurface(_render_target, text_surface);
	if (text_texture == NULL)
		error("Can`t create text texture !");
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(_font);
}


void constructor() //class constructor
{
	_window = nullptr;
	_background_image = nullptr;
	_render_target = nullptr;
	_window_width = 1000;
	_window_height = 750;
	_min_window_height = _window_height;
	_min_window_width = _window_width;
	_game_state = game_state::IN_GAME;
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, 0, 0, 0);
}


void destructor() //class destructor
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
	SDL_DestroyTexture(singleplayer.texture);
	SDL_DestroyTexture(multiplayer.texture);
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, 1, 0, 0);
	SDL_Quit();
}


void play_game()
{
	init_game();
	game_loop();
}


void init_game()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0)
		error("Can`t initialize SDL !");
	if (TTF_Init() < 0)
		error("Can`t initialize SDL_TTF !");
	_window = SDL_CreateWindow("Pomo Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _window_width, _window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (_window == nullptr)
		error("Cannot create window !");
	_windowID = SDL_GetWindowID(_window);
	_render_target = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	_background_image = load_texture("background.bmp", _render_target);
	if (_background_image == NULL)
		error("Cannot load background image !");
	logo.text = "pomo Pong";
	start.text = "start";
	play.text = "play";
	options.text = "options";
	quit.text = "quit";
	back.text = "back";
	singleplayer.text = "singleplayer";
	multiplayer.text = "multiplayer";
	option1.text = "option 1";
	option2.text = "sound";
	_font_type1 = "Capture_it.ttf";
	_font_type2 = "Capture_it_2.ttf";
	difficulty.text = "difficulty";
	easy.text = "easy";
	medium.text = "medium";
	hard.text = "hard";
	instructions.text = "instructions";
	instruction1.text = "player 1 controls : w s";
	instruction2.text = "player 2 controls : up down";
	instruction3.text = "press space to start/resume game";
	instruction4.text = "press p to pause game";
	instruction5.text = "press r to reset game";
	instruction6.text = "press esc to return to menu";
	_primary_color = { 0,255,0,0 };
	_secondary_color = { 200 ,0,255,0 };
	hover_animation_speed = 6;
	hover_animation_size = 20;
	null_rect.x = null_rect.y = null_rect.w = null_rect.h = 0;
	button_size = 7;
	title_size = 15;
	SDL_SetRenderDrawColor(_render_target, 255, 255, 255, 255);
	_field.width = (int)(0.9 * _min_window_width);
	_field.height = (int)(0.7 * _min_window_height);


	bounce_sound_buffer.loadFromFile("bounce.wav");
	bounce_sound.setBuffer(bounce_sound_buffer);
	bounce_sound.setVolume(100);
	lost_sound_buffer.loadFromFile("lost.wav");
	lost_sound.setBuffer(lost_sound_buffer);
	lost_sound.setVolume(100);
	click_sound_buffer.loadFromFile("click.wav");
	click_sound.setBuffer(click_sound_buffer);
	click_sound.setVolume(100);
	music.openFromFile("game_music.ogg");
	music.setLoop(true);
	music.setVolume(20);
	music.play();
}


void game_loop()
{
	game_intro();
	//_game_state = game_state::PLAY_MENU;
	while (_game_state != game_state::EXIT)
	{
		switch (_game_state)
		{
		case game_state::MENU:
			game_menu();
			break;
		case game_state::PLAY_MENU:
			game_play_menu();
			break;
		case game_state::OPTIONS_MENU:
			game_options_menu();
			break;
		case game_state::DIFFICULTY_MENU:
			game_difficulty_menu();
			break;
		case game_state::MULTIPLAYER:
			game_playing();
			break;
		case game_state::SINGLEPLAYER:
			game_playing();
			break;
		case game_state::INSTRUCTIONS:
			game_instructions();
			break;
		}
	}
}


void game_intro()
{
	SDL_Color logo_color = { 0,0,0,1 };
	update_object(logo, title_size, _font_type1, logo_color);
	logo.rect.x = (_window_width - logo.rect.w) / 2;
	logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;
	brand.text = "by Pomo Alex";

	update_object(brand, 3, _font_type1, logo_color);
	brand.rect.x = (int)((_window_width) * 0.8 - brand.rect.w);
	brand.rect.y = logo.rect.y + logo.rect.h;

	start.text = " ";					// start button appears after logo entry

	Uint8 index;
	int counter;
	int speed = 50;
	int delay_time = 20;
	brand.text = " ";

	create_text_texture(brand.texture,brand.text, _font_type1, brand.size, logo_color);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 0,index,255,1 };
		create_text_texture(logo.texture,logo.text, _font_type1, logo.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "b";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { index,0,255,1 };
		create_text_texture(logo.texture,logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture,brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 255,0,index,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by P";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 255,index,0,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Po";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { index,255,0,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pom";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 0,255,index,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 0,index,255,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo A";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { index,0,255,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Al";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { 255,0,index,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Ale";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 1;counter <= 255;counter += speed, index += speed)
	{
		logo_color = { 255,index,0,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	brand.text = "by Pomo Alex";
	create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
	SDL_QueryTexture(brand.texture, NULL, NULL, &brand.rect.w, &brand.rect.h);
	for (counter = index = 254;counter >= 0;counter -= speed, index -= speed)
	{
		logo_color = { index,255,0,1 };
		create_text_texture(logo.texture, logo.text, _font_type1, logo.size, logo_color);
		create_text_texture(brand.texture, brand.text, _font_type1, brand.size, logo_color);
		draw_game_intro();
		SDL_Delay(delay_time);
	}
	SDL_Delay(1000);

	start.text = "start";					//from now on it`ll be shown

	update_object(start, button_size, _font_type1, _primary_color);
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
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object(logo, title_size, _font_type1, _primary_color);
						update_object(brand, 3, _font_type1, _primary_color);
						update_object(start, button_size, _font_type1, _primary_color);
						logo.rect.x = (_window_width - logo.rect.w) / 2;
						logo.rect.y = (int)(_window_height * 0.8 - logo.rect.h) / 2;
						brand.rect.x = (int)((_window_width) * 0.8 - brand.rect.w);
						brand.rect.y = logo.rect.y + logo.rect.h;
						start.rect.x = (_window_width - start.rect.w) / 2;
						start.rect.y = _window_height - (_window_height - brand.rect.y - brand.rect.h + start.rect.h) / 2;
					}
				break;
			case SDL_MOUSEMOTION:
				if (is_in_rect(input.motion.x, input.motion.y, start.rect))
					if (!hovering_on_start)
					{
						hovering_on_start = true;
						for (index = 1;index <= hover_animation_size; index += hover_animation_speed)
						{
							create_text_texture(start.texture,start.text, _font_type1, start.size + index, _secondary_color);
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
						for (index = hover_animation_size;index > hover_animation_speed; index -= hover_animation_speed)
						{
							create_text_texture(start.texture,start.text, _font_type1, start.size + index, _primary_color);
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
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			}
			draw_game_intro();
		}
	}
}


void game_menu()
{
	update_object_and_position(logo, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	update_object_and_position(play, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 1);
	update_object_and_position(options, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 2);
	update_object_and_position(quit, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 3);
	SDL_Event input;
	draw_game_menu();
	play.hovering = false;
	options.hovering = false;
	quit.hovering = false;
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
				hover_effect(play, input, logo.rect, 3, 1, draw_game_menu);
				hover_effect(options, input, logo.rect, 3, 2, draw_game_menu);
				hover_effect(quit, input, logo.rect, 3, 3, draw_game_menu);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, play.rect))
				{
					_game_state = game_state::PLAY_MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, options.rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, quit.rect))
				{
					_game_state = game_state::EXIT;
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(logo, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_object_and_position(play, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 1);
						update_object_and_position(options, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 2);
						update_object_and_position(quit, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, logo.rect, 3, 3);
					}
				break;
			default:
				break;
			}
			draw_game_menu();
		}
	}
}


void game_play_menu()
{
	update_object_and_position(play, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	update_object_and_position(singleplayer, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 1);
	update_object_and_position(multiplayer, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 2);
	update_object_and_position(instructions, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 3);
	update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 4);
	draw_play_menu();

	singleplayer.hovering = false;
	multiplayer.hovering = false;
	instructions.hovering = false;
	back.hovering = false;
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
				hover_effect(singleplayer, input, play.rect, 4, 1, draw_play_menu);
				hover_effect(multiplayer, input, play.rect, 4, 2, draw_play_menu);
				hover_effect(instructions, input, play.rect, 4, 3, draw_play_menu);
				hover_effect(back, input, play.rect, 4, 4, draw_play_menu);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, singleplayer.rect))
				{
					_game_state = game_state::DIFFICULTY_MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, multiplayer.rect))
				{
					_game_state = game_state::MULTIPLAYER;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, instructions.rect))
				{
					_game_state = game_state::INSTRUCTIONS;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(play, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_object_and_position(singleplayer, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 1);
						update_object_and_position(multiplayer, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 2);
						update_object_and_position(instructions, 5, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 3);
						update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, play.rect, 4, 4);
					}
				break;
			default:
				break;
			}
			draw_play_menu();
		}
	}
}


void game_difficulty_menu()
{
	update_object_and_position(difficulty, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	update_object_and_position(easy, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 1);
	update_object_and_position(medium, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 2);
	update_object_and_position(hard, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 3);
	update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 4);

	draw_difficulty_menu();

	easy.hovering = false;
	medium.hovering = false;
	hard.hovering = false;
	back.hovering = false;
	SDL_Event input;
	while (_game_state == game_state::DIFFICULTY_MENU)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				hover_effect(easy, input, difficulty.rect, 4, 1, draw_difficulty_menu);
				hover_effect(medium, input, difficulty.rect, 4, 2, draw_difficulty_menu);
				hover_effect(hard, input, difficulty.rect, 4, 3, draw_difficulty_menu);
				hover_effect(back, input, difficulty.rect, 4, 4, draw_difficulty_menu);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, easy.rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::EASY;
					click_sound.play();
					difficulty_level = 6;
					_ball.set_reset_speed(0.4);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, medium.rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::MEDIUM;
					click_sound.play();
					difficulty_level = 8;
					_ball.set_reset_speed(0.5);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, hard.rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::HARD;
					click_sound.play();
					difficulty_level = 10;
					_ball.set_reset_speed(0.6);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::PLAY_MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(difficulty, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_object_and_position(easy, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 1);
						update_object_and_position(medium, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 2);
						update_object_and_position(hard, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 3);
						update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, difficulty.rect, 4, 4);
					}
				break;
			default:
				break;
			}
			draw_difficulty_menu();
		}
	}
}


void game_instructions()
{
	update_object_and_position(instructions, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	update_object_and_position(instruction1, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 1);
	update_object_and_position(instruction2, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 2);
	update_object_and_position(instruction3, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 3);
	update_object_and_position(instruction4, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 4);
	update_object_and_position(instruction5, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 5);
	update_object_and_position(instruction6, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 6);
	update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 7);
	draw_instructions();
	
	back.hovering = false;
	SDL_Event input;
	while (_game_state == game_state::INSTRUCTIONS)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				hover_effect(back, input, instructions.rect, 6, 6, draw_instructions);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::PLAY_MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(instructions, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_object_and_position(instruction1, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 1);
						update_object_and_position(instruction2, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 2);
						update_object_and_position(instruction3, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 3);
						update_object_and_position(instruction4, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 4);
						update_object_and_position(instruction5, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 5);
						update_object_and_position(instruction6, 3, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 6);
						update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, instructions.rect, 7, 7);
					}
				break;
			default:
				break;
			}
			draw_instructions();
		}
	}
}


void game_playing()
{
	set_player_name();
	reset_play_field();
	input _input;
	SDL_Event event;
	frequence_time _time;
	while (_game_state == game_state::SINGLEPLAYER || _game_state == game_state::MULTIPLAYER)
	{
		_input.update();
		event = _input.get_event();
		_time.update();
		click_sound.setVolume(100);
		while (_time.accumulated_time >= _time.step_time)
		{
			_input.update();
			event = _input.get_event();
			if (_play_state != play_state::PLAYING)
				if (_play_state != play_state::PAUSED)
				{
					reset_ball();
					if (_input.is_key_tapped(SDL_SCANCODE_SPACE))
					{
						_play_state = play_state::PLAYING;
						_ball.x_speed = _ball.speed * get_random_sign();
						_ball.y_speed = 0;
					}
				}
			if (_play_state == play_state::PLAYING)
			{
				_ball.x += _ball.x_speed;
				_ball.y += _ball.y_speed;
				double proggressive_speed = 0.02;
				if ((int)_ball.y - _ball.radius == _field.y || (int)_ball.y + _ball.radius == _field.y + _field.height) //wall collision
				{
					_ball.y_speed = -_ball.y_speed;
					bounce_sound.play();
				}
				if (!is_in_rect((int)_ball.x, (int)_ball.y, _field.rect))
				{
					_play_state = play_state::FINISHED;
					if (_ball.x > _field.x + _field.height / 2) player1_score++;
					else player2_score++;
					update_score();
					lost_sound.play();
				}
				if (collided_with_paddle1())
				{
					double paddle_intersection = (paddle1.y + (paddle1.height / 2)) - (_ball.y);
					paddle_intersection /= (paddle1.height / 2); // number between -1 and 1
					double bounce_angle = paddle_intersection * _ball.max_bounce_angle;
					_ball.x_speed = _ball.speed * cos(bounce_angle);
					_ball.y_speed = _ball.speed * -sin(bounce_angle);
					_ball.speed += proggressive_speed;
					bounce_sound.play();
				}
				if (collided_with_paddle2())
				{
					double paddle_intersection = (paddle2.y + (paddle2.height / 2)) - (_ball.y);
					paddle_intersection = (paddle2.height / 2);
					double bounce_angle = paddle_intersection * _ball.max_bounce_angle;
					_ball.x_speed = _ball.speed * cos(bounce_angle);
					_ball.y_speed = _ball.speed * -sin(bounce_angle);
					_ball.speed += proggressive_speed;
					bounce_sound.play();
				}
				if (_game_state == game_state::SINGLEPLAYER)
				{
					if (_game_difficulty == game_difficulty::EASY)
					{
						if (_ball.x < _field.x + 3 * _field.width / 4 && (int)_ball.x % 5 == 0)
						{
							if (_ball.y < paddle1.y + paddle1.height / 2)
							{
								if (paddle1.y - paddle1.speed < _field.y)
									paddle1.y = _field.y;
								else paddle1.y -= paddle1.speed;
							}
							if (_ball.y > paddle1.y + paddle1.height / 2)
							{
								if (paddle1.y + paddle1.height + paddle1.speed > _field.y + _field.height)
									paddle1.y = _field.y + _field.height - paddle1.height;
								else paddle1.y += paddle1.speed;
							}
						}
					}
					if (_ball.x > _field.x && (int)_ball.x % rate(difficulty_level) == 0)
					{
						if (_ball.y < paddle2.y + paddle2.height / 2)
						{
							if (paddle2.y - paddle2.speed < _field.y)
								paddle2.y = _field.y;
							else paddle2.y -= paddle2.speed;
						}
						if (_ball.y > paddle2.y + paddle2.height / 2)
						{
							if (paddle2.y + paddle2.height + paddle2.speed > _field.y + _field.height)
								paddle2.y = _field.y + _field.height - paddle2.height;
							else paddle2.y += paddle2.speed;
						}
					}
				}
			}
			if (_play_state != play_state::PAUSED)
			{
				if (_input.is_key_tapped(SDL_SCANCODE_W))
				{
					if (paddle1.y - paddle1.speed < _field.y)
						paddle1.y = _field.y;
					else paddle1.y -= paddle1.speed;
				}
				if (_input.is_key_tapped(SDL_SCANCODE_S))
				{
					if (paddle1.y + paddle1.height + paddle1.speed > _field.y + _field.height)
						paddle1.y = _field.y + _field.height - paddle1.height;
					else paddle1.y += paddle1.speed;
				}
				if (_game_state == game_state::MULTIPLAYER)
				{
					if (_input.is_key_tapped(SDL_SCANCODE_UP))
					{
						if (paddle2.y - paddle2.speed < _field.y)
							paddle2.y = _field.y;
						else paddle2.y -= paddle2.speed;
					}
					if (_input.is_key_tapped(SDL_SCANCODE_DOWN))
					{
						if (paddle2.y + paddle2.height + paddle2.speed > _field.y + _field.height)
							paddle2.y = _field.y + _field.height - paddle2.height;
						else paddle2.y += paddle2.speed;
					}
				}
			}
			switch (event.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_WINDOWEVENT: // resize
				if (event.window.windowID == _windowID)
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(logo, button_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_score();
						resize_play_field();
					}
				break;
			}

			if (_input.is_key_tapped(SDL_SCANCODE_ESCAPE))
			{
				_play_state = play_state::FINISHED;
				_game_state = game_state::MENU;
				click_sound.play();
			}
			if (_play_state == play_state::PLAYING)
			{
				if (_input.is_key_tapped(SDL_SCANCODE_P))
				{
					_play_state = play_state::PAUSED;
					click_sound.play();
					click_sound.setVolume(0);
				}
			}
			if (_play_state == play_state::PAUSED)
				if (_input.is_key_tapped(SDL_SCANCODE_SPACE))
				{
					_play_state = play_state::PLAYING;
					click_sound.play();
				}
			if (_play_state == play_state::FINISHED)
				if (_input.is_key_tapped(SDL_SCANCODE_R))
				{
					reset_play_field();
					click_sound.play();
					click_sound.setVolume(0);
				}
			_time.accumulated_time -= _time.step_time;
		}
		draw_playing();
	}

}


void game_options_menu()
{
	update_object_and_position(options, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
	update_object_and_position(option1, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 1);
	update_object_and_position(option2, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 2);
	update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 3);

	draw_options_menu();
	option1.hovering = false;
	option2.hovering = false;
	back.hovering = false;
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
				hover_effect(option1, input, options.rect, 3, 1, draw_options_menu);
				hover_effect(option2, input, options.rect, 3, 2, draw_options_menu);
				hover_effect(back, input, options.rect, 3, 3, draw_options_menu);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, option1.rect))
				{
					_game_state = game_state::OPTION1;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, option2.rect))
				{
					_game_state = game_state::OPTION2;
					click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, back.rect))
				{
					_game_state = game_state::MENU;
					click_sound.play();
					SDL_Delay(70);
				}
				break;
			case SDL_WINDOWEVENT: // resize 
				if (input.window.windowID == _windowID)
					if (input.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(_window, &_window_width, &_window_height);
						if (_window_width < _min_window_width) SDL_SetWindowSize(_window, _min_window_width, _window_height);
						if (_window_height < _min_window_height) SDL_SetWindowSize(_window, _window_width, _min_window_height);
						update_object_and_position(options, title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
						update_object_and_position(option1, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 1);
						update_object_and_position(option2, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 2);
						update_object_and_position(back, button_size, _font_type1, _primary_color, position_of_object::MIDDLE, options.rect, 3, 3);
					}
				break;
			default:
				break;
			}
			draw_options_menu();
		}
	}
}

#undef main
int main(int argc,char*argv[])
{
	constructor();
	play_game();
	destructor();
	return 0;
}