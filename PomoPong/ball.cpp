#include "ball.h"
#include<iostream>


ball::ball()
{
	x = 0;
	y = 0;
	radius = 10;
	x_speed = 0;
	y_speed = 0;
	reset_speed = 0.5;
	speed = reset_speed ;
	max_bounce_angle = (M_PI ) / 3; // 75 degree
}


ball::~ball()
{
}


void ball::draw (SDL_Renderer *&_render_target)
{
	for (int line = (int)x - radius;line <= (int)x + (int)y; line++)
		for (int column = (int)y - radius;column <= (int)y + radius;column++)
			if ((line - (int)x) * (line - (int)x) + (column - (int)y) * (column - (int)y) <= radius * radius)
				SDL_RenderDrawPoint(_render_target, line, column);
}


void ball::update(double new_x, double new_y)
{
	x = new_x;
	y = new_y;
}


void ball::update_speed(double new_x_speed, double new_y_speed)
{
	x_speed = new_x_speed;
	y_speed = new_y_speed;
}


void ball::reset()
{
	speed = reset_speed;
}


void ball::set_reset_speed(double new_speed)
{
	reset_speed = new_speed;
}


void ball:: set_reset_position(double new_x, double new_y)
{
	reset_x = new_x;
	reset_y = new_y;
}