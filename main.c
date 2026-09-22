#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define START_ADDRESS 0x200
#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x50

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
} chip8;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void main_loop(chip8* this);
uint16_t get_instruction(chip8* this);
void read_ROM(chip8* this, char* filename);
uint8_t rand_byte();

int main() {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        time_t time_s = 0;
        long time_n = 0, prev_time_n = 0, last_cycle = 0;
        const long freq_ns = (long)(1e9 / 700);

        chip8 mychip8;
        mychip8.pc = START_ADDRESS;

        for (int i = 0; i < FONTSET_SIZE; i++)
                mychip8.memory[FONTSET_START_ADDRESS + i] = fontset[i];

        while (1) {
		clock_gettime(CLOCK_MONOTONIC, &end);
		time_s = end.tv_sec - start.tv_sec;
		time_n = end.tv_nsec - start.tv_nsec;

                if (last_cycle > freq_ns) {
                        last_cycle = 0;
                        main_loop(&mychip8);
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
}

void main_loop(chip8* this) {
        uint16_t instruction = get_instruction(this);

        uint8_t first = (instruction & 0xF000) >> 12;
        uint8_t x = (instruction & 0x0F00) >> 8;
        uint8_t y = (instruction & 0x00F0) >> 4;
        uint8_t n = (instruction & 0x000F);
        uint8_t nn = (instruction & 0x00FF);

        switch (first) {
                case 0x0:
                        if (nn == 0xE0)
                                memset(this->video, 0, sizeof(this->video));
                        if (nn == 0xEE) {
                                this->sp--;
                                this->pc = this->stack[this->sp];
                        }
                        break;
                case 0x1:
                        this->pc = instruction & 0x0FFF;
                        break;
                case 0x2:
                        this->stack[this->sp] = this->pc;
                	this->sp++;
                	this->pc = instruction & 0x0FFF;
                        break;
                case 0x3:
                        if (this->registers[x] == nn)
                                this->pc += 2;
                        break;
                case 0x4:
                        if (this->registers[x] != nn)
                                this->pc += 2;
                        break;
                case 0x5:
                        if (this->registers[x] == this->registers[y])
                                this->pc += 2;
                        break;
                case 0x6:
                        this->registers[x] = nn;
                        break;
                case 0x7:
                        this->registers[x] += nn;
                        break;
                case 0x8:
                        switch (n) {
                                case 0x0:
                                        this->registers[x] = this->registers[y];
                                        break;
                                case 0x1:
                                        this->registers[x] |= this->registers[y];
                                        break;
                                case 0x2:
                                        this->registers[x] &= this->registers[y];
                                        break;
                                case 0x3:
                                        this->registers[x] ^= this->registers[y];
                                        break;
                                case 0x4:
                                        this->registers[x] += this->registers[y];
                                        break;
                                case 0x5:
                                        this->registers[x] -= this->registers[y];
                                        break;
                                case 0x6:
                                        this->registers[x] = (this->registers[y] >> 1);
                                        break;
                                case 0x7:
                                        this->registers[x] = this->registers[y] - this->registers[x];
                                        break;
                                case 0xE:
                                        this->registers[x] = (this->registers[y] << 1);
                                        break;
                        }
                        break;
                case 0x9:
                        if (this->registers[x] != this->registers[y])
                                this->pc += 2;
                        break;
                case 0xA:
                        this->index = nnn;
                        break;
                case 0xB:
                        this->pc = instruction & 0x0FFF;
                        this->pc += this->registers[0];
                        break;
                case 0xC:
                        this->registers[x] = rand_byte() & nn;
                        break;
        }
}

uint16_t get_instruction(chip8* this) {
        uint16_t instruction = (this->memory[this->pc] & 0xFF) << 8;
        this->pc++;

        instruction = (instruction & 0xFF00) ^ (this->memory[this->pc] & 0xFF);
        this->pc++;

        return instruction;
}

void read_ROM(chip8* this, char* filename) {
        FILE* rom = fopen(filename, "rb");

        fseek(rom, 0, SEEK_END);
        long size = ftell(rom);
        uint8_t buffer[size];

        fseek(rom, 0, SEEK_SET);
        fread(buffer, sizeof(uint8_t), size, rom);

	for (long i = 0; i < size; ++i)
		this->memory[START_ADDRESS + i] = buffer[i];
}

uint8_t rand_byte() {
        srand(time(NULL));
        uint8_t byte = (uint8_t)rand();

        return byte;
}
