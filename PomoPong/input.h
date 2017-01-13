#pragma once
#include<SDL.h>

class input
{
public:
	input();
	~input();
	void update();
	bool is_key_tapped(SDL_Scancode key);
	bool was_mouse_button_pressed(int button_number);
	int x, y;
	SDL_Event event;
	bool empty_event;
private:
	const Uint8 *keys;
	bool mouse[3];
	void get_mouse_buttons_state();
};

