#include "core.hpp"

extern uint8_t getRegionSize(uint32_t size);

using namespace LaOS;

#if(PROTECTED_STACK)
/*!
 * Zones will be created :
 * - 1-st full memory map       - read only for unprivileged
 * - 2-nd stack of threads      - full access, no execution
 * - 3-rd unprotected heap      - full access, no execution
 * - 4-th peripheral            - full access, no execution
 * - 5-th data in DTCM          - full access, no execution
 */
void Core::ConfigMPU()
{
    /*--- Full address space - unprivileged read-only ---*/
    uint32_t RNR = 0;
    __DMB();
    ARM_MPU_Disable();
    MPU->RNR = RNR++;
    MPU->RBAR = 0x0;
    MPU->RASR =
        MPU_RASR_ENABLE_Msk                         | /* bit 0 */
        ARM_MPU_REGION_SIZE_4GB << MPU_RASR_SIZE_Pos    | /* bits 5:1 */
                        /* Sub-region disable */
        0                << MPU_RASR_SRD_Pos        | /* bits 15:8 */
        1                << MPU_RASR_B_Pos          | /* bufferable - bit 16 */
        1                << MPU_RASR_C_Pos          | /* cacheable - bit17 */
        1                << MPU_RASR_S_Pos          | /* shareable - bit18 */
                        /* Type extension field */
        0                << MPU_RASR_TEX_Pos        | /* bits 21:19 */
                        /* Unprivileged access read-only */
        ARM_MPU_AP_URO    << MPU_RASR_AP_Pos        | /* bits 26:24 */
        /* Enable instruction fetch */
        0                << MPU_RASR_XN_Pos         ; /* bit 28 */

    /*--------------------------*/
    /*--- Unprivileged stack ---*/
    /*--------------------------*/
    /* Internal SRAM : Shareable, write through.
     * C=1, B=0, TEX=0, S=1 */
    uint32_t stackStart = 0;
    uint32_t stackEnd = 0;
    __asm__ volatile ("ldr %[addr], =_upStackStart"
            :[addr]"=r"(stackStart)
            :
            :"r0");
    __asm__ volatile ("ldr %[addr], =_upStackEnd"
        :[addr]"=r"(stackEnd)
        :
        :"r0");
    PRINTF("MPU : _upStackStart = 0x%lx, _upStackEnd = 0x%lx\r\n",
            stackStart, stackEnd);
    MPU->RNR = RNR++;
    MPU->RBAR = stackStart;
    MPU->RASR =
        MPU_RASR_ENABLE_Msk                         | /* bit 0 */
        getRegionSize(stackEnd - stackStart) << MPU_RASR_SIZE_Pos   | /* bits 5:1 */
        0                 << MPU_RASR_SRD_Pos       | /* bits 15:8 */
        0                 << MPU_RASR_B_Pos         | /* bufferable - bit 16 */
        1                 << MPU_RASR_C_Pos         | /* cacheable - bit17 */
        1                 << MPU_RASR_S_Pos         | /* shareable - it18 */
        0                 << MPU_RASR_TEX_Pos       | /* bits 21:19 */
                        /* Full access */
        ARM_MPU_AP_FULL << MPU_RASR_AP_Pos          | /* bits 26:24 */
        /* Disable instruction fetch */
        1                << MPU_RASR_XN_Pos         ; /* bit 28 */

    /*-------------------------*/
    /*--- Unprivileged heap ---*/
    /*-------------------------*/
    /* The same as stack */
    uint32_t heapStart = 0;
    __asm__ volatile ("ldr %[hStart], =_upHeapStart"
    :[hStart]"=r"(heapStart)
    :
    :"r0");
    PRINTF("MPU : _upHeapStart = 0x%lx\r\n", heapStart);
    MPU->RNR = RNR++;
    MPU->RBAR = heapStart;
    MPU->RASR =
        MPU_RASR_ENABLE_Msk                         | /* bit 0 */
        getRegionSize(UNPROTECTED_HEAP_SIZE) << MPU_RASR_SIZE_Pos   | /* bits 5:1 */
        0                 << MPU_RASR_SRD_Pos       | /* bits 15:8 */
        0                 << MPU_RASR_B_Pos         | /* bufferable - bit 16 */
        1                 << MPU_RASR_C_Pos         | /* cacheable - bit17 */
        1                 << MPU_RASR_S_Pos         | /* shareable - it18 */
        0                 << MPU_RASR_TEX_Pos       | /* bits 21:19 */
                        /* Full access */
        ARM_MPU_AP_FULL << MPU_RASR_AP_Pos          | /* bits 26:24 */
        /* Disable instruction fetch */
        1                << MPU_RASR_XN_Pos         ; /* bit 28 */


    /*------------------*/
    /*--- Peripheral ---*/
    /*------------------*/
    /* Device : Shareable devices.
     * C=0, B=1, TEX=1, S=1 */
    MPU->RNR = RNR++;
    MPU->RBAR = PERIPH_BASE; //STM32 HAL
    MPU->RASR =
        MPU_RASR_ENABLE_Msk                         | /* bit 0 */
        ARM_MPU_REGION_SIZE_512MB << MPU_RASR_SIZE_Pos    | /* bits 5:1 */
        0                 << MPU_RASR_SRD_Pos       | /* bits 15:8 */
        1                 << MPU_RASR_B_Pos         | /* bufferable - bit 16 */
        0                 << MPU_RASR_C_Pos         | /* cacheable - bit17 */
        1                 << MPU_RASR_S_Pos         | /* shareable - it18 */
        0                 << MPU_RASR_TEX_Pos       | /* bits 21:19 */
                        /* Full access */
        ARM_MPU_AP_FULL << MPU_RASR_AP_Pos          | /* bits 26:24 */
        /* Disable instruction fetch */
        1                << MPU_RASR_XN_Pos         ; /* bit 28 - execute never */

    /*-----------------------------*/
    /*--- Unprivileged data/bss ---*/
    /*-----------------------------*/
    /* Tightly coupled memory.
     * I assume cacheable, not shareable,
     * bufferable - even it is on dedicated CPU bus,
     * I didn't find any mention about buffer.
     * data start -> bss end : one whole region */
    uint32_t upDataStart = 0;
    uint32_t upBssEnd = 0;
    __asm__ volatile ("ldr %[addr], =_upDataStart"
            :[addr]"=r"(upDataStart)
            :
            :"r0");
    __asm__ volatile ("ldr %[addr], =_upBssEnd"
            :[addr]"=r"(upBssEnd)
            :
            :"r0");
    PRINTF("MPU : _upDataStart = 0x%lx, _upBssEnd = 0x%lx\r\n",
            upDataStart, upBssEnd);
    MPU->RNR = RNR++;
    MPU->RBAR = upDataStart;
    MPU->RASR =
        MPU_RASR_ENABLE_Msk                         | /* bit 0 */
        getRegionSize(upBssEnd - upDataStart) << MPU_RASR_SIZE_Pos | /* bits 5:1 */
        0                 << MPU_RASR_SRD_Pos       | /* bits 15:8 */
        1                 << MPU_RASR_B_Pos         | /* bufferable - bit 16 */
        1                 << MPU_RASR_C_Pos         | /* cacheable - bit17 */
        0                 << MPU_RASR_S_Pos         | /* shareable - it18 */
        0                 << MPU_RASR_TEX_Pos       | /* bits 21:19 */
                        /* Full access */
        ARM_MPU_AP_FULL << MPU_RASR_AP_Pos          | /* bits 26:24 */
        /* Disable instruction fetch */
        1                << MPU_RASR_XN_Pos         ; /* bit 28 */

    /* Start MPU */
    ARM_MPU_Enable(0);
    __DSB();
    __ISB();
}
#endif //PROTECTED_STACK
/* EOF */

