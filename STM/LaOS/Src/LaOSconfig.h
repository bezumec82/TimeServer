#pragma once

#define PROTECTED_STACK                     true

#if (PROTECTED_STACK)
# define USE_WATCHDOG                       true

# define WATCHDOG_TIMEOUT_MS                100
# define STATIC_STACK_PROTECTION            true
# define THREAD_STACK_SIZE_WORDS            256
/* User should provide information about amount of threads
 * he is willing to create. It it will be declared less threads,
 * then attempt to create extra thread will fail. */
# define THREAD_AMNT                        5
# define PROTECTION_ZONE_WORDS              64
# if (PROTECTION_ZONE_WORDS < 32)
#  error "Protection zone too low"
# endif

//Check 16 bytes alignment for stack
# if( THREAD_STACK_SIZE_WORDS % 16 )
#  error "Stack alignment error"
# endif

# if( PROTECTION_ZONE_WORDS % 16 )
#  error "Protection zone alignment error"
# endif

#define UNPROTECTED_HEAP_SIZE               8192

#endif
