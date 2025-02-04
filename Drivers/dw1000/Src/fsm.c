#include "fsm.h"

uint8_t
anchor_loop(void) {
    terminal_print("Device type: ANCHOR\r\n", strlen("Device type: ANCHOR\r\n"));

    uint8_t tx_buffer[TX_RX_BUFFER_SIZE] = {0};
    size_t tx_buffer_size = 0;

    uint8_t rx_buffer[TX_RX_BUFFER_SIZE] = {0};
    size_t rx_buffer_size = 0;

    uint8_t sequence_number = 0;

    anchor_fsm_state_t anchor_fsm_state = ANCHOR_FSM_IDLE;

    uint64_t poll_rx_ts, response_tx_time, response_tx_ts, final_rx_ts;
    double tof, distance;

    while (1) {
        switch (anchor_fsm_state) {
            case ANCHOR_FSM_IDLE: {
                anchor_fsm_state = ANCHOR_FSM_WAIT_BLINK;
                break;
            };
            case ANCHOR_FSM_WAIT_BLINK: {
                anchor_fsm_state = ANCHOR_FSM_SEND_RANGING_INIT;
                break;
            };
            case ANCHOR_FSM_SEND_RANGING_INIT: {
                anchor_fsm_state = ANCHOR_FSM_WAIT_POLL;
                break;
            };
            case ANCHOR_FSM_WAIT_POLL: {
                if (uwb_receive_data(FALSE, rx_buffer, &rx_buffer_size, RESPONSE_RX_TIMEOUT_UUS) == FALSE) {
                    anchor_fsm_state = ANCHOR_FSM_IDLE;
                    break;
                }

                if (poll_frame_parser(rx_buffer, rx_buffer_size) == FALSE) {
                    anchor_fsm_state = ANCHOR_FSM_IDLE;
                    break;
                }

                poll_rx_ts = get_rx_timestamp_u64();

                anchor_fsm_state = ANCHOR_FSM_SEND_RESPONSE;
                break;
            };
            case ANCHOR_FSM_SEND_RESPONSE: {
                global_response_frame.frame_control = POLL_RESPONSE_FINAL_FRAME_CONTROL;
                global_response_frame.sequence_number = sequence_number;
                global_response_frame.pan_id = PAN_ID;
                global_response_frame.destination_address = 1;
                global_response_frame.source_address = 2;
                global_response_frame.function_code = RESPONSE_FUNCTION_CODE;
                global_response_frame.calculated_time_of_flight = 0;

                response_frame_builder(tx_buffer, &tx_buffer_size);

                response_tx_time = (poll_rx_ts + (POLL_RX_TO_RESPONSE_TX_DLY_UUS * UUS_TO_DWU)) >> 8;

                if (uwb_transmit_data(tx_buffer, tx_buffer_size, response_tx_time, RESPONSE_TX_TO_FINAL_RX_DLY_UUS,
                                      TRUE) == FALSE) {
                    anchor_fsm_state = ANCHOR_FSM_IDLE;
                    break;
                }

                anchor_fsm_state = ANCHOR_FSM_WAIT_FINAL;
                break;
            };
            case ANCHOR_FSM_WAIT_FINAL: {
                if (uwb_receive_data(TRUE, rx_buffer, &rx_buffer_size, FINAL_RX_TIMEOUT_UUS) == FALSE) {
                    anchor_fsm_state = ANCHOR_FSM_IDLE;
                    break;
                }

                if (final_frame_parser(rx_buffer, rx_buffer_size) == FALSE) {
                    anchor_fsm_state = ANCHOR_FSM_IDLE;
                    break;
                }

                double Ra, Rb, Da, Db;
                int64 tof_dtu;

                response_tx_ts = get_tx_timestamp_u64();
                final_rx_ts = get_rx_timestamp_u64();

                Ra = (double) (global_final_frame.resp_rx_time_minus_poll_tx_time);
                Rb = (double) (final_rx_ts - response_tx_ts);
                Da = (double) (global_final_frame.final_tx_time_minus_resp_rx_time);
                Db = (double) (response_tx_ts - poll_rx_ts);
                tof_dtu = (int64) ((Ra * Rb - Da * Db) / (Ra + Rb + Da + Db));

                tof = tof_dtu * DWT_TIME_UNITS;
                distance = tof * SPEED_OF_LIGHT;

                char dist_str[64] = {0};

                static uint8_t anc_i = 0;
                static double sum = 0.0;

                if (anc_i < 100) {
                    sum += distance;

//                    if (sum > 200) {
//                        sum = 0.0;
//                        anc_i = 0;
//                    } else if (sum < 0) {
//                        sum = 0.0;
//                        anc_i = 0;
//                    }

                    anc_i++;
                } else {
                    sprintf(dist_str, "DIST: %4.4f m\r\n", sum / 100.0);
                    terminal_print(dist_str, strlen(dist_str));

                    //calibrate_anchor_antenna_delay(sum / 100.0);
                    sum = 0.0;
                    anc_i = 0;
                }

                anchor_fsm_state = ANCHOR_FSM_CALCULATE_TOF;
                break;
            };
            case ANCHOR_FSM_CALCULATE_TOF: {
                anchor_fsm_state = ANCHOR_FSM_IDLE;
                break;
            }
            default: {
                anchor_fsm_state = ANCHOR_FSM_IDLE;
                break;
            }
        }
    }
}

uint8_t
tag_loop(void) {
    terminal_print("Device type: TAG\r\n", strlen("Device type: TAG\r\n"));

    uint8_t tx_buffer[TX_RX_BUFFER_SIZE] = {0};
    size_t tx_buffer_size = 0;

    uint8_t rx_buffer[TX_RX_BUFFER_SIZE] = {0};
    size_t rx_buffer_size = 0;

    uint8_t sequence_number = 0;

    uint64_t poll_tx_ts, response_rx_ts, final_tx_time, final_tx_ts;

    tag_fsm_state_t tag_fmt_state = TAG_FSM_IDLE;

    while (1) {
        switch (tag_fmt_state) {
            case TAG_FSM_IDLE: {
                tag_fmt_state = TAG_FSM_SEND_BLINK;
                break;
            };
            case TAG_FSM_SEND_BLINK: {
                tag_fmt_state = TAG_FSM_WAIT_RANGING_INIT;
                break;
            };
            case TAG_FSM_WAIT_RANGING_INIT: {
                tag_fmt_state = TAG_FSM_SEND_POLL;
                break;
            };
            case TAG_FSM_SEND_POLL: {
                global_poll_frame.frame_control = POLL_RESPONSE_FINAL_FRAME_CONTROL;
                global_poll_frame.sequence_number = sequence_number;
                global_poll_frame.pan_id = PAN_ID;
                global_poll_frame.destination_address = 1;
                global_poll_frame.source_address = 0;
                global_poll_frame.function_code = POLL_FUNCTION_CODE;

                poll_frame_builder(tx_buffer, &tx_buffer_size);

                if (uwb_transmit_data(tx_buffer, tx_buffer_size, 0, POLL_TX_TO_RESPONSE_RX_DLY_UUS, TRUE) == FALSE) {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                tag_fmt_state = TAG_FSM_WAIT_RESPONSE;
                break;
            };
            case TAG_FSM_WAIT_RESPONSE: {
                if (uwb_receive_data(TRUE, rx_buffer, &rx_buffer_size, RESPONSE_RX_TIMEOUT_UUS) == FALSE) {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                if (response_frame_parser(rx_buffer, rx_buffer_size) == FALSE) {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                tag_fmt_state = TAG_FSM_SEND_FINAL;
                break;
            };
            case TAG_FSM_SEND_FINAL: {
                poll_tx_ts = get_tx_timestamp_u64();
                response_rx_ts = get_rx_timestamp_u64();

                final_tx_time = (response_rx_ts + (RESPONSE_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWU)) >> 8;
                final_tx_ts = (((uint64) (final_tx_time & 0xFFFFFFFEUL)) << 8) + TAG_TX_ANT_DLY_DTU;

                global_final_frame.frame_control = POLL_RESPONSE_FINAL_FRAME_CONTROL;
                global_final_frame.sequence_number = sequence_number;
                global_final_frame.pan_id = PAN_ID;
                global_final_frame.destination_address = 1;
                global_final_frame.source_address = 2;
                global_final_frame.function_code = FINAL_FUNCTION_CODE;

                if (response_rx_ts > poll_tx_ts) {
                    global_final_frame.resp_rx_time_minus_poll_tx_time = response_rx_ts - poll_tx_ts;
                } else {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                if (final_tx_ts > response_rx_ts) {
                    global_final_frame.final_tx_time_minus_resp_rx_time = final_tx_ts - response_rx_ts;
                } else {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                final_frame_builder(tx_buffer, &tx_buffer_size);

                if (uwb_transmit_data(tx_buffer, tx_buffer_size, final_tx_time, 0, FALSE) == FALSE) {
                    tag_fmt_state = TAG_FSM_IDLE;
                    break;
                }

                tag_fmt_state = TAG_FSM_IDLE;
                break;
            };
            default: {
                tag_fmt_state = TAG_FSM_IDLE;
                break;
            };
        }
    }
}
