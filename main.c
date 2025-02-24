
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 1024

char *expr;
int pos = 0;

int parse_factor(); // Объявляем функции парсера
int parse_term();
int parse_expression();

// Функция для обработки чисел и выражений в скобках
int parse_factor() {
    while (isspace(expr[pos])) pos++; // Пропускаем пробельные символы
    
    if (expr[pos] == '(') { // Если начинается скобка, обрабатываем подвыражение
        pos++;
        int result = parse_expression();
        if (expr[pos] == ')') pos++; // Закрывающая скобка
        return result;
    }
    
    int num = 0;
    while (isdigit(expr[pos])) { // Читаем число
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    return num;
}

// Функция для обработки умножения и деления
int parse_term() {
    int result = parse_factor();
    
    while (1) {
        while (isspace(expr[pos])) pos++; // Пропускаем пробелы

        if (expr[pos] == '*') {
            pos++;
            result *= parse_factor();
        } else if (expr[pos] == '/') {
            pos++;
            int divisor = parse_factor();
            if (divisor != 0) {
                result /= divisor;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
        } else {
            break;
        }
    }
    return result;
}

// Функция для обработки сложения и вычитания
int parse_expression() {
    int result = parse_term();
    
    while (1) {
        while (isspace(expr[pos])) pos++; // Пропускаем пробелы

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

int main() {
    static char input[MAX_LEN];

    if (!fgets(input, MAX_LEN, stdin)) {
        return 1; // Ошибка чтения ввода
    }

    expr = input;
    pos = 0;

    int result = parse_expression();
    printf("%d\n", result); // Вывод только числа

    return 0;
}
