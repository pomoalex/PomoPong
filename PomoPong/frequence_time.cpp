#include "frequence_time.h"


frequence_time::frequence_time()
{
	last_time = 0;
	current_time = 0;
	delta_time = 0;
	accumulated_time = 0;
	step_time = 1;
}


frequence_time::~frequence_time()
{
}


void frequence_time::update()
{
	last_time = current_time;
	current_time = SDL_GetTicks();
	delta_time = current_time - last_time;
	accumulated_time += delta_time;
}