#pragma once  //makes sure you include a header only once
#include <SDL.h>
#include<SDL_ttf.h>
enum class game_state{PLAY,MENU,PLAY_MENU,OPTIONS_MENU,ONE_PLAYER,TWO_PLAYERS,OPTION1,OPTION2,EXIT};
class main_game
{
public:
	main_game();
	~main_game();
	void play_game();
	void game_loop();
	void game_menu();
	void game_intro();
	void init_game();
	void game_play_menu();
	void game_options_menu();
	void draw_background();
	void draw_game_intro();
	void draw_game_menu();
	void draw_play_menu();
	void draw_options_menu();
	int get_font_size(int width_percentage, int height_percentage, std::string text);
	void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect);
private:
	struct object
	{
		SDL_Texture *texture;
		std::string text;
		SDL_Rect rect;
		int size;
	};
	void update_object(object &_object, int height_percentage, std::string font_type, SDL_Color color);
	int hover_animation_speed;
	object logo, brand, start, play, options, quit, back, one_player, two_players, option1, option2;
	SDL_Window* _window;
	SDL_Texture* _background_image;
	SDL_Color _primary_color;
	SDL_Color _secondary_color;
	SDL_Renderer* _render_target;
	int _window_width;
	int _window_height;
	game_state _game_state;
	TTF_Font *_font;
	SDL_Color _text_color;
	std::string _font_type1;
	std::string _font_type2;
	SDL_Texture* create_text_texture(std::string text_to_display, std::string type_of_font, int text_size, SDL_Color text_color);
	Uint32 _windowID;
};

