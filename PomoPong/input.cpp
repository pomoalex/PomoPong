#include "input.h"



input::input()
{
	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(NULL);
	x = y = 0;
	for (int index = 0;index < 3;index++)
		mouse[index] = 0;
}


input::~input()
{
}


void input:: update()
{
	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&x, &y);
	get_mouse_buttons_state();
}


bool input:: is_key_tapped(SDL_Scancode key)
{
	if (keys[key] == 1)
		return true;
	return false;
}


bool input:: is_mouse_button_down(int button_number)
{
	if (button_number <= 3 && button_number >= 1)
		return mouse[button_number - 1];
	return 0;
}


SDL_Event input:: get_event()
{
	return event;
}


void input:: get_mouse_buttons_state()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case  SDL_MOUSEBUTTONDOWN:
		mouse[event.button.button - 1] = true; // 1 = left  2 = center  3 = right
		break;
	case  SDL_MOUSEBUTTONUP:
		mouse[event.button.button - 1] = false;
		break;
	}
}