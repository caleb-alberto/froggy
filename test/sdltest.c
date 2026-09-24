#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define width 64
#define height 32
#define scale 20

int main() {
	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);

        uint32_t video[width * height];
        memset(video, 0, sizeof(video));

	window = SDL_CreateWindow(
                "froggy",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width * scale,
                height * scale,
                0
        );

        SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, 0);
        SDL_Texture* texture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING,
                width,
                height
        );

        SDL_UpdateTexture(texture, NULL, video, width * sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (1) {
                SDL_PollEvent(&event);
                if (event.type == SDL_QUIT) {
                        break;
                }
        }

	return 0;
}
