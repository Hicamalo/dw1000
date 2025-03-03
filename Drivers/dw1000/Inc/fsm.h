/**
 * \file            dw1000.h
 * \brief           Конечные автоматы Tag и Anchor DW1000
 */
#ifndef TEST_DW1000_FSM_H
#define TEST_DW1000_FSM_H

#include <stdint.h>
#include <math.h>
#include "deca_device_api.h"
#include "terminal.h"
#include "deca_regs.h"
#include "dw1000.h"
#include "frame.h"
#include "calibration.h"

#define US_TO_UUS (449.2 / 512) /* 1 uus (UWB microsecond) = 512 / 499.2 us. Уможаем микросекунды, чтобы перевести в микросекунды UWB */

#define US_TO_DTU (499.2 * 128) /* 1 dtu (device time unit) = 128 * 499.2 us. Уможаем микросекунды, чтобы перевести в dtu */


#define UUS_TO_DWU (65536) /* Умножаем UUS, чтобы перевести в dtu */
#define SPEED_OF_LIGHT 299702547 /* Скорость света в воздухе, в метрах в секунду */

#define POLL_TX_TO_RESPONSE_RX_DLY_UUS 100 /* Задержка перед переключением режима работы из TX в RX, в момент передачи Poll и получения Response */

#define RESPONSE_RX_TO_FINAL_TX_DLY_UUS 4500 /* Задержка перед переключением режима работы из RX в TX, в момент получения Response и передачи Final */


#define RESPONSE_RX_TIMEOUT_UUS 10000 /* Таймаут ожидания Response */

#define POLL_RX_TO_RESPONSE_TX_DLY_UUS 4500 /* Задержка перед переключением режима работы из RX в TX, в момент получения Poll и передачи Response */

#define RESPONSE_TX_TO_FINAL_RX_DLY_UUS 100 /* Задержка перед переключением режима работы из TX в RX, в момент передачи Response и получения Final */


#define FINAL_RX_TIMEOUT_UUS 10000 /* Таймаут ожидания Final */

#define TX_RX_BUFFER_SIZE 32

/**
 * \brief ENUM, описывающий состояния якоря (anchor)
 */
typedef enum {
    ANCHOR_FSM_IDLE = 0,
    ANCHOR_FSM_WAIT_BLINK = 1,
    ANCHOR_FSM_SEND_RANGING_INIT = 2,
    ANCHOR_FSM_WAIT_POLL = 3,
    ANCHOR_FSM_SEND_RESPONSE = 4,
    ANCHOR_FSM_WAIT_FINAL = 5,
    ANCHOR_FSM_CALCULATE_TOF = 6,
    ANCHOR_FSM_SEND_DIST_TO_TAG = 7,
} anchor_fsm_state_t;

/**
 * \brief ENUM, описывающий состояния тега (tag)
 */
typedef enum {
    TAG_FSM_IDLE = 0,
    TAG_FSM_SEND_BLINK = 1,
    TAG_FSM_WAIT_RANGING_INIT = 2,
    TAG_FSM_SEND_POLL = 3,
    TAG_FSM_WAIT_RESPONSE = 4,
    TAG_FSM_SEND_FINAL = 5,
    TAG_FSM_WAIT_DIST_FROM_TAG = 6,
} tag_fsm_state_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint8_t
anchor_loop(void);

uint8_t
tag_loop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //TEST_DW1000_FSM_H
