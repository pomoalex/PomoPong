#pragma once
#include<SDL.h>
class field
{
public:
	field();
	~field();
	void draw(SDL_Renderer *&_render_target);
	void set(int new_x, int new_y, int new_width, int new_height);
	int x;
	int y;
	int width;
	int height;
	SDL_Rect rect;
	void update_rect();
private:
	int radius;
	double reset_x;
	double reset_y;
};

