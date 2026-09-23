#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);
	printf("SDL initialized\n");

	window = SDL_CreateWindow("An SDL2 window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
        while (1) {}

	SDL_Quit();
	return 0;
}
