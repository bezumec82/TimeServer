#pragma once

#define PROTECTED_STACK                     true

#if (PROTECTED_STACK)

//unsuitable with debug
# define USE_WATCHDOG                       false
# define WATCHDOG_TIMEOUT_MS                100

/* To put system in sleep - event driven system.
 * Incompatible with stepped debug. */
# define EVENT_DRIVEN_SYSTEM                false
# define USE_SYSTICK                        true
# define SYSTICK_PERIOD_MS                  10

# define STATIC_STACK_PROTECTION            true
# define THREAD_STACK_SIZE_WORDS            256
/* User should provide information about amount of threads
 * he is willing to create. It it will be declared less threads,
 * then attempt to create extra thread will fail. */
# define THREAD_AMNT                        16 //overshoot
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

#endif //PROTECTED_STACK

//Makes colored output
#define LINUX_TERMINAL
//Un-comment to disable core debug
//#define NDEBUG
