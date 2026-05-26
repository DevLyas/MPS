#include "utils.h"
#include <cstdint>
#include <string>
#include <SDL3/SDL.h>

#ifndef APPLICATION_H
#define APPLICATION_H

struct Application
{
	SDL_Window   *window;
	SDL_Renderer *renderer;

	std::string  name;
	std::string  version;
	std::string  identifier;


	int          window_width;
	int          window_height;

	bool         running;
	uint64_t     frame_time;
	float        fps;

	void init();
	void run();
	void quit();

	void frame();
	void handle();
	void update();
	void render();
};

#endif /* APPLICATION_H */
