#include "core.hpp"

uint8_t getRegionSize(uint32_t size)
{
	uint8_t regionSize = 0;
	/* Count leading zeroes */
	uint8_t leadZeroes = __builtin_clz(size);
	regionSize = 32 - leadZeroes; //get power of 2

	if( size == (uint32_t)( 1<<(regionSize-1) ) )
		regionSize--;
	switch(regionSize)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return ARM_MPU_REGION_SIZE_32B;
		case 6:
			return ARM_MPU_REGION_SIZE_64B;
		case 7:
			return ARM_MPU_REGION_SIZE_128B;
		case 8:
			return ARM_MPU_REGION_SIZE_256B;
		case 9:
			return ARM_MPU_REGION_SIZE_512B;
		case 10:
			return ARM_MPU_REGION_SIZE_1KB;
		case 11:
			return ARM_MPU_REGION_SIZE_2KB;
		case 12:
			return ARM_MPU_REGION_SIZE_4KB;
		case 13:
			return ARM_MPU_REGION_SIZE_8KB;
		case 14:
			return ARM_MPU_REGION_SIZE_16KB;
		case 15:
			return ARM_MPU_REGION_SIZE_32KB;
		case 16:
			return ARM_MPU_REGION_SIZE_64KB;
		default : /* I'm tired */
			return regionSize;
	}
}
