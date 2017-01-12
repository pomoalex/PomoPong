#pragma once
#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<iostream>

void error(std::string error_text);
double get_random_number(int min_bound, int max_bound);
int get_random_sign();

void error(std::string error_text)
{
	std::cout << error_text << std::endl;
	std::cout << "Press any key ... ";
	char c;
	std::cin >> c;
	SDL_Quit();
	TTF_Quit();
	exit(1);
}


double get_random_number(int min_bound, int max_bound)
{
	std::random_device rd;
	std::default_random_engine generator(rd()); // rd() provides a random seed
	std::uniform_real_distribution<double> distribution(min_bound, max_bound);

	return distribution(generator);
}


int get_random_sign()
{
	std::random_device rd;
	std::default_random_engine generator(rd()); // rd() provides a random seed
	std::uniform_real_distribution<double> distribution(-1, 1);
	if (distribution(generator) > 0)
		return  1;
	return -1;
}
