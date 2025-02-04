#include "utils.h"

/**
 * \brief Перевести массив байт в uint16_t
 * \param bytes указатель на массив байт
 * \return uint16_t число
 */
uint16_t bytes_to_uint16(const uint8_t bytes[2]) {
    if (bytes == NULL) {
        return 0;
    }

    uint16_t result = 0;

    for (int i = 0; i < 2; i++) {
        result = (result << 8) | bytes[i];
    }

    return result;
}

/**
 * \brief Перевести массив байт в uint32_t
 * \param bytes указатель на массив байт
 * \return uint32_t число
 */
uint32_t bytes_to_uint32(const uint8_t bytes[4]) {
    if (bytes == NULL) {
        return 0;
    }

    uint32_t result = 0;

    for (int i = 0; i < 4; i++) {
        result = (result << 8) | bytes[i];
    }

    return result;
}

/**
 * \brief Перевести массив байт в uint64_t
 * \param bytes указатель на массив байт
 * \return uint64_t число
 */
uint64_t bytes_to_uint64(const uint8_t bytes[8]) {
    if (bytes == NULL) {
        return 0;
    }

    uint64_t result = 0;

    for (int i = 0; i < 8; i++) {
        result = (result << 8) | bytes[i];
    }

    return result;
}

/**
 * \brief Перевести число uint16_t в массив байт
 * \param value uint16_t число
 * \param bytes указатель на массив байт
 */
void uint16_to_bytes(uint16_t value, uint8_t *bytes) {
    do {
        if (bytes == NULL) {
            break;
        }

        uint8_t digits = 8;

        for (int i = 0; i < 2; i++) {
            bytes[i] = (value >> digits) & 0xFF;
            digits -= 8;
        }
    } while (0);

}

/**
 * \brief Перевести число uint32_t в массив байт
 * \param value uint32_t число
 * \param bytes указатель на массив байт
 */
void uint32_to_bytes(uint32_t value, uint8_t *bytes) {
    do {
        if (bytes == NULL) {
            break;
        }

        uint8_t digits = 24;

        for (int i = 0; i < 4; i++) {
            bytes[i] = (value >> digits) & 0xFF;
            digits -= 8;
        }
    } while (0);
}

/**
 * \brief Перевести число uint64_t в массив байт
 * \param value uint64_t число
 * \param bytes указатель на массив байт
 */
void uint64_to_bytes(uint64_t value, uint8_t *bytes) {
    do {
        if (bytes == NULL) {
            break;
        }

        uint8_t digits = 56;

        for (int i = 0; i < 8; i++) {
            bytes[i] = (value >> digits) & 0xFF;
            digits -= 8;
        }
    } while (0);
}

/**
 * \brief Изменить порядок байт в uint16_t числе
 * \param value число
 * \return число с измененным порядком байт
 */
uint16_t swap_endian16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

/**
 * \brief Изменить порядок байт в uint32_t числе
 * \param value число
 * \return число с измененным порядком байт
 */
uint32_t swap_endian32(uint32_t value) {
    return ((value >> 24) & 0xFF) | // Старший байт -> младший
           ((value >> 8) & 0xFF00) | // Второй байт -> третий
           ((value << 8) & 0xFF0000) | // Третий байт -> второй
           ((value << 24) & 0xFF000000); // Младший байт -> старший
}

/**
 * \brief Изменить порядок байт в uint64_t числе
 * \param value число
 * \return число с измененным порядком байт
 */
uint64_t swap_endian64(uint64_t value) {
    return ((value >> 56) & 0xFF) | // Старший байт -> младший
           ((value >> 40) & 0xFF00) | // Второй байт -> предпоследний
           ((value >> 24) & 0xFF0000) | // Третий байт -> третий с конца
           ((value >> 8) & 0xFF000000) | // Четвёртый байт -> четвёртый с конца
           ((value << 8) & 0xFF00000000) | // Четвёртый с конца -> четвёртый
           ((value << 24) & 0xFF0000000000) | // Третий с конца -> третий
           ((value << 40) & 0xFF000000000000) | // Предпоследний -> второй
           ((value << 56) & 0xFF00000000000000); // Младший -> старший
}

/**
 * \brief Вычислить контрольную сумму для массива, по алгоритму CRC-16/KERMIT
 * \param data указатель на массив
 * \param length размер массива
 * \return контрольная сумма
 */
uint16_t crc16_kermit(const uint8_t *data, size_t length) {
    if (data == NULL) {
        return 0;
    }

    uint16_t crc = 0x0000;

    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0x8408;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

/**
 * \brief Проверка на переполнение при сложение двух uint64_t чисел
 * \param a число 1
 * \param b число 2
 * \return 1: при сложении не возникнет переполнения; 0: при сложении возникнет переполнение
 */
uint8_t uint64_overflow_in_add_check(uint64_t a, uint64_t b) {
    if (a > UINT64_MAX - b) {
        return 0;
    } else {
        return 1;
    }
}
