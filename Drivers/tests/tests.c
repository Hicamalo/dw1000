// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "tests.h"

/**
 * \brief Функция для проверки интерфейса связи с DW1000 (SPI)
 * \return TRUE: Тест пройден, FALSE: Тест провален
 */
uint8_t
test_spi_dw1000(void) {
    uint8_t data_a[20] = "spi_test";
    uint8_t data_b[20] = {0};

    dwt_writetodevice(0x21, 0, 20, &data_a[0]);
    dwt_readfromdevice(0x21, 0, 20, &data_b[0]);

    if (strcmp(data_a, data_b) == 0) {
        return TRUE;
    }

    return FALSE;
}

/**
 * \brief Функция для проверки ID DW1000
 * \return TRUE: Тест пройден, FALSE: Тест провален
 */
uint8_t
test_check_id_dw1000(void) {
    reset_DW1000();
    port_set_dw1000_slowrate();
    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR) {
        return FALSE;
    }
    port_set_dw1000_fastrate();

    uint32_t dev_id = dwt_readdevid();

    uint32_t dw1000_dev_id = 0xDECA0130;

    if (dev_id == dw1000_dev_id) {
        return TRUE;
    }
    

    return FALSE;
}

