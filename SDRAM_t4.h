#ifndef _SDRAM_T4_h
#define _SDRAM_T4_h

// Configure MPU to allow read/write, disallow exec, use cache
//put following lines in startup.c
//    SCB_MPU_RBAR = 0x80000000 | REGION(i++); // SDRAM
//    SCB_MPU_RASR = MEM_CACHE_WBWA | READWRITE | NOEXEC | SIZE_32M;

#include "Arduino.h"

class SDRAM_t4 {
public:
    constexpr SDRAM_t4() {};
    static bool init();
    
private:
    static unsigned int ns_to_clocks(float ns, float freq);
    static void configure_sdram_pins();
    static bool SendIPCommand(uint32_t address, uint16_t command, uint32_t write, uint32_t *read);
    static bool IPCommandComplete();
    
    //set NOCAP to 1 if cap C29 is removed
    static const uint8_t NOCAP = 0;
    
};
#endif