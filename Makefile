CFLAGS  := -Wall
LDFLAGS := -lSDL2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS += -L/opt/homebrew/lib
endif


build: main.c
	gcc $(CFLAGS) main.c -o main

clean: main
	rm -f main

debug: main.c
	gcc $(CFLAGS) -g main.c -o main
