#include "dw1000.h"

/**
 * \brief Структура с настройками радиоканала
 */
static dwt_config_t config =
        {
                2,                   /* Номер канала */
                DWT_PRF_64M,         /* PRF - Pulse repetition frequency. */
                DWT_PLEN_2048,       /* Длина преамбулы */
                DWT_PAC64,           /* Preamble acquisition chunk size */
                9,                   /* Код преамбулы TX */
                9,                   /* Код преамбулы RX */
                1,                   /* Использовать нестандартный SFD */
                DWT_BR_110K,         /* Скорость передачи данных */
                DWT_PHRMODE_STD,     /* PHY header mode */
                (2048 + 1 + 64 - 64) /* Таймаут SFD (preamble length + 1 + SFD length - PAC size) */
        };

/**
 * \brief Получение временной метки TX в 64-битной переменной. Эта функция предполагает, что длина временных меток равна 40 битам, как для TX, так и для RX!
 * \return 64-битное значение считанной временной метки.
 */
uint64
get_tx_timestamp_u64(void) {
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/**
 * \brief Получение временной метки RX в 64-битной переменной. Эта функция предполагает, что длина временных меток равна 40 битам, как для TX, так и для RX!
 * \return 64-битное значение считанной временной метки.
 */
uint64
get_rx_timestamp_u64(void) {
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/**
 * \brief Получение временной метки системного времени. Эта функция предполагает, что длина временных меток равна 40 битам
 * \return 64-битное значение считанной временной метки.
 */
uint64
get_system_time_u64(void) {
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readsystime(ts_tab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/**
 * \brief Функция для инициализации DW1000
 * \return TRUE: инициализация успешна
 */
uint8_t
initialise_dw1000(void) {
    reset_DW1000();

    port_set_dw1000_slowrate();
    if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR) {
        terminal_print("Initialise dw1000 failed\r\n", strlen("Initialise dw1000 failed\r\n"));
        while (1) {};
    }
    port_set_dw1000_fastrate();

    return TRUE;
}

/**
 * \brief Функция для конфигурирования DW1000
 * \return TRUE: конфигурирование успешно
 */
uint8_t
configure_dw1000(void) {
    dwt_configure(&config);
    dwt_setleds(1); /* Включить системные светодиоды (RX - красный, TX - синий) */

    /* Возможно придется восстанавливать предыдущий panid, так как у tag и anchor он разный */
    //dwt_setpanid(0);
    //dwt_enableframefilter(DWT_FF_DATA_EN);

    return TRUE;
}

/**
 * \brief Функция выполняющая прием данных через радиоканал UWB
 * \param rx_started режим RX уже включен? (TRUE: да, FALSE: нет)
 * \param data указатель на массив для приема данных
 * \param data_size  указатель на размер массива
 * \param rx_delay задержка перед включением режима RX, если rx_started = FALSE (0 - без задержки)
 * \param rx_timeout таймаут ожидания приема (0 - без ожидания)
 * \return TRUE: данные получены успешно, FALSE: при приеме данных возникла ошибка
 */
uint8_t
uwb_receive_data(uint8_t rx_started, uint8_t *data, size_t *data_size, uint32_t rx_delay, uint32_t rx_timeout) {
    assert_param(data != NULL);
    assert_param(data_size != NULL);

    if (rx_timeout != 0) {
        dwt_setrxtimeout(rx_timeout);
    }

    if (rx_started == FALSE) {

        if (rx_delay == 0) {
            if (dwt_rxenable(DWT_START_RX_IMMEDIATE) != DWT_SUCCESS) {
                return FALSE;
            }
        } else {
            dwt_setdelayedtrxtime(rx_delay);

            if (dwt_rxenable(DWT_START_RX_DELAYED) != DWT_SUCCESS) {
                return FALSE;
            }
        }

    }

    uint8_t result = FALSE;
    uint32_t status_reg = 0;

    while (1) {
        status_reg = dwt_read32bitreg(SYS_STATUS_ID);

        if (status_reg & SYS_STATUS_RXFCG) {
            /* Прием успешен */
            break;
        }

        if (status_reg & SYS_STATUS_RXPTO) {
            /* Preamble detection timeout */
            break;
        }

        if (status_reg & SYS_STATUS_RXSFDTO) {
            /* Receive SFD timeout */
            break;
        }

        if (status_reg & SYS_STATUS_RXRFTO) {
            /* Receive Frame Wait Timeout */
            break;
        }

        if (status_reg & SYS_STATUS_RXPHE) {
            /* Receiver PHY Header Error */
            break;
        }

        if (status_reg & SYS_STATUS_RXFCE) {
            /* Receiver FCS Error */
            break;
        }

        if (status_reg & SYS_STATUS_RXRFSL) {
            /* Receiver Reed Solomon Frame Sync Loss */
            break;
        }

        if (status_reg & SYS_STATUS_AFFREJ) {
            /* Automatic Frame Filtering rejection */
            break;
        }

        if (status_reg & SYS_STATUS_LDEERR) {
            /* Leading edge detection processing error */
            break;
        }
    }

    if (status_reg & SYS_STATUS_RXFCG) {
        *data_size = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;

        dwt_readrxdata(data, *data_size, 0);

        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

        result = TRUE;
    } else {
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

        dwt_rxreset();

        result = FALSE;
    }

    return result;
}

/**
 * \brief Функция для передачи данных через радиоканал UWB
 * \param data указатель на данные для передачи
 * \param data_size размер данных для передачи
 * \param tx_delay задержка перед передачей данных (0 - без задержки)
 * \param rx_delay задержка перед включением режима RX, после передачи данных (0 - без задержки)
 * \param response_expected включить режим RX, после передачи данных? (TRUE: да, FALSE: нет)
 * \return TRUE: данные получены успешно, FALSE: при приеме данных возникла ошибка
 */
uint8_t
uwb_transmit_data(uint8_t *data, size_t data_size, uint32_t tx_delay, uint32_t rx_delay,
                  uint8_t response_expected) {
    assert_param(data != NULL);

    uint8_t result = FALSE;
    uint8_t tx_mode = 0;

    if (response_expected == TRUE) {
        dwt_setrxaftertxdelay(rx_delay);
    }

    do {
        if (dwt_writetxdata(data_size, data, 0) != DWT_SUCCESS) {
            break;
        }

        dwt_writetxfctrl(data_size, 0, 0);

        if (tx_delay > 0) {
            dwt_setdelayedtrxtime(tx_delay);
            tx_mode |= DWT_START_TX_DELAYED;
        } else {
            tx_mode |= DWT_START_TX_IMMEDIATE;
        }

        if (response_expected == TRUE) {
            tx_mode |= DWT_RESPONSE_EXPECTED;
        }

        if (dwt_starttx(tx_mode) != DWT_SUCCESS) {
            break;
        }

        result = TRUE;
    } while (0);

    if (result == TRUE) {
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS)) {};

        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
    }

    return result;
}

/**
 * \brief Функция которая позволяет получить одни данные, после передачи других, через радиоканал UWB
 * \param data_to_transfer указатель на массив данных для передачи
 * \param data_to_transfer_size размер данных для передачи
 * \param data_received указатель на массив данных для получения
 * \param data_received_size указатель на размер данных для приема
 * \param tx_delay задержка перед передачей данных (0 - без задержки)
 * \param rx_delay задержка перед включением режима RX, после передачи данных (0 - без задержки)
 * \param rx_timeout таймаут ожидания приема (0 - без ожидания)
 * \return
 */
uint8_t
uwb_receive_data_after_transmit(uint8_t *data_to_transfer, size_t data_to_transfer_size, uint8_t *data_received,
                                size_t *data_received_size, uint32_t tx_delay, uint32_t rx_delay,
                                uint32_t rx_timeout) {
    assert_param(data_to_transfer != NULL);
    assert_param(data_received != NULL);
    assert_param(data_received_size != NULL);

    if (uwb_transmit_data(data_to_transfer, data_to_transfer_size, tx_delay, rx_delay, TRUE) == FALSE) {
        return FALSE;
    }

    return uwb_receive_data(TRUE, data_received, data_received_size, 0, rx_timeout);
}
