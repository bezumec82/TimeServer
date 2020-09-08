/* newlib facilities */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* LaOS facilities */
#include "../Src/eINK.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/* Your application goes here */
int _main( void )
{
    /* Your code here */
    ::eINK::IL0373 il;
    ::eINK::Gui gui(il);
    gui.fill(::eINK::Color::BLACK);
    gui.drawAll();
    return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
