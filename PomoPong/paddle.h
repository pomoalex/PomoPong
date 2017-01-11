#pragma once
#include<SDL.h>
class paddle
{
public:
	paddle();
	~paddle();
	void draw(SDL_Renderer *&_render_target);
	void update(int nex_x, int new_y);
	void reset();
	void set_reset_speed(double new_speed);
	void set_reset_position(double new_x, double new_y);;
	double x;
	double y;
	int width;
	int height;
	SDL_Rect rect;
	void update_rect();
	double speed;
private:
	double reset_x;
	double reset_speed;
	double reset_y;
};

