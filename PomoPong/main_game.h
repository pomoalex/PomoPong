#pragma once  //makes sure you include a header only once
#include <SDL.h>
#include<SDL_ttf.h>
enum class game_state{PLAY,MENU,PLAY_MENU,OPTIONS_MENU,EXIT};
class main_game
{
public:
	main_game();
	~main_game();
	void play();
	void game_loop();
	void game_menu();
	void game_intro();
	void init_game();
	void draw_background();
	int get_font_size(int width_percentage, int height_percentage, std::string text);
private:
	SDL_Window* _window;
	SDL_Texture* _background_image;
	std::string _logo_string;
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
	void draw_texture(SDL_Texture *texture, SDL_Rect texture_rect);
	Uint32 _windowID;
};

