#pragma once
#include<SDL.h>
class ball
{
public:
	ball();
	~ball();
	void draw(SDL_Renderer *&_render_target);
	void update(double nex_x,double new_y);
	void update_speed(double new_x_speed,double new_y_speed);
	void reset();
	void set_reset_speed(double new_speed);
	void set_reset_position(double new_x, double new_y);
	double x;
	double y;
	double x_speed;
	double y_speed;
	int radius;
	double speed;
	double max_bounce_angle;
private:
	int reset_x;
	double reset_speed ;
	int reset_y;
};

