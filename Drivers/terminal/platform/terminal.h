/**
 * \file            terminal.h
 * \brief           Функции для работы с терминалом
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <string.h>
#include "lwshell.h"
#include "terminal_strings.h"
#include "usart.h"

// Максимальное количество входных символов в терминале
#define MAX_TERMINAL_INPUT_BUFFER_SIZE (64)

// Красный цвет текста в терминале
#define TERMINAL_RED_TEXT_COLOR        "\033[0;31m"
// Сброс цвета текста в терминале
#define TERMINAL_RESET_TEXT_COLOR      "\033[0m"
// Команда очистки дисплея терминала
#define TERMINAL_DISPLAY_CLEAR         "\033[2J"
// Команда возврата коретки в терминале
#define TERMINAL_RESET_CURSOR          "\033[H"

// Шаблон структуры с настройками терминала
typedef struct {
    bool terminal_echo;
} terminal_settings_t;

typedef enum {
    TERMINAL_DRIVER_OK = 0x00,
    TERMINAL_DRIVER_ERROR = 0x01,
    TERMINAL_DRIVER_BUSY = 0x02,
    TERMINAL_DRIVER_TIMEOUT = 0x03,
    TERMINAL_DRIVER_NULL_ARGUMENT = 0x04,
} terminal_driver_status_t;

// Структура с настройками терминала
extern terminal_settings_t terminal_settings;

// Входной буфер терминала
extern char terminal_input_buffer[MAX_TERMINAL_INPUT_BUFFER_SIZE];

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

terminal_driver_status_t terminal_print(uint8_t *str, uint16_t len);

terminal_driver_status_t terminal_scan(uint8_t *str, uint16_t max_len);

long int terminal_echo(long int argc, char **argv);

long int terminal_clear(long int argc, char **argv);

long int terminal_test(long int argc, char **argv);

void terminal_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //TERMINAL_H
