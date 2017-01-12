#include "source.h"

bool is_in_rect(int x, int y, SDL_Rect rect)
{
	if (x < rect.x) return 0;
	if (x > rect.x + rect.w) return 0;
	if (y < rect.y) return 0;
	if (y > rect.y + rect.h) return 0;
	return 1;
}


bool touched_ball(SDL_Rect rect)
{
	for (int line = (int)_ball.x - _ball.radius;line <= (int)_ball.x + (int)_ball.radius; line++)
		for (int column = (int)_ball.y - _ball.radius;column <= (int)_ball.y + _ball.radius;column++)
			if ((line - (int)_ball.x) * (line - (int)_ball.x) + (column - (int)_ball.y) * (column - (int)_ball.y) <= _ball.radius * _ball.radius)
				if (is_in_rect(line, column, rect)) return true;
	return false;
}


bool collided_with_paddle1()
{
	paddle1.update_rect();
	if (touched_ball(paddle1.rect))
		return true;
	return false;
}


bool collided_with_paddle2()
{
	paddle2.update_rect();
	if (touched_ball(paddle2.rect))
		return true;
	return false;
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
			player2_name = "easy ai";
			break;
		case game_difficulty::MEDIUM:
			player2_name = "medium ai";
			break;
		case game_difficulty::HARD:
			player2_name = "hard ai";
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


int get_font_size(int height_percentage, std::string text)
{
	int reference_height = 65;   //height of a single character when size 100
	int text_length = text.length();
	int size = ((_window_height * height_percentage) / 100);
	if (size * text_length >(int)( _window_width * 0.95))
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


void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect)
{
	SDL_RenderCopy(_render_target, texture, NULL, &texture_rect);
}


void draw_background()
{
	SDL_RenderCopy(_render_target, _background_image, NULL, NULL);
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


void update_used_objects(int title_size, int button_size)
{
	if (number_of_used_objects > 0)
	{
		update_object_and_position(_object[0], title_size, _font_type1, _primary_color, position_of_object::TOP, null_rect, 0, 0);
		for (int index = 1; index < number_of_used_objects; index++)
		{
			update_object_and_position(_object[index], button_size, _font_type1, _primary_color, position_of_object::MIDDLE, _object[0].rect, number_of_used_objects - 1, index);
			_object[index].hovering = false;
		}
	}
}


void draw_used_objects()
{
	if (number_of_used_objects > 0)
	{
		SDL_RenderClear(_render_target);
		draw_background();
		for (int index = 0;index < number_of_used_objects;index++)
			draw_texture(_object[index].texture, _object[index].rect);
		SDL_RenderPresent(_render_target);
	}
}


void destroy_used_objects()
{
	for (char index = 0;index < number_of_used_objects;index++)
	{
		SDL_DestroyTexture(_object[index].texture);
		_object[index].text = "";
	}
}


void used_objects_hover_effect(SDL_Event input)
{
	if (number_of_used_objects > 1)
		for(char index = 1;index <number_of_used_objects;index++)
			hover_effect(_object[index], input, _object[0].rect, number_of_used_objects - 1, index, draw_used_objects);
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
	score1.rect.y = logo.rect.y + logo.rect.h + (int)(0.01 * _window_height);
	score2.rect.x = (int)(0.95 * _window_width - score2.rect.w);
	score2.rect.y = logo.rect.y + logo.rect.h + (int)(0.01 * _window_height);
}


void reset_field()
{
	double new_space = _window_height - (score1.rect.y + score1.rect.h);
	_field.y = score1.rect.y + score1.rect.h + (int)(0.05 * new_space);
	_field.height = (int)(0.9 * new_space);
	_field.width = (int)(1.8 * _field.height);
	_field.x = (_window_width - _field.width) / 2;
	_field.update_rect();
}


void reset_paddle1()
{
	paddle1.x = _field.x + _field.width * 0.02;
	paddle1.y = _field.y + (_field.height - paddle1.height) / 2;
	paddle1.reset();
}


void reset_paddle2()
{
	paddle2.x = _field.x - paddle1.width + _field.width * 0.98;
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
	SDL_DestroyRenderer(_render_target);
	SDL_DestroyTexture(_background_image);
	SDL_DestroyTexture(logo.texture);
	SDL_DestroyTexture(brand.texture);
	SDL_DestroyTexture(start.texture);
	SDL_DestroyTexture(score1.texture);
	SDL_DestroyTexture(score2.texture);
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, 1, 0, 0);
	TTF_Quit();
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
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	_window_height = _min_window_height = (int)(0.85 * mode.h);
	_window_width = _min_window_width = (int)(_window_height * 1.4);
	fps = mode.refresh_rate;
	_window = SDL_CreateWindow("Pomo Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _window_width, _window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (_window == nullptr)
		error("Cannot create window !");
	_windowID = SDL_GetWindowID(_window);
	_render_target = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	_background_image = load_texture("background.bmp", _render_target);
	logo.text = "pomo Pong";
	start.text = "start";
	_font_type1 = "Capture_it.ttf";
	_font_type2 = "Capture_it_2.ttf";
	_assist_state = assist_state::OFF;
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
	//_game_state = game_state::MENU;
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
		case game_state::SOUND_OPTIONS:
			sound_options_menu();
			break;
		case game_state::GAMEPLAY_OPTIONS:
			gameplay_options_menu();
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
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				break;
			}
			draw_game_intro();
		}
	}
	SDL_DestroyTexture(logo.texture);
	SDL_DestroyTexture(brand.texture);
	SDL_DestroyTexture(start.texture);
}


void game_menu()
{
	_object[0].text = "pomo pong";
	_object[1].text = "play";
	_object[2].text = "options";
	_object[3].text = "quit";
	number_of_used_objects = 4;
	update_used_objects(title_size,button_size);
	draw_used_objects();
	SDL_Event input;
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
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					_game_state = game_state::PLAY_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[3].rect))
				{
					_game_state = game_state::EXIT;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}


void game_play_menu()
{
	_object[0].text = "play";
	_object[1].text = "singleplayer";
	_object[2].text = "multiplayer";
	_object[3].text = "instructions";
	_object[4].text = "back";
	number_of_used_objects = 5;
	update_used_objects(title_size, button_size);
	draw_used_objects();
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
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					_game_state = game_state::DIFFICULTY_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					_game_state = game_state::MULTIPLAYER;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[3].rect))
				{
					_game_state = game_state::INSTRUCTIONS;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[4].rect))
				{
					_game_state = game_state::MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}


void game_difficulty_menu()
{
	_object[0].text = "difficulty";
	_object[1].text = "easy";
	_object[2].text = "medium";
	_object[3].text = "hard";
	_object[4].text = "back";
	number_of_used_objects = 5;
	update_used_objects(title_size, button_size);
	draw_used_objects();
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
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::EASY;
					if (_effects_state == effects_state::ON) click_sound.play();
					difficulty_level = 6;
					_ball.set_reset_speed(0.4);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::MEDIUM;
					if (_effects_state == effects_state::ON) click_sound.play();
					difficulty_level = 9;
					_ball.set_reset_speed(0.5);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[3].rect))
				{
					_game_state = game_state::SINGLEPLAYER;
					_game_difficulty = game_difficulty::HARD;
					if (_effects_state == effects_state::ON) click_sound.play();
					difficulty_level = 12;
					_ball.set_reset_speed(0.6);
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[4].rect))
				{
					_game_state = game_state::PLAY_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}


void game_instructions()
{
	_object[0].text = "instructions";
	_object[1].text = "player 1 controls : w s";
	_object[2].text = "player 2 controls : up down";
	_object[3].text = "press space to start/resume game";
	_object[4].text = "press p to pause game";
	_object[5].text = "press r to reset game";
	_object[6].text = "press esc to return to menu";
	_object[7].text = "back";
	number_of_used_objects = 8;
	update_used_objects(title_size, 4);
	draw_used_objects();
	_object[7].hovering = false;
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
				hover_effect(_object[7], input, _object[0].rect, 6, 6, draw_used_objects);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[7].rect))
				{
					_game_state = game_state::PLAY_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, 4);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
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
					if (_effects_state == effects_state::ON) bounce_sound.play();
				}
				
				if (!is_in_rect((int)_ball.x, (int)_ball.y, _field.rect))
				{
					_play_state = play_state::FINISHED;
					if (_ball.x > _field.x + _field.height / 2) player1_score++;
					else player2_score++;
					update_score();
					if (_effects_state == effects_state::ON) lost_sound.play();
				}
				if (collided_with_paddle1())
				{
					double paddle_intersection = (paddle1.y + (paddle1.height / 2)) - (_ball.y);
					paddle_intersection /= (paddle1.height / 2); // number between -1 and 1
					double bounce_angle = paddle_intersection * _ball.max_bounce_angle;
					_ball.x_speed = _ball.speed * cos(bounce_angle);
					_ball.y_speed = _ball.speed * -sin(bounce_angle);
					_ball.speed += proggressive_speed;
					if (_effects_state == effects_state::ON) bounce_sound.play();
				}
				if (collided_with_paddle2())
				{
					double paddle_intersection = (paddle2.y + (paddle2.height / 2)) - (_ball.y);
					paddle_intersection = (paddle2.height / 2);
					double bounce_angle = paddle_intersection * _ball.max_bounce_angle;
					_ball.x_speed = _ball.speed * cos(bounce_angle);
					_ball.y_speed = _ball.speed * -sin(bounce_angle);
					_ball.speed += proggressive_speed;
					if (_effects_state == effects_state::ON) bounce_sound.play();
				}
				if (_game_state == game_state::SINGLEPLAYER)
				{
					if (_assist_state == assist_state::ON)
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
				if (_input.is_key_tapped(SDL_SCANCODE_P))
				{
					_play_state = play_state::PAUSED;
					if (_effects_state == effects_state::ON) click_sound.play();
					click_sound.setVolume(0);
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
			if (_play_state == play_state::PAUSED)
				if (_input.is_key_tapped(SDL_SCANCODE_SPACE))
				{
					_play_state = play_state::PLAYING;
					if (_effects_state == effects_state::ON) click_sound.play();
				}
			if (_play_state == play_state::FINISHED)
			{
				if (_input.is_key_tapped(SDL_SCANCODE_R))
				{
					reset_play_field();
					if (_effects_state == effects_state::ON) click_sound.play();
					click_sound.setVolume(0);
				}
				if (_input.is_key_tapped(SDL_SCANCODE_ESCAPE))
				{
					_play_state = play_state::FINISHED;
					_game_state = game_state::MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
				}
			}
			_time.accumulated_time -= _time.step_time;
		}
		draw_playing();
		if (fps != 0)
			if ((1000 / fps) > SDL_GetTicks() - _time.current_time)
				SDL_Delay((1000 / fps) - SDL_GetTicks() + _time.current_time);
	}

}


void game_options_menu()
{
	_object[0].text = "options";
	_object[1].text = "gameplay";
	_object[2].text = "sound";
	_object[3].text = "back";
	number_of_used_objects = 4;
	update_used_objects(title_size, button_size);
	draw_used_objects();
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
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					_game_state = game_state::GAMEPLAY_OPTIONS;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					_game_state = game_state::SOUND_OPTIONS;
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[3].rect))
				{
					_game_state = game_state::MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}


void sound_options_menu()
{
	_object[0].text = "sound";
	_object[1].text = "music on";
	_object[2].text = "sound effects on";
	_object[3].text = "back";
	number_of_used_objects = 4;
	update_used_objects(title_size, button_size);
	draw_used_objects();
	SDL_Event input;
	while (_game_state == game_state::SOUND_OPTIONS)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					switch (_music_state)
					{
					case music_state::ON:
						music.stop();
						_object[1].text = "music off";
						_music_state = music_state::OFF;
						break;
					case music_state::OFF:
						music.play();
						_object[1].text = "music on";
						_music_state = music_state::ON;
						break;
					}
					update_object_and_position(_object[1], button_size, _font_type1, _primary_color, position_of_object::MIDDLE, _object[0].rect, 3, 1);
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					switch (_effects_state)
					{
					case effects_state::ON:
						_object[2].text = "sound effects off";
						_effects_state = effects_state::OFF;
						break;
					case effects_state::OFF:
						_object[2].text = "sound effects on";
						_effects_state = effects_state::ON;
						break;
					}
					update_object_and_position(_object[2], button_size, _font_type1, _primary_color, position_of_object::MIDDLE, _object[0].rect, 3, 2);
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[3].rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}


void gameplay_options_menu()
{
	_object[0].text = "gameplay";
	_object[1].text = "assist off";
	_object[2].text = "back";
	number_of_used_objects = 3;
	update_used_objects(title_size, button_size);
	draw_used_objects();
	SDL_Event input;
	while (_game_state == game_state::GAMEPLAY_OPTIONS)
	{
		while (SDL_PollEvent(&input))  // while there is some input 
		{
			switch (input.type)
			{
			case SDL_QUIT:
				_game_state = game_state::EXIT;
				break;
			case SDL_MOUSEMOTION: // hovering
				used_objects_hover_effect(input);
				break;
			case SDL_MOUSEBUTTONUP: // pressing button
				if (is_in_rect(input.motion.x, input.motion.y, _object[1].rect))
				{
					switch (_assist_state)
					{
					case assist_state::ON:
						_object[1].text = "assist off";
						_assist_state = assist_state::OFF;
						break;
					case assist_state::OFF:
						_object[1].text = "assist on";
						_assist_state = assist_state::ON;
						break;
					}
					update_object_and_position(_object[1], button_size, _font_type1, _primary_color, position_of_object::MIDDLE, _object[0].rect, 2, 1);
					if (_effects_state == effects_state::ON) click_sound.play();
					SDL_Delay(70);
				}
				if (is_in_rect(input.motion.x, input.motion.y, _object[2].rect))
				{
					_game_state = game_state::OPTIONS_MENU;
					if (_effects_state == effects_state::ON) click_sound.play();
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
						update_used_objects(title_size, button_size);
					}
				break;
			default:
				break;
			}
			draw_used_objects();
		}
	}
	//destroy_used_objects();
	number_of_used_objects = 0;
}

#undef main
int main(int argc,char*argv[])
{
	constructor();
	play_game();
	destructor();
	return 0;
}