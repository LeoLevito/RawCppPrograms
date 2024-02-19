#pragma once
#include "Input.h"

// Some default colors !!
using Color = unsigned int;
#define COLOR_BLACK 0x000000FF
#define COLOR_DARK_GRAY 0x222222FF
#define COLOR_GRAY 0x666666FF
#define COLOR_LIGHT_GRAY 0xC0C0C0FF
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0x00FF00FF
#define COLOR_BLUE 0x0000FFFF
#define COLOR_YELLOW 0xFFD700FF
#define COLOR_ORANGE 0xFFA500FF
#define COLOR_SALMON 0xFA8072FF
#define COLOR_CYAN 0x00FFFFFF
#define COLOR_MAGENTA 0xFF00FFFF
#define COLOR_VIOLET 0x8A2BE2FF
#define COLOR_LAVENDER 0xE6E6FAFF

// --- Engine functions ---
// Opens a window! Must be called before any other eng- functions
void engInit(const char* title, int width, int height);
// Closes the window
void engClose();

// Locks the program and waits for the user to press a key. Returns which key was pressed.
Key engWaitKey();

// Begins a frame, updating delta-time and input states.
// Returns false if 'engClose' was called or the user closed the window
bool engBeginFrame();

// Returns the time passed (in seconds) between previous and current frame
float engDeltaTime();

// --- Random functions ---
int engRandom(int max);
int engRandom(int min, int max);
float engRandomF();

// --- Draw functions ---
// Sets the current draw color for all subsequent draw / clear calls
// Specified in hex (0xRRGGBBAA in hexadecimal)
void engSetDrawColor(Color color);

// Clears the screen to specified draw color
void engClearScreen();

// Draws a line between two points
void engDrawLine(int x1, int y1, int x2, int y2);

// Draws an outlined rectangle
void engDrawRect(int x, int y, int width, int height);
// Draws a filled rectangle
void engFillRect(int x, int y, int width, int height);

// Draws a circle outline
void engDrawCircle(int x, int y, float radius);

// Draws text to screen
void engDrawText(int x, int y, const char* string);
// Draws formatted text, visit https://cplusplus.com/reference/cstdio/printf/ for formatting options
void engDrawTextF(int x, int y, const char* format, ...);

// --- Input functions ---
// NOTE: These functions only work when using 'engBeginFrame'

// Returns if specified key is held down
bool engKeyDown(Key key);
// Returns if a key was pressed this frame (subsequent held frames will return false)
bool engKeyPressed(Key key);
// Returns if a key was released this frame (subsequent un-held frames will return false)
bool engKeyReleased(Key key);

// Returns if a mouse button is held down
bool engMouseButtonDown(Mouse_Button button);
// Returns if a mouse button was pressed this frame (subsequent held frames will return false)
bool engMouseButtonPressed(Mouse_Button button);
// Returns if a mouse button was released this frame (subsequent un-held frames will return false)
bool engMouseButtonReleased(Mouse_Button button);

// Returns x position of the mouse cursor
int engMouseX();
// Returns y position of the mouse cursor
int engMouseY();