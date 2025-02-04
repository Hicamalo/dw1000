#include "calibration.h"

uint16_t antenna_delay = 32966; // Начальная задержка антенны
uint16_t antenna_delay_delta = 10000; // Начальный сдвиг задержки
double last_delta = 0.0;
const double fix_anchor_distance = 1.0; // Известное расстояние между модулями в метрах

char cal_string[64]; // Буфер для вывода строки в консоль

/**
 * \brief Функция итеративно осуществляет калибровку значения задержки антенны anchor модуля, при помощи алгоритма бинарного поиска
 * \param calculated_distance измеренное значение расстояния между anchor и tag в метрах
 * \note Для работы функции необходимо:
 *       1. Взять 1 anchor модуль и 1 tag модуль, разместить на заранее известном расстоянии и записать это расстояние в константу fix_anchor_distance.
 *       2. Выставить значение задержки антенны RX и TX для tag модуля = 1.
 *       3. Подключить к anchor модуль к компьютеру при помощи переходника UART-USB и открыть терминал COM портов.
 *       Функция выдаст одинаковую общую задержку для RX и TX и только для anchor модуля.
 *       Функция покажет итоговую задержку в терминале и заблокирует дальнейшую работу программы
 */
void
calibrate_anchor_antenna_delay(double calculated_distance) {
    if (antenna_delay_delta < 3) {
        sprintf(cal_string, "Final anchor antenna delay = %hu\r\n", antenna_delay);
        terminal_print(cal_string, strlen(cal_string));
        while (1) {};
    }

    double this_delta = calculated_distance - fix_anchor_distance;

    if (this_delta * last_delta < 0.0) {
        antenna_delay_delta = (antenna_delay_delta > 2) ? antenna_delay_delta / 2 : 1;
    }

    last_delta = this_delta;

    if (this_delta > 0.0) {
        antenna_delay += antenna_delay_delta;
    } else {
        antenna_delay -= antenna_delay_delta;
    }

    sprintf(cal_string, "%hu\r\n", antenna_delay);
    terminal_print(cal_string, strlen(cal_string));

    dwt_setrxantennadelay(antenna_delay);
    dwt_settxantennadelay(antenna_delay);
}
