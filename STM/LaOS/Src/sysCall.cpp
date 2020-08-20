#include "core.hpp"

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * For future used - currently tied to asm SVC_Handler
 * but unused.
 * @param svc_num
 */
__weak void SVC_Handler_C(uint32_t svc_num)
{
	printf("SVC exception number : %lu", svc_num);
}

#ifdef __cplusplus
}
#endif
