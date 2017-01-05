#pragma once  //makes sure you include a header only once
#include <SDL.h>
enum class game_state{PLAY,EXIT};
class main_game
{
public:
	main_game();
	~main_game();
	void play();
	void game_loop();
	void process_input();
private:
	void init_game();
	SDL_Window* _window;
	SDL_Texture* _background_image;
	SDL_Renderer* _render_target;
	int _window_width;
	int _window_height;
	game_state _game_state;
};

