CC = clang
CFLAGS = -Wall -Wextra
TARGET = raytracer
SRCS = main.c
DEPS = linear_algebra.h

all: $(TARGET)

$(TARGET): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lm

clean:
	rm -f $(TARGET)