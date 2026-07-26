CC = clang
CFLAGS = -Wall -Wextra -O3 -ffast-math -g -fno-omit-frame-pointer -march=native
TARGET = raytracer.out
SRCS = main.c
DEPS = linear_algebra.h window.h Makefile

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm -lraylib

clean:
	rm -f $(TARGET) *.ppm *.out