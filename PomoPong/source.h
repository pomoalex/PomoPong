#pragma once
#include <SDL.h>
#include<SDL_ttf.h>
#include<functional>
#include<string>
#include<iostream>
#include"ball.h"
#include"paddle.h"
#include"field.h"
#include<SFML\Audio.hpp>


sf::SoundBuffer bounce_sound_buffer;
sf::SoundBuffer lost_sound_buffer;
sf::SoundBuffer click_sound_buffer;
sf::Sound bounce_sound,lost_sound,click_sound;
sf::Music music;

enum class game_state { IN_GAME, MENU, PLAY_MENU, OPTIONS_MENU, SINGLEPLAYER, MULTIPLAYER, DIFFICULTY_MENU, OPTION1, OPTION2,INSTRUCTIONS, EXIT };
enum class play_state { PLAYING, PAUSED ,FINISHED};
enum class position_of_object { TOP, MIDDLE };
enum class game_difficulty {EASY,MEDIUM,HARD};

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
void draw_background();
void draw_game_intro();
void draw_game_menu();
void draw_play_menu();
void draw_difficulty_menu();
void draw_playing();
void draw_instructions();
void draw_options_menu();
int get_font_size(int height_percentage, std::string text);
void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect);
struct object
{
	SDL_Texture *texture;
	std::string text;
	SDL_Rect rect;
	int size;
	bool hovering;
};
object logo, brand, start, play, options, quit, back, singleplayer, multiplayer, option1, option2;
object difficulty, easy, medium, hard , instructions , instruction1 , instruction2, instruction3, instruction4, instruction5, instruction6;
object score1, score2;
int player1_score, player2_score;
std::string player1_name;
std::string player2_name;
paddle paddle1, paddle2;
ball _ball;
field _field;
void hover_effect(object &_object, SDL_Event input, SDL_Rect top_object_rect, int number_of_objects, int object_number, std::function<void()>draw_function);
void update_object(object &_object, int height_percentage, std::string font_type, SDL_Color color);
void update_object_and_position(object &_object, int height_percentage, std::string font_type, SDL_Color color, position_of_object position, SDL_Rect top_object_rect, int number_of_objects, int object_number);
void update_middle_object_position(SDL_Rect top_object_rect, SDL_Rect &object_rect, int number_of_objects, int object_number);
void update_top_object_position(SDL_Rect &rect);
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
game_state _game_state;
play_state _play_state;
game_difficulty _game_difficulty;
int difficulty_level;
TTF_Font *_font;
SDL_Color _text_color;
std::string _font_type1;
std::string _font_type2;
void create_text_texture(SDL_Texture* &text_texture,std::string text_to_display, std::string type_of_font, int text_size, SDL_Color text_color);
Uint32 _windowID;
SDL_Rect null_rect;
int button_size;
int title_size;