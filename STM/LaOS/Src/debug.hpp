#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include "LaOSconfig.h"

#ifdef LINUX_TERMINAL
    #define NORM     "\x1B[0m"  //normal
    #define RED      "\x1B[31m" //red
    #define GRN      "\x1B[32m" //green
    #define YEL      "\x1B[33m" //yellow
    #define BLU      "\x1B[34m" //blue
    #define MAG      "\x1B[35m" //magenta
    #define CYN      "\x1B[36m" //cyan
    #define WHT      "\x1B[37m" //white
    #define BOLD     "\e[1m"    //bold symbols
    #define ITAL     "\e[3m"    //italic text
    #define BLINK    "\e[5m"    //blink
#else // win, QT creator terminal, Eclipse terminal
    #define NORM     ""
    #define RED      ""
    #define GRN      ""
    #define YEL      ""
    #define BLU      ""
    #define MAG      ""
    #define CYN      ""
    #define WHT      ""
    #define BOLD     ""
    #define ITAL     ""
    #define BLINK    ""
#endif

#ifndef NDEBUG
# define PRINTF(text, ...)   printf(text, ##__VA_ARGS__)
# define __FILE_NAME__                                                      \
                    (strrchr(__FILE__, '/') ?                               \
                                            strrchr(__FILE__, '/') + 1 :    \
                                            __FILE__)
/* Extended version */
# define PRINTF_EXT(color, text, ...)                                   \
        printf(color "%-20.20s %-20.20s #%-5i: " text "" NORM,          \
               __FILE_NAME__, __FUNCTION__, __LINE__ ,                  \
               ##__VA_ARGS__);

# define PRINTF_COL(color, text, ...)                                   \
        printf(color "" text "" NORM, ##__VA_ARGS__);

# define PRINT_ERR(text, ...)                                           \
        printf(RED "%-20.20s %-20.20s #%-5i: ERROR: " text "" NORM,     \
               __FILE_NAME__, __FUNCTION__, __LINE__,                   \
               ##__VA_ARGS__);

# define PRINT_WARN(text, ...)                                          \
        printf(YEL "%-20.20s %-20.20s #%-5i: WARNING: " text "" NORM,   \
               __FILE_NAME__, __FUNCTION__, __LINE__,                   \
               ##__VA_ARGS__);

# define PRINT_COND(condition, color, text, ... )                       \
        if (condition) { printf(color "" text "" NORM, ##__VA_ARGS__); }
#else
# define PRINTF(color, text, ...)
# define PRINTF_EXT(color, text, ...)
#endif

/*--- Function prototypes ---*/
void debug(const char * msg);

#ifdef __cplusplus
}
#endif
