#include "core.hpp"

extern uint32_t stackPool[];
extern uint8_t getRegionSize(uint32_t size);

using namespace LaOS;

/*!
 * Two zones will be created :
 * - 1-st read only for unprivileged - full memory map
 * - 2-nd stack of threads
 */
void Core::ConfigMPU()
{
	uint32_t RNR = 0;
	__DMB();
	ARM_MPU_Disable();
	MPU->RNR = RNR++;
	MPU->RBAR = 0x0;
	MPU->RASR =
		MPU_RASR_ENABLE_Msk	<< MPU_RASR_ENABLE_Pos 	| /* bit 0 */
		ARM_MPU_REGION_SIZE_4GB << MPU_RASR_SIZE_Pos 	| /* bits 5:1 */
						/* Sub-region disable */
		0				<< MPU_RASR_SRD_Pos 	| /* bits 15:8 */
		1				<< MPU_RASR_B_Pos	 	| /* bufferable - bit 16 */
		1				<< MPU_RASR_C_Pos 		| /* cacheable - bit17 */
		1				<< MPU_RASR_S_Pos 		| /* shareable - bit18 */
						/* Type extension field */
		0				<< MPU_RASR_TEX_Pos 	| /* bits 21:19 */
						/* Unprivileged access read-only */
		ARM_MPU_AP_URO	<< MPU_RASR_AP_Pos		| /* bits 26:24 */
		0				<< MPU_RASR_XN_Pos		; /* bit 28*/

	/* Configure MPU for unprivileged stack */
	uint32_t addrStart = 0;
	uint32_t addrEnd = 0;
	__asm__ volatile ("ldr %[addr], =_sustack"
			:[addr]"=r"(addrStart)
			:
			:"r0");
	__asm__ volatile ("ldr %[addr], =_eustack"
		:[addr]"=r"(addrEnd)
		:
		:"r0");

	MPU->RNR = RNR++;
	MPU->RBAR = (uint32_t)&stackPool[0];
	MPU->RASR =
		MPU_RASR_ENABLE_Msk << MPU_RASR_ENABLE_Pos 	| /* bit 0 */
		getRegionSize(addrEnd - addrStart) << MPU_RASR_SIZE_Pos 	| /* bits 5:1 */
		0 				<< MPU_RASR_SRD_Pos 	| /* bits 15:8 */
		1 				<< MPU_RASR_B_Pos	 	| /* bufferable - bit 16 */
		1 				<< MPU_RASR_C_Pos 		| /* cacheable - bit17 */
		1 				<< MPU_RASR_S_Pos 		| /* shareable - it18 */
		0 				<< MPU_RASR_TEX_Pos 	| /* bits 21:19 */
						/* Full access */
		ARM_MPU_AP_FULL << MPU_RASR_AP_Pos		| /* bits 26:24 */
		/* Disable instruction fetch */
		0				<< MPU_RASR_XN_Pos		; /* bit 28*/

	/* Configure MPU for peripheral */
	MPU->RNR = RNR++;
	MPU->RBAR = PERIPH_BASE; //STM32 HAL
	MPU->RASR =
		MPU_RASR_ENABLE_Msk << MPU_RASR_ENABLE_Pos 	| /* bit 0 */
		ARM_MPU_REGION_SIZE_512MB << MPU_RASR_SIZE_Pos 	| /* bits 5:1 */
		0 				<< MPU_RASR_SRD_Pos 	| /* bits 15:8 */
		1 				<< MPU_RASR_B_Pos	 	| /* bufferable - bit 16 */
		1 				<< MPU_RASR_C_Pos 		| /* cacheable - bit17 */
		1 				<< MPU_RASR_S_Pos 		| /* shareable - it18 */
		0 				<< MPU_RASR_TEX_Pos 	| /* bits 21:19 */
						/* Full access */
		ARM_MPU_AP_FULL << MPU_RASR_AP_Pos		| /* bits 26:24 */
		/* Disable instruction fetch */
		0				<< MPU_RASR_XN_Pos		; /* bit 28*/

	/* Start MPU */
	ARM_MPU_Enable(0);
	__DSB();
	__ISB();

}
