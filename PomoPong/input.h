#pragma once
#include<SDL.h>

class input
{
public:
	input();
	~input();
	void update();
	bool is_key_tapped(SDL_Scancode key);
	bool is_mouse_button_down(int button_number);
	SDL_Event get_event();
private:
	const Uint8 *keys;
	bool mouse[3];
	int x, y;
	SDL_Event event;
	void get_mouse_buttons_state();
};

