#define PANEL_SETTING               0x00
# define DISP_RES_OFF                6
# define DISP_RES_96x230             0b00
# define DISP_RES_128x296            0b10

# define LUT_SEL_OFF                 5
# define LUT_FROM_OTP                0
# define LUT_FROM_REG                1

# define BWR_OFF                     4
# define BWR_LU1LU2                  0
# define BWR_LU1_ONLY                1

# define GATE_SCAN_DIR_OFF           3
# define GATE_SCAN_DOWN              0
# define GATE_SCAN_UP                1

# define SOURCE_SHIFT_DIR_OFF        2
# define SOURCE_SHIFT_LEFT           0
# define SOURCE_SHIFT_RIGHT          1

# define BOOSTER_SW_OFF              1
# define BOOSTER_OFF                 0
# define BOOSTER_ON                  1

# define SOFT_RST_OFF                0
# define NO_RST                      1

#define POWER_ON                    0x04

#define BOOSTER_SOFT_START              0x06
# define BOOSTER_SOFT_START_DEFVAL       0x17

#define DATA_START_TRANSMISSION         0x10
    /* This command starts transmitting data and write them into SRAM.
     * To complete data transmission,
     * command DSP (Data transmission Stop) must be issued.
     * Then the chip will start to send data/VCOM for panel. */

#define DISPLAY_REFRESH                 0x12
    /* While user sent this command,
     * driver will refresh display (data/VCOM) according to SRAM data and LUT. */

#define DATA_START_TRANSMISSION_2       0x13

#define VCOM_LUT                        0x20
    /* This command stores VCOM Look-Up Table with 7 groups of data. */

#define W2W_LUT                         0x21
    /* This command stores White-to-White Look-Up Table with 7 groups of data. */

#define B2W_LUT                         0x22
    /* This command builds Look-up Table for Black-to-White. */

#define W2B_LUT                         0x23
    /* This command builds Look-up Table for White-to-Black. */

#define B2B_LUT                         0x24
    /* This command builds Look-up Table for Black-to-Black. */


#define VCOM_AND_DATA_INTERVAL_SETTING  0x50
# define BORDER_DATA_SEL_OFF             6
# define BORDER_DATA_VAL                 0b11

# define DATA_POL_OFF                    4
# define DATA_POL_VAL                    0b11

# define INTERVAL_OFF                    0
# define INTERVAL_VAL                    0b0000

#define RESOLUTION_SETTING              0x61

#define PARTIAL_OUT                     0x92
    /* This command makes the display exit partial mode
     * and enter normal mode. */

