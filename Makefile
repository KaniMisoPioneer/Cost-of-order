CC = gcc
CFLAGS = -g -Wall -Wextra -I./input -I./constants
LDFLAGS = -pthread

SOURCES = main.c input/input.c mongoose/mongoose.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server

TEMPLATES_DIR = templates
CSS_DIR = css

.PHONY: all clean run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Запуск сервера..."
	@./$(EXECUTABLE)

clean:
	@echo "Очистка..."
	@rm -f $(OBJECTS) $(EXECUTABLE)

init:
	@mkdir -p $(TEMPLATES_DIR) $(CSS_DIR)

format:
	@clang-format -i *.c *.h input/*.c input/*.h

test:
	@curl -X POST http://localhost:8081/calculate --data "bread=1&bread_qty=2"