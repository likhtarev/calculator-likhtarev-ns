# Компилятор и флаги
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11
TARGET := app.exe
TEST_TARGET := unit-tests.exe
SRC := main.c
OBJ := $(SRC:.c=.o)
VENV_DIR := venv
PYTEST := $(VENV_DIR)/bin/pytest

# Создание директорий
$(shell mkdir -p build)

.PHONY: all clean run-int run-float run-unit-test run-integration-tests venv

# Сборка всех целей
all: build/$(TARGET) build/$(TEST_TARGET)

# Очистка артефактов
clean:
 @echo "Cleaning build and venv"
 @rm -rf build/ $(VENV_DIR)

# Запуск калькулятора в целочисленном режиме
run-int: build/$(TARGET)
 @echo "Running in integer mode"
 @build/$(TARGET)

# Запуск калькулятора в режиме с плавающей точкой
run-float: build/$(TARGET)
 @echo "Running in float mode"
 @build/$(TARGET) --float

# Запуск юнит-тестов
run-unit-test: build/$(TEST_TARGET)
 @echo "Running unit-tests"
 @build/$(TEST_TARGET)

# Создание виртуального окружения и установка pytest
venv:
 @echo "Creating virtual environment"
 @python -m venv $(VENV_DIR)
 @source $(VENV_DIR)/bin/activate; pip install -U pytest

# Запуск интеграционных тестов (Python)
run-integration-tests: build/$(TARGET) venv tests/integration/test_calculator.py
 @echo "Running integration tests..."
 @source $(VENV_DIR)/bin/activate; $(PYTEST) tests/integration/test_calculator.py

# Компиляция исполняемого файла калькулятора
build/$(TARGET): $(SRC)
 @echo "Building $(TARGET)"
 @$(CC) $(CFLAGS) -o build/$(TARGET) $(SRC)

# Компиляция тестового исполняемого файла
build/$(TEST_TARGET): tests/unit/test_calculator.c $(SRC)
 @echo "Building unit tests"
 @$(CC) $(CFLAGS) -o build/$(TEST_TARGET) tests/unit/test_calculator.c $(SRC)
