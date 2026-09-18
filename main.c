#include <stdint.h>
#include <stdio.h>
#include <time.h>


const unsigned int START_ADDRESS = 0x200;

typedef struct {
	uint8_t registers[16];
	uint8_t memory[4096];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[16];
	uint8_t sp;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t keypad[16];
	uint32_t video[64 * 32];
	uint16_t opcode;
} Chip8;

void main_loop() {
}

void read_ROM(Chip8* this, char* filename) {
        FILE* rom = fopen(filename, "rb");

        fseek(rom, 0, SEEK_END);
        long size = ftell(rom);
        uint8_t buffer[size];

        fseek(rom, 0, SEEK_SET);
        fread(buffer, sizeof(uint8_t), size, rom);

	for (long i = 0; i < size; ++i)
		this->memory[START_ADDRESS + i] = buffer[i];
}

int main() {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        time_t time_s = 0;
        long time_n = 0, prev_time_n = 0, last_cycle = 0;
        int loops = 0;
        const long freq_ns = (long)(1e9 / 700);

        while (1) {
		clock_gettime(CLOCK_MONOTONIC, &end);
		time_s = end.tv_sec - start.tv_sec;
		time_n = end.tv_nsec - start.tv_nsec;

                if (last_cycle > freq_ns) {
                        loops++;
                        last_cycle = 0;
                }
                else {
                        if (prev_time_n > time_n)
                                last_cycle += (long)(1e9 - prev_time_n) + time_n;
                        else
                                last_cycle += time_n - prev_time_n;
                }

                prev_time_n = time_n;

                if (time_n >= 0 && time_n <= 100 && time_s == 1)
                        break;
        }

        printf("%d loops in %d seconds\n", loops, (int)time_s);
}
