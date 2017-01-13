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


bool input:: was_mouse_button_pressed(int button_number)
{
	if (button_number <= 3 && button_number >= 1)
		return mouse[button_number - 1];
	return 0;
}


void input:: get_mouse_buttons_state()
{
	if (SDL_PollEvent(&event))
	{
		empty_event = false;
		switch (event.type)
		{
		case  SDL_MOUSEBUTTONUP:
			switch (event.button.button)// 1 = left  2 = center  3 = right
			{
			case SDL_BUTTON_LEFT:
				mouse[0] = true;
			case SDL_BUTTON_MIDDLE:
				mouse[1] = true;
			case SDL_BUTTON_RIGHT:
				mouse[2] = true;
			}
			break;
			break;
		}
	}
	else empty_event = true;
}