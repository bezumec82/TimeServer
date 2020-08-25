#include "core.hpp"

using namespace LaOS;

#define IWDG_MAX_WINDOW	0x0FFF
#define IWDG_MAX_RELOAD 0x0FFF

#ifndef IWDG_PRESCALER_4
# define IWDG_PRESCALER_4       0b000
# define IWDG_PRESCALER_8       0b001
# define IWDG_PRESCALER_16      0b010
# define IWDG_PRESCALER_32      0b011
# define IWDG_PRESCALER_64      0b100
# define IWDG_PRESCALER_128     0b101
# define IWDG_PRESCALER_256     0b111
#endif

#ifndef IWDG_KEY_RELOAD
# define IWDG_KEY_RELOAD                0x0000AAAAu  /*!< IWDG Reload Counter Enable   */
# define IWDG_KEY_ENABLE                0x0000CCCCu  /*!< IWDG Peripheral Enable       */
# define IWDG_KEY_WRITE_ACCESS_ENABLE   0x00005555u  /*!< IWDG KR Write Access Enable  */
# define IWDG_KEY_WRITE_ACCESS_DISABLE  0x00000000u  /*!< IWDG KR Write Access Disable */
#endif

#define IWDG_SR_TIMEOUT     50

#if (USE_WATCHDOG)

void Core::ConfigWDG()
{
    PRINTF("Initializing watchdog.\r\n");
    /* 1) Enable the IWDG by writing 0x0000CCCC
     * in the IWDG key register (IWDG_KR). */
    iwdgRegs->KR = IWDG_KEY_ENABLE;
    /* 2) Enable register access by writing 0x00005555
     * in the IWDG key register (IWDG_KR). */
    iwdgRegs->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
    /* 3) Write the prescaler by programming
     * the IWDG prescaler register (IWDG_PR) from 0 to 7. */
    iwdgRegs->PR = IWDG_PRESCALER_16;
    /* 4) Write the IWDG reload register (IWDG_RLR). */
    iwdgRegs->RLR = (32000 * WATCHDOG_TIMEOUT_MS) / (16 * 1000);
    /* 5) Wait for the registers to be updated (IWDG_SR = 0x0000 0000). */
    //TODO : switch to sysTick timer
    uint32_t tickstart = HAL_GetTick();
    while (iwdgRegs->SR != 0x00u)
    {
        if ((HAL_GetTick() - tickstart) > IWDG_SR_TIMEOUT)
        {
            isIwdgConfigured = false;
            PRINTF("Can't initialize watchdog.\r\n");
            return;
        }
    }
    /* 6) Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA). */
    //iwdgRegs->WINR = (32000 * WATCHDOG_TIMEOUT_MS) / (16 * 1000);
    iwdgRegs->KR = IWDG_KEY_RELOAD;
    isIwdgConfigured = true;
}

void Core::KickDog()
{
    if(isIwdgConfigured)
        iwdgRegs->KR = IWDG_KEY_RELOAD;
}

#endif
/* EOF */

