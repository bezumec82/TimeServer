#include "core.hpp"

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * For future used - currently tied to asm SVC_Handler
 * but unused.
 * @param svc_num - number of SVC call
 * number 0 is already taken by privilege change routine
 */
__attribute__(( section (".reset_handler") ))
void zeroOutSection(uint32_t * dst_start, uint32_t * dst_end)
{
    for( ;dst_start < dst_end; dst_start++)
    {
        *dst_start = 0x00;
    }
}

__attribute__(( section (".reset_handler") ))
void copySection(uint32_t * src_start,
        uint32_t * dst_start, uint32_t * dst_end)
{
    for( ;dst_start < dst_end; dst_start++, src_start++ )
    {
        *dst_start = *src_start;
    }
}


#ifdef __cplusplus
}
#endif
