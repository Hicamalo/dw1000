#include "frame.h"

blink_t global_blink_frame;
ranging_init_msg_t global_ranging_init_msg;
poll_t global_poll_frame;
response_t global_response_frame;
final_t global_final_frame;

/**
 * \brief Функция для преобразования массива данных в структуру Blink
 * \param data указатель на массив данных
 * \param size размер массива
 * \return TRUE: преобразование успешно, FALSE: преобразование неудачно
 */
uint8_t
blink_frame_parser(const uint8_t *data, size_t size) {
    assert_param(data != NULL);

    if (size != BLINK_FRAME_SIZE) {
        return FALSE;
    }

    uint16_t data_crc = bytes_to_uint16(data + 10);
    uint16_t calculated_crc = crc16_kermit(data, size - 2);

    global_blink_frame.frame_control = data[0];
    global_blink_frame.sequence_number = data[1];
    global_blink_frame.tag_id = bytes_to_uint64(data + 3);
    global_blink_frame.frame_checksum = swap_endian16(data_crc);

    if (global_blink_frame.frame_control != BLINK_FRAME_CONTROL) {
        return FALSE;
    }

    if (global_blink_frame.frame_checksum != calculated_crc) {
        return FALSE;
    }

    return TRUE;
}

/**
 * \brief Функция для преобразования массива данных в структуру Ranging Init
 * \param data указатель на массив данных
 * \param size размер массива
 * \return TRUE: преобразование успешно, FALSE: преобразование неудачно
 */
uint8_t
ranging_init_frame_parser(const uint8_t *data, size_t size) {
    assert_param(data != NULL);

    if (size != RANGING_INIT_MSG_SIZE) {
        return FALSE;
    }

    uint16_t data_crc = bytes_to_uint16(data + 20);
    uint16_t calculated_crc = crc16_kermit(data, size - 2);

    global_ranging_init_msg.frame_control = swap_endian16(bytes_to_uint16(data));
    global_ranging_init_msg.sequence_number = data[2];
    global_ranging_init_msg.pan_id = swap_endian16(bytes_to_uint16(data + 3));
    global_ranging_init_msg.destination_address = bytes_to_uint64(data + 5);
    global_ranging_init_msg.source_address = bytes_to_uint16(data + 13);
    global_ranging_init_msg.function_code = data[15];
    global_ranging_init_msg.tag_shot_address = bytes_to_uint16(data + 16);
    global_ranging_init_msg.response_delay = bytes_to_uint16(data + 18);
    global_ranging_init_msg.frame_checksum = swap_endian16(data_crc);

    if (global_ranging_init_msg.frame_control != RANGING_INIT_FRAME_CONTROL) {
        return FALSE;
    }

    if (global_ranging_init_msg.pan_id != PAN_ID) {
        return FALSE;
    }

    if (global_ranging_init_msg.function_code != RANGING_INIT_FUNCTION_CODE) {
        return FALSE;
    }

    if (global_ranging_init_msg.frame_checksum != calculated_crc) {
        return FALSE;
    }

    return TRUE;
}

/**
 * \brief Функция для преобразования массива данных в структуру Poll
 * \param data указатель на массив данных
 * \param size размер массива
 * \return TRUE: преобразование успешно, FALSE: преобразование неудачно
 */
uint8_t
poll_frame_parser(const uint8_t *data, size_t size) {
    assert_param(data != NULL);

    if (size != POLL_FRAME_SIZE) {
        return FALSE;
    }

    uint16_t data_crc = bytes_to_uint16(data + 10);
    uint16_t calculated_crc = crc16_kermit(data, size - 2);

    global_poll_frame.frame_control = swap_endian16(bytes_to_uint16(data));
    global_poll_frame.sequence_number = data[2];
    global_poll_frame.pan_id = swap_endian16(bytes_to_uint16(data + 3));
    global_poll_frame.destination_address = bytes_to_uint16(data + 5);
    global_poll_frame.source_address = bytes_to_uint16(data + 7);
    global_poll_frame.function_code = data[9];
    global_poll_frame.frame_checksum = swap_endian16(data_crc);

    if (global_poll_frame.frame_control != POLL_RESPONSE_FINAL_FRAME_CONTROL) {
        return FALSE;
    }

    if (global_poll_frame.pan_id != PAN_ID) {
        return FALSE;
    }

    if (global_poll_frame.function_code != POLL_FUNCTION_CODE) {
        return FALSE;
    }

    if (global_poll_frame.frame_checksum != calculated_crc) {
        return FALSE;
    }

    return TRUE;
}

/**
 * \brief Функция для преобразования массива данных в структуру Response
 * \param data указатель на массив данных
 * \param size размер массива
 * \return TRUE: преобразование успешно, FALSE: преобразование неудачно
 */
uint8_t
response_frame_parser(const uint8_t *data, size_t size) {
    assert_param(data != NULL);

    if (size != RESPONSE_FRAME_SIZE) {
        return FALSE;
    }

    uint16_t data_crc = bytes_to_uint16(data + 18);
    uint16_t calculated_crc = crc16_kermit(data, size - 2);

    global_response_frame.frame_control = swap_endian16(bytes_to_uint16(data));
    global_response_frame.sequence_number = data[2];
    global_response_frame.pan_id = swap_endian16(bytes_to_uint16(data + 3));
    global_response_frame.destination_address = bytes_to_uint16(data + 5);
    global_response_frame.source_address = bytes_to_uint16(data + 7);
    global_response_frame.function_code = data[9];
    global_response_frame.calculated_time_of_flight = bytes_to_uint64(data + 10);

    global_response_frame.frame_checksum = swap_endian16(data_crc);

    if (global_response_frame.frame_control != POLL_RESPONSE_FINAL_FRAME_CONTROL) {
        return FALSE;
    }

    if (global_response_frame.pan_id != PAN_ID) {
        return FALSE;
    }

    if (global_response_frame.function_code != RESPONSE_FUNCTION_CODE) {
        return FALSE;
    }

    if (global_response_frame.frame_checksum != calculated_crc) {
        return FALSE;
    }

    return TRUE;
}

/**
 * \brief Функция для преобразования массива данных в структуру Final
 * \param data указатель на массив данных
 * \param size размер массива
 * \return TRUE: преобразование успешно, FALSE: преобразование неудачно
 */
uint8_t
final_frame_parser(const uint8_t *data, size_t size) {
    assert_param(data != NULL);

    if (size != FINAL_FRAME_SIZE) {
        return FALSE;
    }

    uint16_t data_crc = bytes_to_uint16(data + 26);
    uint16_t calculated_crc = crc16_kermit(data, size - 2);

    global_final_frame.frame_control = swap_endian16(bytes_to_uint16(data));
    global_final_frame.sequence_number = data[2];
    global_final_frame.pan_id = swap_endian16(bytes_to_uint16(data + 3));
    global_final_frame.destination_address = bytes_to_uint16(data + 5);
    global_final_frame.source_address = bytes_to_uint16(data + 7);
    global_final_frame.function_code = data[9];
    global_final_frame.resp_rx_time_minus_poll_tx_time = bytes_to_uint64(data + 10);
    global_final_frame.final_tx_time_minus_resp_rx_time = bytes_to_uint64(data + 18);

    global_final_frame.frame_checksum = swap_endian16(data_crc);

    if (global_final_frame.frame_control != POLL_RESPONSE_FINAL_FRAME_CONTROL) {
        return FALSE;
    }

    if (global_final_frame.pan_id != PAN_ID) {
        return FALSE;
    }

    if (global_final_frame.function_code != FINAL_FUNCTION_CODE) {
        return FALSE;
    }

    if (global_final_frame.frame_checksum != calculated_crc) {
        return FALSE;
    }

    return TRUE;
}

/**
 * \brief Функция для преобразования структуры Blink в массив данных
 * \param data указатель на массив данных
 * \param size указатель на размер массива
 * \return TRUE: преобразование успешно
 */
uint8_t
blink_frame_builder(uint8_t *data, size_t *size) {
    assert_param(data != NULL);
    assert_param(size != NULL);

    data[0] = global_blink_frame.frame_control;           /* 1 байт */
    data[1] = global_blink_frame.sequence_number;         /* 1 байт */
    uint64_to_bytes(global_blink_frame.tag_id, &data[2]); /* 8 байт */

    *size = BLINK_FRAME_SIZE;

    return TRUE;
}

/**
 * \brief Функция для преобразования структуры Ranging Init в массив данных
 * \param data указатель на массив данных
 * \param size указатель на размер массива
 * \return TRUE: преобразование успешно
 */
uint8_t
ranging_init_msg_builder(uint8_t *data, size_t *size) {
    assert_param(data != NULL);
    assert_param(size != NULL);

    uint16_to_bytes(swap_endian16(global_ranging_init_msg.frame_control), data); /* 2 байта */
    data[2] = global_ranging_init_msg.sequence_number;                           /* 1 байт */
    uint16_to_bytes(swap_endian16(global_ranging_init_msg.pan_id), data + 3);    /* 2 байта */
    uint64_to_bytes(global_ranging_init_msg.destination_address, data + 5);      /* 8 байт */
    uint16_to_bytes(global_ranging_init_msg.source_address, data + 13);          /* 2 байта */
    data[15] = global_ranging_init_msg.function_code;                            /* 1 байт */
    uint16_to_bytes(global_ranging_init_msg.tag_shot_address, data + 16);        /* 2 байта */
    uint16_to_bytes(global_ranging_init_msg.response_delay, data + 18);          /* 2 байта */

    *size = RANGING_INIT_MSG_SIZE;

    return TRUE;
}

/**
 * \brief Функция для преобразования структуры Poll в массив данных
 * \param data указатель на массив данных
 * \param size указатель на размер массива
 * \return TRUE: преобразование успешно
 */
uint8_t
poll_frame_builder(uint8_t *data, size_t *size) {
    assert_param(data != NULL);
    assert_param(size != NULL);

    uint16_to_bytes(swap_endian16(global_poll_frame.frame_control), data); /* 2 байта */
    data[2] = global_poll_frame.sequence_number;                           /* 1 байта */
    uint16_to_bytes(swap_endian16(global_poll_frame.pan_id), data + 3);    /* 2 байта */
    uint16_to_bytes(global_poll_frame.destination_address, data + 5);      /* 2 байта */
    uint16_to_bytes(global_poll_frame.source_address, data + 7);           /* 2 байта */
    data[9] = global_poll_frame.function_code;                             /* 1 байта */

    *size = POLL_FRAME_SIZE;

    return TRUE;
}

/**
 * \brief Функция для преобразования структуры Response в массив данных
 * \param data указатель на массив данных
 * \param size указатель на размер массива
 * \return TRUE: преобразование успешно
 */
uint8_t
response_frame_builder(uint8_t *data, size_t *size) {
    assert_param(data != NULL);
    assert_param(size != NULL);

    uint16_to_bytes(swap_endian16(global_response_frame.frame_control), data);   /* 2 байта */
    data[2] = global_response_frame.sequence_number;                             /* 1 байт */
    uint16_to_bytes(swap_endian16(global_response_frame.pan_id), data + 3);      /* 2 байта */
    uint16_to_bytes(global_response_frame.destination_address, data + 5);        /* 2 байта */
    uint16_to_bytes(global_response_frame.source_address, data + 7);             /* 2 байта */
    data[9] = global_response_frame.function_code;                               /* 1 байт */
    uint64_to_bytes(global_response_frame.calculated_time_of_flight, data + 10); /* 8 байт */

    *size = RESPONSE_FRAME_SIZE;

    return TRUE;
}

/**
 * \brief Функция для преобразования структуры Final в массив данных
 * \param data указатель на массив данных
 * \param size указатель на размер массива
 * \return TRUE: преобразование успешно
 */
uint8_t
final_frame_builder(uint8_t *data, size_t *size) {
    assert_param(data != NULL);
    assert_param(size != NULL);

    uint16_to_bytes(swap_endian16(global_final_frame.frame_control), data);          /* 2 байта */
    data[2] = global_final_frame.sequence_number;                                    /* 1 байт */
    uint16_to_bytes(swap_endian16(global_final_frame.pan_id), data + 3);             /* 2 байта */
    uint16_to_bytes(global_final_frame.destination_address, data + 5);               /* 2 байта */
    uint16_to_bytes(global_final_frame.source_address, data + 7);                    /* 2 байта */
    data[9] = global_final_frame.function_code;                                      /* 1 байт */
    uint64_to_bytes(global_final_frame.resp_rx_time_minus_poll_tx_time, data + 10);  /* 8 байт */
    uint64_to_bytes(global_final_frame.final_tx_time_minus_resp_rx_time, data + 18); /* 8 байт */

    *size = FINAL_FRAME_SIZE;

    return TRUE;
}
