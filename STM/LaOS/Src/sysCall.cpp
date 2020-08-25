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
__weak void SVC_Handler_C(uint32_t svc_num)
{
	printf("SVC exception number : %lu", svc_num);
}

#ifdef __cplusplus
}
#endif
