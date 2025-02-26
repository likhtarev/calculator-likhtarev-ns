#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1024
#define INT_MIN_BOUND (-2000000000)
#define INT_MAX_BOUND 2000000000
#define EPSILON 1e-4

char *expr;
int pos = 0;
int float_mode = 0; // Флаг режима с плавающей запятой

double parse_factor();
double parse_term();
double parse_expression();

// Проверка разрешенных символов
void validate_input(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (!isdigit(c) && c != '(' && c != ')' && c != '*' && c != '+' && c != '-' && c != '/' && isspace(c) == 0) {
            fprintf(stderr, "Error: Invalid character in input\n");
            exit(1);
        }
    }
}

// Функция обработки чисел и выражений в скобках
double parse_factor() {
    while (isspace(expr[pos])) pos++; // Пропуск пробелов

    if (expr[pos] == '(') { // Обработка скобок
        pos++;
        double result = parse_expression();
        if (expr[pos] == ')') pos++;
        return result;
    }

    long long num = 0;
    while (isdigit(expr[pos])) { // Чтение числа
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }

    if (num < 0 || num > 2000000000) { // Проверка границ чисел
        fprintf(stderr, "Error: Number out of bounds\n");
        exit(1);
    }

    return (double) num;
}

// Функция обработки умножения и деления
double parse_term() {
    double result = parse_factor();

    while (1) {
        while (isspace(expr[pos])) pos++;

        if (expr[pos] == '*') {
            pos++;
            result *= parse_factor();
        } else if (expr[pos] == '/') {
            pos++;
            double divisor = parse_factor();
            if (fabs(divisor) < EPSILON) { // Проверка деления на слишком малое число
                fprintf(stderr, "Error: Division by zero or too small value\n");
                exit(1);
            }
            result = float_mode ? result / divisor : floor(result / divisor);
        } else {
            break;
        }
    }
    return result;
}

// Функция обработки сложения и вычитания
double parse_expression() {
    double result = parse_term();

    while (1) {
        while (isspace(expr[pos])) pos++;

        if (expr[pos] == '+') {
            pos++;
            result += parse_term();
        } else if (expr[pos] == '-') {
            pos++;
            result -= parse_term();
        } else {
            break;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    static char input[MAX_LEN];

    // Проверка флага --float
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--float") == 0) {
            float_mode = 1;
        }
    }

    if (!fgets(input, MAX_LEN, stdin)) {
        return 1; // Ошибка ввода
    }

    validate_input(input); // Проверка разрешенных символов

    expr = input;
    pos = 0;

    double result = parse_expression();

    if (float_mode) {
        printf("%.4f\n", result); // Вывод результата в режиме вещественных чисел
    } else {
        printf("%d\n", (int) result); // Вывод результата в режиме целых чисел
    }

    return 0;
}
