/*
    Copyright (c) 2012 <benemorius@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#define DELAY50MS() for(uint32_t i = 0; i < 416635; i++)
#define DELAY250MS() for(uint32_t i = 0; i < 2083175; i++)

#define DEBOUNCE_DELAY (20)

#define REFERENCE_VOLTAGE (3.0f)

extern "C" char* get_stack_top(void);
extern "C" char* get_heap_end(void);
extern "C" char* get_mem_free(void);

#define SPI0_MOSI_PORT (0)
#define SPI0_MOSI_PIN (18)
#define SPI0_MISO_PORT (0)
#define SPI0_MISO_PIN (17)
#define SPI0_SCK_PORT (0)
#define SPI0_SCK_PIN (15)

#define SPI1_MOSI_PORT (0)
#define SPI1_MOSI_PIN (9)
#define SPI1_MISO_PORT (0)
#define SPI1_MISO_PIN (8)
#define SPI1_SCK_PORT (0)
#define SPI1_SCK_PIN (7)

#define LCD_SELECT_PORT (2)
#define LCD_SELECT_PIN (7)
#define LCD_REFRESH_PORT (2)
#define LCD_REFRESH_PIN (2)
#define LCD_UNK0_PORT (2)
#define LCD_UNK0_PIN (5)
#define LCD_UNK1_PORT (2)
#define LCD_UNK1_PIN (6)
#define LCD_RESET_PORT (2)
#define LCD_RESET_PIN (3)

#define DEBUG_TX_PORTNUM (0)
#define DEBUG_TX_PINNUM (2)
#define DEBUG_RX_PORTNUM (0)
#define DEBUG_RX_PINNUM (3)
#define DEBUG_BAUD (115200)


#define KLINE_TX_PORTNUM (4)
#define KLINE_TX_PINNUM (28)
#define KLINE_RX_PORTNUM (4)
#define KLINE_RX_PINNUM (29)
#define KLINE_BAUD (9600)

#define LLINE_TX_PORTNUM (2)
#define LLINE_TX_PINNUM (8)
#define LLINE_RX_PORTNUM (2)
#define LLINE_RX_PINNUM (9)
#define LLINE_BAUD (9600)

#define KL_WAKE_PORT (2)
#define KL_WAKE_PIN (4)

#define LCD_BACKLIGHT_PORT (1)
#define LCD_BACKLIGHT_PIN (23)

#define CLOCK_BACKLIGHT_PORT (3)
#define CLOCK_BACKLIGHT_PIN (25)

#define AUX_BACKLIGHT_PORT (1)
#define AUX_BACKLIGHT_PIN (24)

#define LCD_BIASCLOCK_PORT (1)
#define LCD_BIASCLOCK_PIN (26)

#define KEYPAD_BACKLIGHT_PORT (3)
#define KEYPAD_BACKLIGHT_PIN (26)

#define RUN_PORT (2)
#define RUN_PIN (11)

#define SDCARD_DETECT_PORT (1)
#define SDCARD_DETECT_PIN (17)
#define SDCARD_CS_PORT (1)
#define SDCARD_CS_PIN (16)

#define CCM_DATA_PORT (1)
#define CCM_DATA_PIN (9)
#define CCM_CLOCK_PORT (1)
#define CCM_CLOCK_PIN (4)
#define CCM_LATCH_PORT (1)
#define CCM_LATCH_PIN (8)

#define FUEL_LEVEL_PORT (0)
#define FUEL_LEVEL_PIN (6)

#define STALK_BUTTON_PORT (2)
#define STALK_BUTTON_PIN (12)

#define SPEED_PORT (0)
#define SPEED_PIN (4)

#define FUEL_CONS_PORT (0)
#define FUEL_CONS_PIN (5)

#define BATTERY_VOLTAGE_PORT (0)
#define BATTERY_VOLTAGE_PIN (25)

#define EXT_TEMP_PORT (0)
#define EXT_TEMP_PIN (23)

#define VSTART_PORT (0)
#define VSTART_PIN (24)

#define VREF_EN_PORT (1)
#define VREF_EN_PIN (14)

#define CAN1_TD_PORT (0)
#define CAN1_TD_PIN (22)
#define CAN1_RD_PORT (0)
#define CAN1_RD_PIN (21)

#define I2C1_SDA_PORT (0)
#define I2C1_SDA_PIN (19)
#define I2C1_SCL_PORT (0)
#define I2C1_SCL_PIN (20)

#define I2C0_SDA_PORT (0)
#define I2C0_SDA_PIN (27)
#define I2C0_SCL_PORT (0)
#define I2C0_SCL_PIN (28)

#define PCA95XX_ADDRESS (0x40)
#define PCA95XX_INTERRUPT_PORT (1)
#define PCA95XX_INTERRUPT_PIN (0)

#define CODE_LED_PORT (1)
#define CODE_LED_PIN (0)
#define LIMIT_LED_PORT (1)
#define LIMIT_LED_PIN (1)
#define TIMER_LED_PORT (1)
#define TIMER_LED_PIN (2)
#define CCM_LIGHT_PORT (0)
#define CCM_LIGHT_PIN (2)
#define CHIME0_PORT (1)
#define CHIME0_PIN (4)
#define CHIME1_PORT (1)
#define CHIME1_PIN (3)
#define VENTILATION_PORT (0)
#define VENTILATION_PIN (1)
#define ANTITHEFT_HORN_PORT (0)
#define ANTITHEFT_HORN_PIN (0)
#define EWS_PORT (1)
#define EWS_PIN (5)
#define OUT0_PORT (0)
#define OUT0_PIN (3)
#define OUT1_PORT (0)
#define OUT1_PIN (4)
#define OUT2_PORT (0)
#define OUT2_PIN (5)
#define OUT3_PORT (0)
#define OUT3_PIN (6)
#define IO0_PORT (0)
#define IO0_PIN (7)
#define IO1_PORT (1)
#define IO1_PIN (7)

#define LCD_BIAS_EN_PORT (1)
#define LCD_BIAS_EN_PIN (6)

#define ACCEL_ADDRESS (0x38)
#define ACCEL_INTERRUPT_PORT (2)
#define ACCEL_INTERRUPT_PIN (13)

#define ANALOG_IN1_PORT (1)
#define ANALOG_IN1_PIN (31)
#define ANALOG_IN2_PORT (1)
#define ANALOG_IN2_PIN (30)

#define ANALOG_OUT_PORT (0)
#define ANALOG_OUT_PIN (26)

#define ILLUMINATION_PORT (1)
#define ILLUMINATION_PIN (19)

#define AMBIENT_LIGHT_PORT (1)
#define AMBIENT_LIGHT_PIN (18)

#define BRAKE_SWITCH_PORT (1)
#define BRAKE_SWITCH_PIN (1)

#define BRAKE_CHECK_PORT (1)
#define BRAKE_CHECK_PIN (10)

#define DEFAULT_CCM_DISABLE_MASK (0x00)
#define DEFAULT_CCM_INVERT_MASK (ObcCCMBits::WasherFluid | ObcCCMBits::CoolantLevel | ObcCCMBits::BrakeLight1 | ObcCCMBits::BrakeLight2)
