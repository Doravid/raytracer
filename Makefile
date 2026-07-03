CC = clang
CFLAGS = -Wall -Wextra -O3 -ffast-math
TARGET = raytracer.out
SRCS = main.c
DEPS = linear_algebra.h window.h Makefile

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm -lraylib

clean:
	rm -f $(TARGET) *.ppm *.out