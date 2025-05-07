# Компилятор и флаги
CC = gcc
CFLAGS = -g -Wall -Wextra -I./input -I./constants
LDFLAGS = -pthread  # Mongoose может требовать pthread

# Исходные файлы
SOURCES = main.c input/input.c mongoose/mongoose.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server

# Пути к шаблонам (опционально, если нужно копировать)
TEMPLATES_DIR = templates
CSS_DIR = css

.PHONY: all clean run

# Основная цель
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Сборка объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Запуск сервера
run: all
	@echo "Запуск сервера..."
	@./$(EXECUTABLE)

# Очистка проекта
clean:
	@echo "Очистка..."
	@rm -f $(OBJECTS) $(EXECUTABLE)

# -------------------------------
# Дополнительные цели (опционально)
# -------------------------------

# Инициализация папок (если нужно копировать ресурсы)
init:
	@mkdir -p $(TEMPLATES_DIR) $(CSS_DIR)

# Проверка стиля кода (требует установки clang-format)
format:
	@clang-format -i *.c *.h input/*.c input/*.h

# Тестирование через curl (пример)
test:
	@curl -X POST http://localhost:8081/calculate --data "bread=1&bread_qty=2"