#include <stdint.h>

#ifdef STM32H757xx
# include "stm32h7xx_hal.h"
#endif

#ifdef STM32F746xx
# include "stm32f7xx_hal.h"
#endif

#include "cmsis_gcc.h"

class Atomic
{
public :
    Atomic(uint32_t init_val = 0)
    :atomic(init_val)
    {}

    uint32_t load()
    {
        int status;
        int retVal;
        __DMB();
        do{
            __CLREX(); //Force the local exclusive access monitor to clear
            retVal = __LDREXW(        (volatile uint32_t*)&atomic); //set monitor
            status = __STREXW(retVal, (volatile uint32_t*)&atomic);
        }while( status != 0 );
        return retVal;
    }

    void store(uint32_t new_val)
    {
        int status;
        do{
            __CLREX(); //Force the local exclusive access monitor to clear
            (void)   __LDREXW(         (volatile uint32_t*)&atomic); //set monitor
            status = __STREXW(new_val, (volatile uint32_t*)&atomic);
        }while( status != 0 );
        __DMB();
    }
private :
    uint32_t atomic;
};

class Semaphore
{
public :
    Semaphore(uint32_t count = 0)
    :exMemory(count)
    {}
    void take()
    {
        uint32_t newLock = 0;
        uint32_t status;
        assert( (__get_IPSR() == 0)&&
                (__get_PRIMASK() != 0)&&
                    "Can't take semaphore from an exception.\r\n"
                    "Sanity test failed.\r\n");
        do
        {
            do
            {
                newLock =__LDREXW( (volatile uint32_t*)&exMemory );
                if(!newLock) Yield();
            }while(newLock == 0);

            newLock = newLock - 1;
            status = __STREXW(newLock, (volatile uint32_t*)&exMemory);
        }while( status != 0 );
        __DMB();
    }

    void give()
    {
        uint32_t status;
        uint32_t newLock;
        __DMB();
        //if ((__get_IPSR() != 0)||(__get_PRIMASK() != 0)) //interrupt context
            __CLREX();  //Force the local exclusive access monitor to clear
                        //so that next exclusive store must fail
        do{ //Blocking attempt to give lock
            newLock = __LDREXW( (volatile uint32_t*)&exMemory );
            newLock = newLock + 1;
            status = __STREXW(newLock, (volatile uint32_t*)&exMemory);
        }while( status != 0 );
    }

private :
    uint32_t exMemory;

};
