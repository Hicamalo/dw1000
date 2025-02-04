/**
 * \file            calibration.h
 * \brief           Функции для калибровки DW1000
 */
#ifndef TEST_DW1000_CALIBRATION_H
#define TEST_DW1000_CALIBRATION_H

#include <stdint.h>
#include "stdio.h"
#include "terminal.h"
#include "deca_device_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void
calibrate_anchor_antenna_delay(double calculated_distance);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //TEST_DW1000_CALIBRATION_H
