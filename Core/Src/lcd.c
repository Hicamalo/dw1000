/**
* \file            lcd.c
 * \brief           Функции для работы с LCD QAPASS 1602
 * \note
 *
 * The LCD1602 controller is an HD44780-compatible controller that normally operates
 * via an 8-bit or 4-bit wide parallel bus.
 *
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * The LCD1602 controller is connected to a PCF8574A I/O expander via the I2C bus.
 * Only the top four bits are connected to the controller's data lines. The lower
 * four bits are used as control lines:
 *
 *   - B7: data bit 3
 *   - B6: data bit 2
 *   - B5: data bit 1
 *   - B4: data bit 0
 *   - B3: backlight (BL): off = 0, on = 1
 *   - B2: enable (EN): change from 1 to 0 to clock data into controller
 *   - B1: read/write (RW): write = 0, read = 1
 *   - B0: register select (RS): command = 0, data = 1
 *
 * Therefore to send a command byte requires the following operations:
 *
 *   // First nibble:
 *   val = command & 0xf0              // extract top nibble
 *   val |= 0x04                       // RS = 0 (command), RW = 0 (write), EN = 1
 *   i2c_write_byte(i2c_address, val)
 *   sleep(2ms)
 *   val &= 0xfb                       // EN = 0
 *   i2c_write_byte(i2c_address, val)
 *
 *   // Second nibble:
 *   val = command & 0x0f              // extract bottom nibble
 *   val |= 0x04                       // RS = 0 (command), RW = 0 (write), EN = 1
 *   i2c_write_byte(i2c_address, val)
 *   sleep(2ms)
 *   val &= 0xfb                       // EN = 0
 *   i2c_write_byte(i2c_address, val)
 *
 * Sending a data byte is very similar except that RS = 1 (data)
 *
 * When the controller powers up, it defaults to:
 *
 *   - display cleared
 *   - 8-bit interface, 1 line display, 5x8 dots per character
 *   - increment by 1 set
 *   - no shift
 *
 * The controller must be set to 4-bit operation before proper communication can commence.
 * The initialisation sequence for 4-bit operation is:
 *
 *   0. wait > 15ms after Vcc rises to 4.5V, or > 40ms after Vcc rises to 2.7V
 *   1. send nibble 0x03     // select 8-bit interface
 *   2. wait > 4.1ms
 *   3. send nibble 0x03     // select 8-bit interface again
 *   4. wait > 100us
 *   5. send command 0x32    // select 4-bit interface
 *   6. send command 0x28    // set 2 lines and 5x7(8?) dots per character
 *   7. send command 0x0c    // display on, cursor off
 *   8. send command 0x06    // move cursor right when writing, no scroll
 *   9. send command 0x80    // set cursor to home position (row 1, column 1)
 */


#include "lcd.h"

static i2c_lcd1602_info_t i2c_lcd1602_info = {};

uint8_t string_to_terminal[256] = {};

// Set or clear the specified flag depending on condition
static uint8_t
set_or_clear(uint8_t flags, uint8_t condition, uint8_t flag) {
    if (condition == TRUE) {
        flags |= flag;
    } else {
        flags &= ~flag;
    }
    return flags;
}

// send data to the I/O Expander
static lsd_err_t
write_to_expander(uint8_t value) {

    HAL_StatusTypeDef result;
    for (size_t i = 0; i < I2C_EXPANDER_MAX_TRY; i++) {
        result = HAL_I2C_IsDeviceReady(I2C_LSD, I2C_LSD_ADDRESS, 1,HAL_MAX_DELAY);
        if (result == HAL_OK)
            break;
    }

    uint8_t data[1] = {value | i2c_lcd1602_info.backlight_flag};

    result = HAL_I2C_Master_Transmit(I2C_LSD, I2C_LSD_ADDRESS, data, 1,
                                     HAL_MAX_DELAY);

    if (result == HAL_OK) {
        return LSD_OK;
    }

    return LSD_FAIL;

}

// IMPORTANT - for the display to stay "in sync" it is important that errors do not interrupt the
// 2 x nibble sequence.

// clock data from expander to LCD by causing a falling edge on Enable
static lsd_err_t
strobe_enable(uint8_t data) {
    lsd_err_t err1 = write_to_expander(data | FLAG_ENABLE);
    HAL_Delay(DELAY_ENABLE_PULSE_WIDTH);
    lsd_err_t err2 = write_to_expander(data & ~FLAG_ENABLE);
    HAL_Delay(DELAY_ENABLE_PULSE_SETTLE);
    return err1 ? err1 : err2;
}

// send top nibble to the LCD controller
static lsd_err_t
write_top_nibble(uint8_t data) {
    lsd_err_t err1 = write_to_expander(data);
    lsd_err_t err2 = strobe_enable(data);
    return err1 ? err1 : err2;
}

// send command or data to controller
static lsd_err_t
write(uint8_t value, uint8_t register_select_flag) {
    lsd_err_t err1 = write_top_nibble((value & 0xf0) | register_select_flag);
    lsd_err_t err2 = write_top_nibble(((value & 0x0f) << 4) | register_select_flag);
    return err1 ? err1 : err2;
}

// send command to controller
static lsd_err_t
write_command(uint8_t command) {
    return write(command, FLAG_RS_COMMAND);
}

// send data to controller
static lsd_err_t
write_data(uint8_t data) {
    return write(data, FLAG_RS_DATA);
}

/**
 * \brief Clears entire display (clears DDRAM) and returns cursor to home position.
 *        DDRAM content is cleared, CGRAM content is not changed.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_clear() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        err = write_command(COMMAND_CLEAR_DISPLAY);
        if (err == LSD_OK) {
            HAL_Delay(DELAY_CLEAR_DISPLAY);
        }
    }
    return err;
}

/**
 * \brief Move cursor to home position. Also resets any display shift that may have occurred.
 *        DDRAM content is not changed. CGRAM content is not changed.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_home() {
    lsd_err_t err = LSD_FAIL;

    if (i2c_lcd1602_info.init == TRUE) {
        err = write_command(COMMAND_RETURN_HOME);
        if (err == LSD_OK) {
            HAL_Delay(DELAY_CLEAR_DISPLAY);
        }
    }
    return err;
}

/**
 * \brief Move cursor to specified column and row position. This is where a new character will appear.
 *
 * \param[in] col Zero-based horizontal index of intended cursor position. Column 0 is the left column.
 * \param[in] row Zero-based vertical index of intended cursor position. Row 0 is the top row.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_move_cursor(uint8_t col, uint8_t row) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        const int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        if (row > i2c_lcd1602_info.num_rows) {
            row = i2c_lcd1602_info.num_rows - 1;
        }
        if (col > i2c_lcd1602_info.num_columns) {
            col = i2c_lcd1602_info.num_columns - 1;
        }
        err = write_command(COMMAND_SET_DDRAM_ADDR | (col + row_offsets[row]));
    }
    return err;
}

/**
 * \brief Enable or disable the LED backlight.
 *
 * \param[in] enable True to enable, false to disable.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_backlight(uint8_t enable) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.backlight_flag = set_or_clear(i2c_lcd1602_info.backlight_flag, enable, FLAG_BACKLIGHT_ON);
        err = write_to_expander(0);
    }
    return err;
}

/**
 * \brief Enable or disable the display. When disabled, the backlight is not affected, but any
 *        contents of the DDRAM is not displayed, nor is the cursor. The display is "blank".
 *        Re-enabling the display does not affect the contents of DDRAM or the state or position of the cursor.
 *
 * \param[in] enable True to enable, false to disable.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_display(uint8_t enable) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.display_control_flags = set_or_clear(i2c_lcd1602_info.display_control_flags, enable,
                                                              FLAG_DISPLAY_CONTROL_DISPLAY_ON);
        err = write_command(COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info.display_control_flags);
    }
    return err;
}

/**
 * \brief Enable or disable display of the underline cursor.
 *        If enabled, this visually indicates where the next character written to the display will appear.
 *        It may be enabled alongside the blinking cursor, however the visual result is inelegant.
 *
 * \param[in] enable True to enable, false to disable.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_cursor(uint8_t enable) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.display_control_flags = set_or_clear(i2c_lcd1602_info.display_control_flags, enable,
                                                              FLAG_DISPLAY_CONTROL_CURSOR_ON);
        err = write_command(COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info.display_control_flags);
    }
    return err;
}

/**
 * \brief Enable or disable display of the blinking block cursor.
 *        If enabled, this visually indicates where the next character written to the display will appear.
 *        It may be enabled alongside the underline cursor, however the visual result is inelegant.
 *
 * \param[in] enable True to enable, false to disable.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_blink(uint8_t enable) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.display_control_flags = set_or_clear(i2c_lcd1602_info.display_control_flags, enable,
                                                              FLAG_DISPLAY_CONTROL_BLINK_ON);
        err = write_command(COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info.display_control_flags);
    }
    return err;
}

/**
 * \brief Set cursor movement direction following each character write to produce left-to-right text.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_left_to_right() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.entry_mode_flags |= FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT;
        err = write_command(COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info.entry_mode_flags);
    }
    return err;
}

/**
 * \brief Set cursor movement direction following each character write to produce right-to-left text.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_right_to_left() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.entry_mode_flags &= ~FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT;
        err = write_command(COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info.entry_mode_flags);
    }
    return err;
}

/**
 * \brief Enable or disable auto-scroll of display.
 *        When enabled, the display will scroll as characters are written to maintain the cursor position on-screen.
 *        Left-to-right text will appear to be right-justified from the cursor position.
 *        When disabled, the display will not scroll and the cursor will move on-screen.
 *        Left-to-right text will appear to be left-justified from the cursor position.
 *
 * \param[in] enable True to enable, false to disable.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_set_auto_scroll(uint8_t enable) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        i2c_lcd1602_info.entry_mode_flags = set_or_clear(i2c_lcd1602_info.entry_mode_flags, enable,
                                                         FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_ON);
        err = write_command(COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info.entry_mode_flags);
    }
    return err;
}

/**
 * \brief Scroll the display one position to the left. On-screen text will appear to move to the right.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_scroll_display_left() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        // RAM is not changed
        err = write_command(COMMAND_SHIFT | FLAG_SHIFT_MOVE_DISPLAY | FLAG_SHIFT_MOVE_LEFT);
    }
    return err;
}

/**
 * \brief Scroll the display one position to the right. On-screen text will appear to move to the left.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_scroll_display_right() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        // RAM is not changed
        err = write_command(COMMAND_SHIFT | FLAG_SHIFT_MOVE_DISPLAY | FLAG_SHIFT_MOVE_RIGHT);
    }
    return err;
}

/**
 * \brief Move the cursor one position to the left, even if it is invisible.
 *        This affects where the next character written to the display will appear.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_move_cursor_left() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        // RAM is not changed. Shift direction is inverted.
        err = write_command(COMMAND_SHIFT | FLAG_SHIFT_MOVE_CURSOR | FLAG_SHIFT_MOVE_RIGHT);
    }
    return err;
}

/**
 * \brief Move the cursor one position to the right, even if it is invisible.
 *        This affects where the next character written to the display will appear.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_move_cursor_right() {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        // RAM is not changed. Shift direction is inverted.
        err = write_command(COMMAND_SHIFT | FLAG_SHIFT_MOVE_CURSOR | FLAG_SHIFT_MOVE_LEFT);
    }
    return err;
}

/**
 * \brief Reset the display. Custom characters will be cleared.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_reset() {

    lsd_err_t err = LSD_OK;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    HAL_Delay(50);

    // put Expander into known state - Register Select and Read/Write both low
    if (write_to_expander(0) == LSD_FAIL) {
        return err;
    }

    HAL_Delay(1);

    // select 4-bit mode on LCD controller - see datasheet page 46, figure 24.
    if (write_top_nibble(0x03 << 4) == LSD_FAIL) {
        return err;
    }

    HAL_Delay(DELAY_INIT_1);

    if (write_top_nibble(0x03 << 4) == LSD_FAIL) {
        return err;
    }

    HAL_Delay(DELAY_INIT_2);

    if (write_top_nibble(0x03 << 4) == LSD_FAIL) {
        return err;
    }

    HAL_Delay(DELAY_INIT_3);

    // select 4-bit mode
    if (write_top_nibble(0x02 << 4) == LSD_FAIL) {
        return err;
    }

    // now we can use the command()/write() functions
    if (write_command(COMMAND_FUNCTION_SET | FLAG_FUNCTION_SET_MODE_4BIT | FLAG_FUNCTION_SET_LINES_2 |
                      FLAG_FUNCTION_SET_DOTS_5X8) == LSD_FAIL) {
        return err;
    }

    if (write_command(COMMAND_DISPLAY_CONTROL | i2c_lcd1602_info.display_control_flags) == LSD_FAIL) {
        return err;
    }

    if (i2c_lcd1602_clear() == LSD_FAIL) {
        return err;
    }

    if (write_command(COMMAND_ENTRY_MODE_SET | i2c_lcd1602_info.entry_mode_flags) == LSD_FAIL) {
        return err;
    }

    if (i2c_lcd1602_home() == LSD_FAIL) {
        return err;
    }

    return err;

}

/**
 * \brief Initialise a I2C-LCD1602 info instance.
 *
 * \param[in] backlight Initial backlight state.
 * \param[in] num_rows Maximum number of supported rows for this device. Typical values include 2 (1602) or 4 (2004).
 * \param[in] num_columns Maximum number of supported columns for this device. Typical values include 40 (1602, 2004).
 * \param[in] num_visible_columns Number of columns visible at any one time. Typical values include 16 (1602) or 20 (2004).
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_init(uint8_t backlight, uint8_t num_rows, uint8_t num_columns,
                 uint8_t num_visible_columns) {
    lsd_err_t err = LSD_FAIL;

    if (backlight == TRUE) {
        i2c_lcd1602_info.backlight_flag = FLAG_BACKLIGHT_ON;
    } else {
        i2c_lcd1602_info.backlight_flag = FLAG_BACKLIGHT_OFF;
    }
    i2c_lcd1602_info.num_rows = num_rows;
    i2c_lcd1602_info.num_columns = num_columns;
    i2c_lcd1602_info.num_visible_columns = num_visible_columns;

    // display on, no cursor, no blinking
    i2c_lcd1602_info.display_control_flags =
        FLAG_DISPLAY_CONTROL_DISPLAY_ON | FLAG_DISPLAY_CONTROL_CURSOR_OFF | FLAG_DISPLAY_CONTROL_BLINK_OFF;

    // left-justified left-to-right text
    i2c_lcd1602_info.entry_mode_flags = FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT | FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_OFF;

    i2c_lcd1602_info.init = TRUE;

    // See page 45/46 of HD44780 data sheet for the initialisation procedure.

    // Wait at least 40ms after power rises above 2.7V before sending commands.
    HAL_Delay(DELAY_POWER_ON);

    err = i2c_lcd1602_reset();

    return err;
}

/**
 * \brief Define a custom character from an array of pixel data.
 *
 *        There are eight possible custom characters, and the zero-based index is used
 *        to select a character to define. Any existing character definition at that index will be lost.
 *        Characters are 5 pixels wide and 8 pixels tall.
 *        The pixelmap array consists of up to eight bytes, each byte representing the pixel states per row.
 *        The first byte represents the top row. The eighth byte is often left as zero (to leave space for the underline cursor).
 *        For each row, the lowest five bits represent pixels that are to be illuminated. The least significant bit represents
 *        the right-most pixel. Empty rows will be zero.
 *
 *        NOTE: After calling this function, the DDRAM will not be selected and the cursor position will be undefined. Therefore it
 *        is important that the DDRAM address is set following this function, if text is to be written to the display.
 *        This can be performed with a call to i2c_lcd1602_home() or i2c_lcd1602_move_cursor().
 *
 *        Custom characters are written using the I2C_LCD1602_CHARACTER_CUSTOM_X definitions.
 *
 * \param[in] index Zero-based index of the character to define. Only values 0-7 are valid.
 * \param[in] pixelmap An 8-byte array defining the pixel map for the new character definition.
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_define_char(i2c_lcd1602_custom_index_t index,
                        const uint8_t pixelmap[]) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {
        index &= 0x07; // only the first 8 indexes can be used for custom characters
        err = write_command(COMMAND_SET_CGRAM_ADDR | (index << 3));
        for (int i = 0; err == LSD_OK && i < 8; ++i) {
            err = write_data(pixelmap[i]);
        }
    }
    return err;
}

/**
 * \brief Write a single character to the display at the current position of the cursor.
 *        Depending on the active mode, the cursor may move left or right, or the display may shift left or right.
 *        Custom characters can be written using the I2C_LCD1602_CHARACTER_CUSTOM_X definitions.
 *
 *        The display is I2C_LCD1602_NUM_COLUMNS wide, and upon encountering the end of the first line, the cursor
 *        will automatically move to the beginning of the second line.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_write_char(uint8_t chr) {
    lsd_err_t err = LSD_FAIL;

    if (i2c_lcd1602_info.init == TRUE) {
        err = write_data(chr);
        if (err == LSD_OK) {
            HAL_Delay(DELAY_CLEAR_DISPLAY);
        }
    }
    return err;

}

/**
 * \brief Write a string of characters to the display, starting at the current position of the cursor.
 *        Depending on the active mode, the cursor may move left or right, or the display may shift left or right,
 *        after each character is written.
 *
 *        The display is I2C_LCD1602_NUM_COLUMNS wide, and upon encountering the end of the first line, the cursor
 *        will automatically move to the beginning of the second line.
 *
 * \return LSD_OK if successful, otherwise an error constant.
 */
lsd_err_t
i2c_lcd1602_write_string(const char* string) {
    lsd_err_t err = LSD_FAIL;
    if (i2c_lcd1602_info.init == TRUE) {

        err = LSD_OK;
        for (int i = 0; err == LSD_OK && string[i]; ++i) {
            err = write_data(string[i]);
        }
    }
    return err;
}
