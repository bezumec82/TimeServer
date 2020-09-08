#include "eINK.hpp"
#include "IL0373_cmd.h"

using namespace eINK;

IL0373::IL0373()
{
    initGpio();
    /* Set initial GPIO state */
    GPIO_WRITE(eINK_DC, 0);
    GPIO_WRITE(eINK_CS, 1);
    GPIO_WRITE(eINK_RST, 1);

    //I assume SPI already initialized
    //at this point
    init();
    //clear();
}

void IL0373::initGpio(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /* ... if necessary */

    GPIO_InitStruct.Pin = eINK_RST_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(eINK_RST_port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = eINK_DC_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(eINK_DC_port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = eINK_CS_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(eINK_CS_port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = eINK_BUSY_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(eINK_BUSY_port, &GPIO_InitStruct);
}

void IL0373::sendCmd(uint8_t byte)
{
    GPIO_WRITE(eINK_DC, 0); // Data/Cmd pin

    //TODO : re-make this shit
    GPIO_WRITE(eINK_CS, 0);
    writeByte(&byte);
    GPIO_WRITE(eINK_CS, 1);
}

void IL0373::sendData(uint8_t byte)
{
    GPIO_WRITE(eINK_DC, 1); // Data/Cmd pin

    //TODO : re-make this shit
    GPIO_WRITE(eINK_CS, 0);
    writeByte(&byte);
    GPIO_WRITE(eINK_CS, 1);
}

void IL0373::wait()
{
    while(GPIO_READ(eINK_BUSY) == 0)
    {
        yield();
    }
}

void IL0373::init()
{
    reset();

    sendCmd(BOOSTER_SOFT_START); // BOOSTER_SOFT_START
    sendData(BOOSTER_SOFT_START_DEFVAL);
    sendData(BOOSTER_SOFT_START_DEFVAL);
    sendData(BOOSTER_SOFT_START_DEFVAL);

    sendCmd(POWER_ON);
    wait();

    sendCmd(PANEL_SETTING);
#if USE_THREE_COLOR
    sendData(
            DISP_RES_128x296    << DISP_RES_OFF |
            LUT_FROM_OTP        << LUT_SEL_OFF  |
            BWR_LU1LU2          << BWR_OFF      |
            GATE_SCAN_UP        << GATE_SCAN_DIR_OFF |
            SOURCE_SHIFT_RIGHT  << SOURCE_SHIFT_DIR_OFF |
            BOOSTER_ON          << BOOSTER_SW_OFF |
            NO_RST              << SOFT_RST_OFF
            );
#else
    sendData(
            DISP_RES_128x296    << DISP_RES_OFF |
            LUT_FROM_REG        << LUT_SEL_OFF  |
            BWR_LU1_ONLY        << BWR_OFF      |
            GATE_SCAN_UP        << GATE_SCAN_DIR_OFF |
            SOURCE_SHIFT_RIGHT  << SOURCE_SHIFT_DIR_OFF |
            BOOSTER_ON          << BOOSTER_SW_OFF |
            NO_RST              << SOFT_RST_OFF
            );
#endif
    sendCmd(VCOM_AND_DATA_INTERVAL_SETTING);
    sendData(
            BORDER_DATA_VAL << BORDER_DATA_SEL_OFF |
            DATA_POL_VAL    << DATA_POL_OFF |
            INTERVAL_VAL    << INTERVAL_OFF
            );

    sendCmd(RESOLUTION_SETTING);
    sendData(eINK_WIDTH_PIX);
    sendData(eINK_HEIGHT_PIX >> 8);
    sendData(eINK_HEIGHT_PIX & 0xff);
}

void IL0373::reset()
{
    GPIO_WRITE(eINK_RST, 1);
    yield(200);
    GPIO_WRITE(eINK_RST, 0);
    yield(200);
    GPIO_WRITE(eINK_RST, 1);
    yield(200);
}

void IL0373::refresh()
{
    sendCmd(DISPLAY_REFRESH);
    yield(20);
    wait();
}

void IL0373::clear()
{
    uint8_t width  = GET_WIDTH_BYTES;
    uint8_t height = eINK_HEIGHT_PIX;

    /* black/white data */
    sendCmd(DATA_START_TRANSMISSION);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            sendData(0xff);
        }
    }
    sendCmd(PARTIAL_OUT);

    /* red/yellow data */
    sendCmd(DATA_START_TRANSMISSION_2);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            sendData(0xff);
        }
    }
    sendCmd(PARTIAL_OUT);
    refresh();
}

/*!
 * Sends black and red/yellow buffer to the display controller buffer.
 * @param black_img Black buffer to send.
 * @param red_img Red buffer to send.
 */
void IL0373::display(const uint8_t * black_img, const uint8_t * red_img)
{
    uint8_t width  = GET_WIDTH_BYTES;
    uint8_t height = eINK_HEIGHT_PIX;

    /* black/white data */
    sendCmd(DATA_START_TRANSMISSION);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
#if USE_THREE_COLOR
            sendData(black_img[i * width + j/* curr. pos. in line */]);
#else
            sendData(0x00);
#endif
        }
    }
#if USE_THREE_COLOR
    sendCmd(PARTIAL_OUT);
#endif

    /* red/yellow data */
    sendCmd(DATA_START_TRANSMISSION_2);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
#if USE_THREE_COLOR
            sendData(red_img[i * width + j]);
#else
            sendData(black_img[i * width + j]);
#endif
        }
    }
#if USE_THREE_COLOR
    sendCmd(PARTIAL_OUT);
#else
    setLutsFull();
#endif
    refresh();
}

void IL0373::displayBlack(const uint8_t * black_img)
{
    uint8_t width  = GET_WIDTH_BYTES;
    uint8_t height = eINK_HEIGHT_PIX;

    /* black/white data */
    sendCmd(DATA_START_TRANSMISSION);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            sendData(black_img[i * width + j/* curr. pos. in line */]);
        }
    }
    sendCmd(PARTIAL_OUT);
    refresh();
}

void IL0373::displayRed(const uint8_t * red_img)
{
    uint8_t width  = GET_WIDTH_BYTES;
    uint8_t height = eINK_HEIGHT_PIX;

    /* red/yellow data */
    sendCmd(DATA_START_TRANSMISSION_2);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            sendData(red_img[i * width + j]);
        }
    }
    sendCmd(PARTIAL_OUT);
    refresh();
}
