#pragma once

#define PROTECTED_STACK			false

#if (PROTECTED_STACK)
# define THREAD_STACK_SIZE			256
# define THREAD_AMNT				2
# define PROTECTION_ZONE_WORDS		32
# if (PROTECTION_ZONE_WORDS < 32)
#  error "Protection zone too low"
# endif
#endif
