#include "application.h"
#include "material.h"
#include <cstdint>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define TARGET_FPS    60
#define GRID_SIZE     10

static constexpr const char *app_name       = "Material Particle Simulation";
static constexpr const char *app_version    = "0.1.0";
static constexpr const char *app_identifier = "com.devlyas.mps";

static Grid grid(GRID_SIZE, WINDOW_HEIGHT/GRID_SIZE, WINDOW_WIDTH/GRID_SIZE, false);
static Brush brush;
static std::vector<Particle> sand;
static constexpr uint32_t target_frame_time = 1000000000/TARGET_FPS;
static double simulation_time;
static Vec2 mouse;
static bool is_key_down  = false;
static bool debug_hidden = false;
static uint32_t speed = 32;
static uint32_t timer;
static bool paused = false;
static SDL_Color sand_color = {255, 200, 0, SDL_ALPHA_OPAQUE};

void Application::init()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_SetAppMetadata(app_name, app_version, app_identifier);

	window   = SDL_CreateWindow(app_name, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, nullptr);
}

void Application::run()
{
	while (running)
	{
		frame();
	}
}

void Application::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::frame()
{
	uint64_t frame_start = SDL_GetTicksNS();

	handle();
	update();
	render();

	frame_time = SDL_GetTicksNS() - frame_start;
	simulation_time = frame_time/1000000.0;
	
	if (frame_time < target_frame_time)
	{
		uint32_t frame_delay  = target_frame_time - frame_time;
		frame_time           += frame_delay;
		SDL_DelayNS(frame_delay);
	}
	if (frame_time > 0) fps = 1000000000.0f/frame_time;
}

void Application::handle()
{
	SDL_Event event;

	SDL_GetMouseState(&mouse.x, &mouse.y);

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			running = false;
		}
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			int ctrl = event.key.mod & SDL_KMOD_CTRL;
			switch (event.key.key)
			{
				case SDLK_SPACE:
					sand.clear();
				break;
				case SDLK_UP:
					if (brush.magnitude < (grid.cols-1)/2)
					brush.magnitude++;
				break;
				case SDLK_DOWN:
					if (brush.magnitude > 0) brush.magnitude--;
				break;
				case SDLK_LEFT:
					if (speed > 1) speed /= 2;
				break;
				case SDLK_RIGHT:
					if (speed < 1024) speed *= 2;
				break;
				case SDLK_S:
					grid.hidden = !grid.hidden;
				break;
				case SDLK_P:
					paused = !paused;
				break;
				case SDLK_MINUS:
					if (ctrl)
					{
						if (grid.size == 10) grid.size -= 9;
						else if (grid.size > 10) grid.size -= 10;

						while (grid.size > 2 && (window_width % grid.size != 0 || window_height % grid.size != 0))
						{
							grid.size -= 2;
						}
					}
				break;
				case SDLK_EQUALS:
					if (ctrl)
					{
						if (grid.size == 1) grid.size += 9;
						else if (grid.size < 100) grid.size += 10;

						while (grid.size < window_height / 2 && (window_width % grid.size != 0 || window_height % grid.size != 0))
						{
							grid.size += 2;
						}
					}
				break;
				case SDLK_D:
					debug_hidden = !debug_hidden;
				break;
			}
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			is_key_down = true;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			is_key_down = false;
		}
	}
}

void Application::update()
{
	timer += frame_time;
	SDL_GetWindowSize(window, &window_width, &window_height);

	grid.update(window_width, window_height);
	brush.update((int)mouse.x/grid.size, (int)mouse.y/grid.size, grid.size);

	if (!paused && timer > speed*1000000)
	{
		for (auto& s : sand)
		{
			if (s.rect.y < grid.rows * grid.size &&
			    !is_occupied(sand, s.rect.x, s.rect.y + grid.size))
			{
				s.state = State::SOUTH;
			}
			else if (s.rect.x - grid.size >= 0 &&
				 s.rect.y + grid.size < grid.rows * grid.size &&
				 !is_occupied(sand, s.rect.x - grid.size, s.rect.y + grid.size))
			{
				s.state = State::SOUTH_WEST;
			}
			else if (s.rect.x + grid.size < grid.cols * grid.size && s.rect.y + grid.size < grid.rows * grid.size && !is_occupied(sand, s.rect.x + grid.size, s.rect.y + grid.size))
			{
				s.state = State::SOUTH_EAST;
			}
			else
			{
				s.state = State::STATIC;
			}


			switch (s.state)
			{
				case State::STATIC:
				break;
				case State::NORTH:
				break;
				case State::NORTH_EAST:
				break;
				case State::EAST:
				break;
				case State::SOUTH_EAST:
					s.rect.x += grid.size;
					s.rect.y += grid.size;
				break;
				case State::SOUTH:
					s.rect.y += grid.size;
				break;
				case State::SOUTH_WEST:
					s.rect.x -= grid.size;
					s.rect.y += grid.size;
				break;
				case State::WEST:
				break;
				case State::NORTH_WEST:
				break;
			}

			if (s.rect.y >= grid.rows * grid.size)
			{
				s.rect.y = (grid.rows - 1) * grid.size;
				s.state = State::STATIC;
			}
		}
		timer = 0;
	}


	if (is_key_down)
	{
		for (int y = (int)mouse.y/grid.size - brush.magnitude; y <= (int)mouse.y/grid.size + brush.magnitude; y++) {
			for (int x = (int)mouse.x/grid.size - brush.magnitude; x <= (int)mouse.x/grid.size + brush.magnitude; x++)
			{
				if (x >= 0 && x < grid.cols * grid.size && y >= 0 && y < grid.rows * grid.size && !is_occupied(sand, x*grid.size, y*grid.size))
				{
					sand.push_back({ {(float)x*grid.size, (float)y*grid.size, (float)grid.size, (float)grid.size}, State::STATIC });
				}
			}
		}
	}
}

void Application::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (int i = 0; i < sand.size(); i++)
	{
		SDL_SetRenderDrawColor(renderer, sand_color.r, sand_color.g, sand_color.b, sand_color.a);
		SDL_RenderFillRect(renderer, &sand[i].rect);
	}

	SDL_FRect rect = {brush.rect.x+1, brush.rect.y+1, brush.rect.w-1, brush.rect.h-1};
	SDL_SetRenderDrawColor(renderer, sand_color.r, sand_color.g, sand_color.b, sand_color.a);
	SDL_RenderRect(renderer, &rect);

	if (!grid.hidden)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		// Horizontal lines
		for (int x = 0; x < window_width; x += grid.size)
		{
			SDL_RenderLine(renderer, x, 0, x, window_height - 1);
		}
		SDL_RenderLine(renderer, 0, window_height - 1, window_width - 1, window_height - 1);

		// Vertical lines
		for (int y = 0; y < window_height; y += grid.size)
		{
			SDL_RenderLine(renderer, 0, y, window_width - 1, y);
		}
		SDL_RenderLine(renderer, window_width-1, 0, window_width - 1, window_height-1);
	}

	if (!debug_hidden)
	{
		// Debug text
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_SetRenderScale(renderer, 1.5f, 1.5f);

		// Left side
		SDL_RenderDebugTextFormat(renderer, 5, 5, "paused:%s", paused ? "true" : "false");
		SDL_RenderDebugTextFormat(renderer, 5, 15, "grid visible:%s", !grid.hidden ? "true" : "false");
		SDL_RenderDebugTextFormat(renderer, 5, 25, "grid size:%d", grid.size);
		SDL_RenderDebugTextFormat(renderer, 5, 35, "grid rows:%d", grid.rows);
		SDL_RenderDebugTextFormat(renderer, 5, 45, "grid cols:%d", grid.cols);
		SDL_RenderDebugTextFormat(renderer, 5, 55, "grid occupied:%s", is_occupied(sand, brush.rect.x, brush.rect.y) ? "true" : "false");
		SDL_RenderDebugTextFormat(renderer, 5, 65, "mouse pos:(%.0f,%.0f)", mouse.x, mouse.y);
		SDL_RenderDebugTextFormat(renderer, 5, 75, "brush pos:(%.0f,%.0f)", brush.rect.x, brush.rect.y);
		SDL_RenderDebugTextFormat(renderer, 5, 85, "brush size:%.0f", brush.rect.w);
		SDL_RenderDebugTextFormat(renderer, 5, 95, "sand:%zu", sand.size());
		SDL_RenderDebugTextFormat(renderer, 5, 105, "simulation speed:%dms", speed);
		SDL_RenderDebugTextFormat(renderer, 5, 115, "simulation time:%.3lfms/frame", simulation_time);

		// Right side
		SDL_RenderDebugTextFormat(renderer, window_width - window_width/2.8f, 5, "%.0f", fps);
		SDL_SetRenderScale(renderer, 1.0f, 1.0f);
	}

	SDL_RenderPresent(renderer);
}
