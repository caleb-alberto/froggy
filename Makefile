CFLAGS  := -Wall
LDFLAGS := -lSDL2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS += -L/opt/homebrew/lib
endif


build: main.c
	gcc main.c -o main $(CFLAGS) $(LDFLAGS)

clean: main
	rm -f main

debug: main.c
	gcc main.c -o main $(CFLAGS) -g $(LDFLAGS)
