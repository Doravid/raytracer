CC = clang
CFLAGS = -Wall -Wextra -O3 -ffast-math -g 
TARGET = raytracer.out
SRCS = main.c
DEPS = linear_algebra.h Makefile

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm

clean:
	rm -f $(TARGET) *.ppm *.out