CC = clang
CFLAGS = -Wall -Wextra -o2
TARGET = raytracer
SRCS = main.c
DEPS = linear_algebra.h Makefile

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm

clean:
	rm -f $(TARGET)