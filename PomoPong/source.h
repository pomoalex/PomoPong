#pragma once
//Graphics andd audio libraries
#include <SDL.h>
#include<SDL_ttf.h>
#include<SFML\Audio.hpp>
//auxiliary libraries
#include<string>
#include<iostream>
#include<functional>
#include<Windows.h>
#include<random>
#include<math.h>
#include <fstream>
#include<sstream>
//classes
#include"ball.h"
#include"paddle.h"
#include"field.h"
#include"input.h"
#include "frequence_time.h"
//game reference libraries
#include"auxiliary_functions.h"
#include"game_states.h"

//SFML audio files
sf::SoundBuffer bounce_sound_buffer;
sf::SoundBuffer lost_sound_buffer;
sf::SoundBuffer click_sound_buffer;
sf::Sound bounce_sound,lost_sound,click_sound;
sf::Music music;

//draw functions 
void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect);
void draw_background();
void draw_game_intro();
void draw_playing();

//game main functions
void play_game();
void game_loop();
void game_menu();
void game_intro();
void init_game();
void game_play_menu();
void game_difficulty_menu();
void game_playing();
void game_instructions();
void game_options_menu();
void sound_options_menu();
void gameplay_options_menu();
void game_mode_menu();
void game_finish_menu();

//used objects
struct object
{
	SDL_Texture *texture;
	std::string text;
	SDL_Rect rect;
	int size;
	bool hovering;
};
object logo, brand, start;
object score1, score2, displayed_time;
int displayed_time_value,start_match_time,time_on_pause,start_pause_time,total_time_on_pause;
object _object[10];
short number_of_used_objects;
//in-game used functions
void create_text_texture(SDL_Texture* &text_texture,std::string text_to_display, std::string type_of_font, int text_size, SDL_Color text_color);
int get_font_size(int height_percentage, std::string text);
void hover_effect(object &_object, int x, int y, SDL_Rect top_object_rect, int number_of_objects, int object_number, std::function<void()>draw_function);
void update_object(object &_object, int height_percentage, std::string font_type, SDL_Color color);
void update_object_and_position(object &_object, int height_percentage, std::string font_type, SDL_Color color, position_of_object position, SDL_Rect top_object_rect, int number_of_objects, int object_number);
void update_middle_object_position(SDL_Rect top_object_rect, SDL_Rect &object_rect, int number_of_objects, int object_number);
void update_top_object_position(SDL_Rect &rect);
bool is_in_rect(int x, int y, SDL_Rect rect);
bool touched_ball(SDL_Rect rect);
bool collided_with_paddle1();
bool collided_with_paddle2();
void set_player_name();
int rate(int difficulty);
void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect);

//game global variables
int player1_score, player2_score;
bool player1_wins;
std::string player1_name;
std::string player2_name;
paddle paddle1, paddle2;
ball _ball;
field _field;
int hover_animation_speed;
int hover_animation_size;
SDL_Window* _window;
SDL_Texture* _background_image;
SDL_Color _primary_color;
SDL_Color _secondary_color;
SDL_Renderer* _render_target;
int _window_width;
int _window_height;
int _min_window_height;
int _min_window_width;
int difficulty_level;
TTF_Font *_font;
SDL_Color _text_color;
std::string _font_type1;
std::string _font_type2;
Uint32 _windowID;
SDL_Rect null_rect;
int button_size;
int title_size;
int fps;
input _input;
SDL_Event _event;