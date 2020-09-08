#include "eINK.hpp"

using namespace eINK;

/*! Sends buffer to the display */
void Gui::drawAll(void)
{
    display.display(blackBuf.data(), redBuf.data());
}

void Gui::drawBlack(void)
{
    display.displayBlack(blackBuf.data());
}

void Gui::drawRed(void)
{
    display.displayRed(redBuf.data());
}

/*! Fills whole display in one color */
void Gui::fill(Color color)
{
    switch(color)
    {
        case Color::WHITE :
        {
            redBuf.fill(0xff);
            blackBuf.fill(0xff);
            break;
        }
        case Color::BLACK :
        {
            redBuf.fill(0xff);
            blackBuf.fill(0x00);
            break;
        }
        case Color::RED :
        {
            redBuf.fill(0x00);
            blackBuf.fill(0xff);
            break;
        }
    }
}
