/**
 * \file            utils.h
 * \brief           Функции для которых не требуется API драйвера UWB
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint16_t bytes_to_uint16(const uint8_t bytes[2]);

uint32_t bytes_to_uint32(const uint8_t bytes[4]);

uint64_t bytes_to_uint64(const uint8_t bytes[8]);

void uint16_to_bytes(uint16_t value, uint8_t* bytes);

void uint32_to_bytes(uint32_t value, uint8_t* bytes);

void uint64_to_bytes(uint64_t value, uint8_t* bytes);

uint16_t swap_endian16(uint16_t value);

uint32_t swap_endian32(uint32_t value);

uint64_t swap_endian64(uint64_t value);

uint16_t crc16_kermit(const uint8_t* data, size_t length);

uint8_t uint64_overflow_in_add_check(uint64_t a, uint64_t b);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //UTILS_H
