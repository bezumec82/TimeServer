#pragma once

#define USE_THREE_COLOR     false

#define eINK_WIDTH_PIX      122
#define eINK_HEIGHT_PIX     250

#define GET_WIDTH_BYTES     ( ( eINK_WIDTH_PIX % 8 == 0 )?    \
                              ( eINK_WIDTH_PIX/8 ) :          \
                              ( eINK_WIDTH_PIX/8 + 1 ) )
