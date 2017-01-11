#pragma once
#include<SDL.h>
class frequence_time
{
public:
	frequence_time();
	~frequence_time();
	int accumulated_time;
	int step_time;
	void update();
private:
	int last_time;
	int current_time;
	int delta_time;
};

