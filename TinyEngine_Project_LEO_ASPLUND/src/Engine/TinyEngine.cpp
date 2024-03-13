#include "TinyEngine.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <ctime>

// Rendering stuff
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static TTF_Font* font = nullptr;

// Engine state stuff
static bool is_open = false;
static float delta_time = 0.f;
static unsigned int frame_count = 0;

// Input stuff
struct Input_State
{
	bool down = false;
	unsigned int update_frame = 0;

	void set(bool new_down)
	{
		down = new_down;
		update_frame = frame_count;
	}
};

static Input_State key_states[(int)Key::Count];
static Input_State mouse_states[(int)Mouse_Button::Count];
static int mouse_x;
static int mouse_y;

// Time keeping stuff
static LARGE_INTEGER clock_freq;
static LARGE_INTEGER last_frame_time;
static LARGE_INTEGER startup_time;

void engInit(const char* title, int width, int height)
{
	// Inititalize SDL and create a window + renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Initialize randomness
	srand(time(NULL));

	// Enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Initialize fonts
	TTF_Init();
	font = TTF_OpenFont("res/Roboto-Regular.ttf", 24);

	// Initialize clock for delta time calculation
	QueryPerformanceFrequency(&clock_freq);
	QueryPerformanceCounter(&startup_time);
	last_frame_time = startup_time;

	is_open = true;
}

void engClose()
{
	is_open = false;
}

bool engBeginFrame()
{
	if (!is_open)
	{
		return false;
	}

	// Present last frame
	SDL_RenderPresent(renderer);

	// Update delta time
	LARGE_INTEGER frame_time;
	QueryPerformanceCounter(&frame_time);
	delta_time = float(frame_time.QuadPart - last_frame_time.QuadPart) / clock_freq.QuadPart;

	last_frame_time = frame_time;

	frame_count++;

	// Read window events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				is_open = false;
				return false;
			}

			case SDL_KEYDOWN:
			{	
				// We don't care about key repeats
				if (event.key.repeat)
					break;
				
				Input_State& state = key_states[event.key.keysym.scancode];
				state.set(true);
				break;
			}

			case SDL_KEYUP:
			{
				Input_State& state = key_states[event.key.keysym.scancode];
				state.set(false);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				Input_State& state = mouse_states[event.button.button];
				state.set(true);
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				Input_State& state = mouse_states[event.button.button];
				state.set(false);
				break;
			}

			case SDL_MOUSEMOTION:
			{
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
				break;
			}
		}
	}

	// Don't burn down the CPU
	SDL_Delay(1);

	return true;
}

float engCurrentTime()
{
	return float(last_frame_time.QuadPart - startup_time.QuadPart) / clock_freq.QuadPart;
}

float engTimePassedSince(float time)
{
	return engCurrentTime() - time;
}

float engDeltaTime() { return delta_time; }

int engRandom(int max)
{
	return rand() % max;
}

int engRandom(int min, int max)
{
	return (rand() % (max - min)) + min;
}

float engRandomF()
{
	return (float)rand() / RAND_MAX;
}

Key engWaitKey()
{
	// Present what we've drawn before we block
	SDL_RenderPresent(renderer);

	SDL_Event event;
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				return (Key)event.key.keysym.scancode;
			}
		}
	}
}

void engSetDrawColor(Color color)
{
	SDL_SetRenderDrawColor(
		renderer,
		(color >> 24) & 0xFF,
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		(color) & 0xFF
	);
}

void engClearScreen()
{
	SDL_RenderClear(renderer);
}

void engDrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void engDrawRect(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderDrawRect(renderer, &rect);
}

void engFillRect(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderFillRect(renderer, &rect);
}

void engDrawCircle(int x, int y, float radius)
{
	static constexpr int RESOLUTION = 128;
	static constexpr float STEP = 6.28318530718f / RESOLUTION;

	SDL_FPoint points[RESOLUTION + 1];

	for (int i = 0; i <= RESOLUTION; ++i)
	{
		float offset_x = cosf(STEP * i);
		float offset_y = sinf(STEP * i);

		points[i] = {
			x + cosf(STEP * i) * radius,
			y + sinf(STEP * i) * radius
		};
	}

	SDL_RenderDrawLinesF(renderer, points, RESOLUTION + 1);
}

void engDrawText(int x, int y, const char* string)
{
	SDL_Color color;
	SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);

	SDL_Surface* surface = TTF_RenderText_Blended(font, string, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect target_rect = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &target_rect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void engDrawTextF(int x, int y, const char* format, ...)
{
	static char BUFFER[2048];

	va_list vl;
	va_start(vl, format);

	vsprintf_s<2048>(BUFFER, format, vl);
	engDrawText(x, y, BUFFER);

	va_end(vl);
}

bool engKeyDown(Key key)
{
	Input_State& state = key_states[(int)key];
	return state.down;
}

bool engKeyPressed(Key key)
{
	Input_State& state = key_states[(int)key];
	return state.down && state.update_frame == frame_count;
}

bool engKeyReleased(Key key)
{
	Input_State& state = key_states[(int)key];
	return !state.down && state.update_frame == frame_count;
}

bool engMouseButtonDown(Mouse_Button button)
{
	Input_State& state = mouse_states[(int)button];
	return state.down;
}

bool engMouseButtonPressed(Mouse_Button button)
{
	Input_State& state = mouse_states[(int)button];
	return state.down && state.update_frame == frame_count;
}

bool engMouseButtonReleased(Mouse_Button button)
{
	Input_State& state = mouse_states[(int)button];
	return !state.down && state.update_frame == frame_count;
}

int engMouseX() { return mouse_x; }
int engMouseY() { return mouse_y; }