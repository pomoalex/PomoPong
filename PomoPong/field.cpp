#include "field.h"
#include<iostream>


field::field()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}


field::~field()
{
}


void draw_circle(SDL_Renderer *&_render_target,int x, int y, int r)
{
	for (int line = x - r;line <= x + y; line++)
		for (int column = y - y;column <= y + r;column++)
			if ((line - x) * (line - x) + (column - y) * (column - y) < r * r + 200)
				if ((line - x) * (line - x) + (column - y) * (column - y) > r * r - 200)
					SDL_RenderDrawPoint(_render_target, line, column);
}


void field::draw(SDL_Renderer *&_render_target)
{
	SDL_RenderDrawLine(_render_target, x, y, x + width, y);
	SDL_RenderDrawLine(_render_target, x, y, x, y + height);
	SDL_RenderDrawLine(_render_target, x + width, y + height, x + width, y);
	SDL_RenderDrawLine(_render_target, x + width, y + height, x, y + height);
	SDL_RenderDrawLine(_render_target, x + width / 2, y + height, x + width / 2, y);
	draw_circle(_render_target,x + width / 2, y + height / 2, 150);
}


void field::set(int new_x, int new_y,int new_width,int new_height)
{
	x = new_x;
	y = new_y;
	width = new_width;
	height = new_height;
}


void field::update_rect()
{
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}