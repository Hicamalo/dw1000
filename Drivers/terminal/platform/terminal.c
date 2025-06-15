/**
 * \file            terminal.c
 * \brief           Функции для работы с терминалом
 */

#include "terminal.h"

char terminal_input_buffer[MAX_TERMINAL_INPUT_BUFFER_SIZE] = {0};
volatile uint16_t terminal_input_buffer_size = 0;
volatile uint8_t terminal_rx_done = 0;
terminal_settings_t terminal_settings;

/**
 * \brief Вывод строки в терминал
 * \param str Строка для вывода
 * \param len Размер строки для вывода
 * \return Статус завершения
 */
terminal_driver_status_t
terminal_print(uint8_t* str, uint16_t len) {
    if (str == NULL || len == 0) {
        return TERMINAL_DRIVER_NULL_ARGUMENT;
    }

    HAL_StatusTypeDef hal_status = HAL_UART_Transmit(&huart1, str, len, 100);

    switch (hal_status) {
        case HAL_OK: return TERMINAL_DRIVER_OK;
        case HAL_ERROR: return TERMINAL_DRIVER_ERROR;
        case HAL_BUSY: return TERMINAL_DRIVER_BUSY;
        case HAL_TIMEOUT: return TERMINAL_DRIVER_TIMEOUT;
    }
}

/**
 * \brief Получение строки из терминала
 * \param str Указатель на буфер, в который будут записаны данные
 * \param max_len Максимальное количество ожидаемых элементов
 * \return Статус завершения
 */
terminal_driver_status_t
terminal_scan(uint8_t* str, uint16_t max_len) {

    if (str == NULL || max_len == 0) {
        return TERMINAL_DRIVER_NULL_ARGUMENT;
    }

    HAL_StatusTypeDef hal_status = HAL_UARTEx_ReceiveToIdle_IT(&huart1, str, max_len);

    switch (hal_status) {
        case HAL_OK: return TERMINAL_DRIVER_OK;
        case HAL_ERROR: return TERMINAL_DRIVER_ERROR;
        case HAL_BUSY: return TERMINAL_DRIVER_BUSY;
        case HAL_TIMEOUT: return TERMINAL_DRIVER_TIMEOUT;
    }
}

/**
 * \brief Инициализация настроек терминала
 */
void
terminal_settings_init(void) {
    terminal_settings.terminal_echo = false;
}

/**
 * \brief Функция вывода, которая будет использоваться для печати данных из библиотеки LWSHELL пользователю
 * \param str Строка для вывода
 * \param lwobj LwSHELL объект
 */
void
terminal_function_output(const char* str, struct lwshell* lwobj) {
    terminal_print((uint8_t*)str, strlen(str));
}

/**
 * \brief Функция включения/выключения эхо терминала
 */
long int
terminal_echo(long int argc, char** argv) {
    if (terminal_settings.terminal_echo) {
        terminal_settings.terminal_echo = false;
        terminal_print(TERMINAL_ECHO_OFF, strlen(TERMINAL_ECHO_OFF));
    } else {
        terminal_settings.terminal_echo = true;
        terminal_print(TERMINAL_ECHO_ON, strlen(TERMINAL_ECHO_ON));
    }

    return 0;
}

/**
 * \brief Функция очистки терминала
 */
long int
terminal_clear(long int argc, char** argv) {
    terminal_print(TERMINAL_DISPLAY_CLEAR, strlen(TERMINAL_DISPLAY_CLEAR));
    terminal_print(TERMINAL_RESET_CURSOR, strlen(TERMINAL_RESET_CURSOR));

    return 0;
}

/**
 * \brief Функция выводящая "Terminal test!" в консоль
 */
long int
terminal_test(long int argc, char** argv) {
    terminal_print("Terminal test!\r\n", strlen("Terminal test!\r\n"));

    return 0;
}

/**
 * \brief Функция инициализации терминала
 */
void
terminal_init(void) {
    terminal_settings_init();

    lwshell_init();

    lwshell_set_output_fn_ex(NULL, terminal_function_output);

    // Регистрируем команды, которые должны быть в терминале
    lwshell_register_cmd(TERMINAL_ECHO_CMD, terminal_echo, TERMINAL_ECHO_CMD_DESCRIPTION);
    lwshell_register_cmd(TERMINAL_CLEAR_CMD, terminal_clear, TERMINAL_CLEAR_CMD_DESCRIPTION);
    lwshell_register_cmd(TERMINAL_TEST_CMD, terminal_test, TERMINAL_TEST_CMD_DESCRIPTION);
}
