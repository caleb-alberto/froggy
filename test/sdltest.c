#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define width 64
#define height 32
#define scale 20
#define N 5

int main() {
	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);

        uint32_t video[width * height];
        memset(video, 0, sizeof(video));
        uint8_t memory[4096];
        memset(memory, 0 sizeof(memory));
        uint16_t index = 0x00F2;

        uint8_t sprite[N] = { 0xF0, 0x10, 0xF0, 0x80, 0xFF };
        // sample x,y cords to begin rendering the sprite at:
        int pix_x = 0;
        int pix_y = 0;

        pix_x %= 64;
        pix_y %= 32;

        video[0] = 0xFFFFFFFF;

        for (int i = 0; i < N; i++) {
                int row = (i + pix_y) * 64;

                for (int j = 0; j < 8; j++) {
                        int collumn = j + pix_x;

                        if (collumn < 64 && (collumn + row) < (width * height)) {
                                uint8_t video_bit = sprite[i] << j;
                                video_bit &= 0x80;

                                if (video_bit == 0x80)
                                        video[collumn + row] ^= 0xFFFFFFFF;
                        }
                }
        }

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
