/**
 * \file            lcd.h
 * \brief           Функции для работы с LCD QAPASS 1602
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "compiler.h"
#include "terminal.h"
#include "i2c.h"

// Delays (ms)
#define DELAY_POWER_ON            40  // wait at least 40ms after VCC rises to 2.7V
#define DELAY_INIT_1               5  // wait at least 4.1ms (fig 24, page 46)
#define DELAY_INIT_2               5  // wait at least 4.1ms (fig 24, page 46)
#define DELAY_INIT_3               1  // wait at least 100us (fig 24, page 46)

#define DELAY_CLEAR_DISPLAY        2
#define DELAY_RETURN_HOME          2

#define DELAY_ENABLE_PULSE_WIDTH     1  // enable pulse must be at least 450ns wide
#define DELAY_ENABLE_PULSE_SETTLE    1  // command requires > 37us to settle (table 6 in datasheet)

// Commands
#define COMMAND_CLEAR_DISPLAY       0x01
#define COMMAND_RETURN_HOME         0x02
#define COMMAND_ENTRY_MODE_SET      0x04
#define COMMAND_DISPLAY_CONTROL     0x08
#define COMMAND_SHIFT               0x10
#define COMMAND_FUNCTION_SET        0x20
#define COMMAND_SET_CGRAM_ADDR      0x40
#define COMMAND_SET_DDRAM_ADDR      0x80

// COMMAND_ENTRY_MODE_SET flags
#define FLAG_ENTRY_MODE_SET_ENTRY_INCREMENT       0x02
#define FLAG_ENTRY_MODE_SET_ENTRY_DECREMENT       0x00
#define FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_ON        0x01
#define FLAG_ENTRY_MODE_SET_ENTRY_SHIFT_OFF       0x00

// COMMAND_DISPLAY_CONTROL flags
#define FLAG_DISPLAY_CONTROL_DISPLAY_ON  0x04
#define FLAG_DISPLAY_CONTROL_DISPLAY_OFF 0x00
#define FLAG_DISPLAY_CONTROL_CURSOR_ON   0x02
#define FLAG_DISPLAY_CONTROL_CURSOR_OFF  0x00
#define FLAG_DISPLAY_CONTROL_BLINK_ON    0x01
#define FLAG_DISPLAY_CONTROL_BLINK_OFF   0x00

// COMMAND_SHIFT flags
#define FLAG_SHIFT_MOVE_DISPLAY          0x08
#define FLAG_SHIFT_MOVE_CURSOR           0x00
#define FLAG_SHIFT_MOVE_LEFT             0x04
#define FLAG_SHIFT_MOVE_RIGHT            0x00

// COMMAND_FUNCTION_SET flags
#define FLAG_FUNCTION_SET_MODE_8BIT      0x10
#define FLAG_FUNCTION_SET_MODE_4BIT      0x00
#define FLAG_FUNCTION_SET_LINES_2        0x08
#define FLAG_FUNCTION_SET_LINES_1        0x00
#define FLAG_FUNCTION_SET_DOTS_5X10      0x04
#define FLAG_FUNCTION_SET_DOTS_5X8       0x00

// Control flags
#define FLAG_BACKLIGHT_ON    0b00001000      // backlight enabled (disabled if clear)
#define FLAG_BACKLIGHT_OFF   0b00000000      // backlight disabled
#define FLAG_ENABLE          0b00000100
#define FLAG_READ            0b00000010      // read (write if clear)
#define FLAG_WRITE           0b00000000      // write
#define FLAG_RS_DATA         0b00000001      // data (command if clear)
#define FLAG_RS_COMMAND      0b00000000      // command

#define I2C_LSD (&hi2c1)
#define I2C_EXPANDER_MAX_TRY 100000
#define I2C_LSD_ADDRESS (0x27 << 1)
#define LCD_DELAY_MS 1

/**
 * @brief Enum of valid indexes for definitions of user-defined characters.
 */
typedef enum {
    I2C_LCD1602_INDEX_CUSTOM_0 = 0, ///< Index of first user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_1,     ///< Index of second user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_2,     ///< Index of third user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_3,     ///< Index of fourth user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_4,     ///< Index of fifth user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_5,     ///< Index of sixth user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_6,     ///< Index of seventh user-defined custom symbol
    I2C_LCD1602_INDEX_CUSTOM_7,     ///< Index of eighth user-defined custom symbol
} i2c_lcd1602_custom_index_t;

/**
 * @brief Structure containing information related to the I2C-LCD1602 device.
 */
typedef struct {
    uint8_t init;                  ///< TRUE if struct has been initialised, otherwise FALSE
    uint8_t backlight_flag;        ///< Non-zero if backlight is to be enabled, otherwise zero
    uint8_t num_rows;              ///< Number of configured columns
    uint8_t num_columns;           ///< Number of configured columns, including offscreen columns
    uint8_t num_visible_columns;   ///< Number of visible columns
    uint8_t display_control_flags; ///< Currently active display control flags
    uint8_t entry_mode_flags;      ///< Currently active entry mode flags
} i2c_lcd1602_info_t;

typedef enum {
    LSD_OK = 0,
    LSD_FAIL = 1,
} lsd_err_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static uint8_t
set_or_clear(uint8_t flags, uint8_t condition, uint8_t flag);

static lsd_err_t
write_to_expander(uint8_t value);

static lsd_err_t
strobe_enable(uint8_t data);

static lsd_err_t
write_top_nibble(uint8_t data);

static lsd_err_t
write(uint8_t value, uint8_t register_select_flag);

static lsd_err_t
write_command(uint8_t command);

static lsd_err_t
write_data(uint8_t data);

lsd_err_t
i2c_lcd1602_clear();

lsd_err_t
i2c_lcd1602_home();

lsd_err_t
i2c_lcd1602_move_cursor(uint8_t col, uint8_t row);

lsd_err_t
i2c_lcd1602_set_backlight(uint8_t enable);

lsd_err_t
i2c_lcd1602_set_display(uint8_t enable);

lsd_err_t
i2c_lcd1602_set_cursor(uint8_t enable);

lsd_err_t
i2c_lcd1602_set_blink(uint8_t enable);

lsd_err_t
i2c_lcd1602_set_left_to_right();

lsd_err_t
i2c_lcd1602_set_right_to_left();

lsd_err_t
i2c_lcd1602_set_auto_scroll(uint8_t enable);

lsd_err_t
i2c_lcd1602_scroll_display_left();

lsd_err_t
i2c_lcd1602_scroll_display_right();

lsd_err_t
i2c_lcd1602_move_cursor_left();

lsd_err_t
i2c_lcd1602_move_cursor_right();

lsd_err_t
i2c_lcd1602_reset();

lsd_err_t
i2c_lcd1602_init(uint8_t backlight, uint8_t num_rows, uint8_t num_columns,
                 uint8_t num_visible_columns);

lsd_err_t
i2c_lcd1602_define_char(i2c_lcd1602_custom_index_t index,
                        const uint8_t pixelmap[]);

lsd_err_t
i2c_lcd1602_write_char(uint8_t chr);

lsd_err_t
i2c_lcd1602_write_string(const char* string);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //LCD_H
