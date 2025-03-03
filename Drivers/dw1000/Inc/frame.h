/**
 * \file            frame.h
 * \brief           Функции для упаковки/распаковки кадров UWB
 */
#ifndef TEST_DW1000_FRAME_H
#define TEST_DW1000_FRAME_H

#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "port.h"

#define BLINK_FRAME_CONTROL 0xC5
#define RANGING_INIT_FRAME_CONTROL 0xCC41
#define POLL_RESPONSE_FINAL_FRAME_CONTROL 0x8841

#define PAN_ID 0xDECA

#define RANGING_INIT_FUNCTION_CODE 0x20
#define POLL_FUNCTION_CODE 0x61
#define RESPONSE_FUNCTION_CODE 0x50
#define FINAL_FUNCTION_CODE 0x69

#define BLINK_FRAME_SIZE 12
#define RANGING_INIT_MSG_SIZE 22
#define POLL_FRAME_SIZE 12
#define RESPONSE_FRAME_SIZE 20
#define FINAL_FRAME_SIZE 28

/**
 * \brief Структура сообщения "Blink"
 * \note 12 байт
 */
typedef struct {
    uint8_t frame_control; /* 0xC5 */
    uint8_t sequence_number;
    uint64_t tag_id;
    uint16_t frame_checksum; /* dw1000 добавляет автоматически */
} blink_t;

/**
 * \brief Структура сообщения "Ranging Init"
 * \note 22 байта
 */
typedef struct {
    uint16_t frame_control; /* 0xСС41 */
    uint8_t sequence_number;
    uint16_t pan_id; /* 0xDECA */
    uint64_t destination_address;
    uint16_t source_address;
    uint8_t function_code; /* 0x20 */
    uint16_t tag_shot_address;
    uint16_t response_delay;
    uint16_t frame_checksum; /* dw1000 добавляет автоматически */
} ranging_init_msg_t;

/**
 * \brief Структура сообщения "Poll"
 * \note 12 байт
 */
typedef struct {
    uint16_t frame_control; /* 0x8841 */
    uint8_t sequence_number;
    uint16_t pan_id; /* 0xDECA */
    uint16_t destination_address;
    uint16_t source_address;
    uint8_t function_code;   /* 0x61 */
    uint16_t frame_checksum; /* dw1000 добавляет автоматически */
} poll_t;

/**
 * \brief Структура сообщения "Response"
 * \note 20 байт
 */
typedef struct {
    uint16_t frame_control; /* 0x8841 */
    uint8_t sequence_number;
    uint16_t pan_id; /* 0xDECA */
    uint16_t destination_address;
    uint16_t source_address;
    uint8_t function_code; /* 0x50 */
    uint64_t calculated_time_of_flight;
    uint16_t frame_checksum; /* dw1000 добавляет автоматически */
} response_t;

/**
 * \brief Структура сообщения "Final"
 * \note 28 байт
 */
typedef struct {
    uint16_t frame_control; /* 0x8841 */
    uint8_t sequence_number;
    uint16_t pan_id; /* 0xDECA */
    uint16_t destination_address;
    uint16_t source_address;
    uint8_t function_code; /* 0x69 */
    uint64_t resp_rx_time_minus_poll_tx_time;
    uint64_t final_tx_time_minus_resp_rx_time;
    uint16_t frame_checksum; /* dw1000 добавляет автоматически */
} final_t;

extern blink_t global_blink_frame;
extern ranging_init_msg_t global_ranging_init_msg;
extern poll_t global_poll_frame;
extern response_t global_response_frame;
extern final_t global_final_frame;

extern uint8_t source_address;
extern uint8_t designation_address;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t
blink_frame_parser(const uint8_t *data, size_t size);

uint8_t
ranging_init_frame_parser(const uint8_t *data, size_t size);

uint8_t
poll_frame_parser(const uint8_t *data, size_t size);

uint8_t
response_frame_parser(const uint8_t *data, size_t size);

uint8_t
final_frame_parser(const uint8_t *data, size_t size);

uint8_t
blink_frame_builder(uint8_t *data, size_t *size);

uint8_t
ranging_init_msg_builder(uint8_t *data, size_t *size);

uint8_t
poll_frame_builder(uint8_t *data, size_t *size);

uint8_t
response_frame_builder(uint8_t *data, size_t *size);

uint8_t
final_frame_builder(uint8_t *data, size_t *size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //TEST_DW1000_FRAME_H
