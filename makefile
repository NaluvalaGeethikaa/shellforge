CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LIBS = -lreadline
TARGET = shellforge

SRCS = src/history.c src/lexer.c src/main.c src/token.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
