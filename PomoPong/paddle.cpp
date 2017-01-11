#include "paddle.h"



paddle::paddle()
{
	x = 0;
	y = 0;
	width = 10;
	height = 100;
	reset_speed = 0.5;
}


paddle::~paddle()
{
}


void paddle::draw(SDL_Renderer *&_render_target)
{
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_RenderFillRect(_render_target, &rect);
}


void paddle::update(int new_x, int new_y)
{
	x = new_x;
	y = new_y;
}


void paddle::reset()
{
	speed = reset_speed;
}


void paddle::set_reset_speed(double new_speed)
{
	reset_speed = new_speed;
}


void paddle::update_rect()
{
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}


void paddle::set_reset_position(double new_x, double new_y)
{
	reset_x = new_x;
	reset_y = new_y;
}