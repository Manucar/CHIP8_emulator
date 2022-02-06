##### WORKING MAKEFILE WITH SDL #######

# A simple Makefile for compiling small SDL projects

# set the compiler
#CC := clang

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm -Iinclude
# add header files here
#HDRS := ../include/chip8_defines.h ../include/op_codes.h

# add source files here
SRCS := src/main.c src/op_codes.c src/sdl.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := chip8_emulator

# default recipe
all: $(EXEC)
 
showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean