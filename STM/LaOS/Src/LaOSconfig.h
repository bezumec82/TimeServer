#pragma once

#define PROTECTED_STACK			true

#if (PROTECTED_STACK)
# define THREAD_STACK_SIZE			256
# define THREAD_AMNT				3
# define PROTECTION_ZONE_WORDS		64
# if (PROTECTION_ZONE_WORDS < 32)
#  error "Protection zone too low"
# endif

//Check 16 bytes alignment for stack
# if( THREAD_STACK_SIZE % 16 )
#  error "Stack alignment error"
# endif

# if( PROTECTION_ZONE_WORDS % 16 )
#  error "Protection zone alignment error"
# endif

#endif
