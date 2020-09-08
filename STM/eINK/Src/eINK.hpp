#pragma once

#include <type_traits>
#include <utility>
#include <array>

#include "../port/config.hpp"

namespace eINK
{
    /* This class provides IL0373 for IL0373 controller */
    class IL0373
    {
    public :
        IL0373();
        ~IL0373() = default;

    private : /*--- Implementation ---*/
        void initGpio(void);

        void init(void);
        void wait(void);
        void refresh();

        void setLutsFull(void);
        void setLutsPartial(void);
        void sendCmd(uint8_t );
        void sendData(uint8_t );

        /*--- port ---*/
        template<typename Uint = uint32_t,
            typename = ::std::enable_if_t< ::std::is_integral<Uint>::value > >
        void yield(Uint&& ms = 100);

        template < typename Ptr,
            typename = ::std::enable_if_t< ::std::is_pointer<Ptr>::value > >
        void writeByte(Ptr&& ptr);

    public : /*--- Methods ---*/
        void reset(void);
        void clear(void);
        void display(const uint8_t * , const uint8_t * );
        void displayBlack(const uint8_t * );
        void displayRed(const uint8_t * );

    private : /*--- Properties ---*/

    };

    enum class Color : uint8_t
    {
        WHITE = 0,
        RED = 1,
        BLACK = 2
    };

    class Gui
    {
    public :
        Gui(IL0373& display)
        : display(display)
        { }
        ~Gui() = default;

    public : /* Methods */
        void drawAll(void);
        void drawBlack(void);
        void drawRed(void);
        void fill(Color );
    private :

    private :
        IL0373& display;
        ::std::array<uint8_t, (eINK_HEIGHT_PIX * GET_WIDTH_BYTES)> blackBuf;
        ::std::array<uint8_t, (eINK_HEIGHT_PIX * GET_WIDTH_BYTES)> redBuf;
    };
}


#include "../port/port.hpp"

