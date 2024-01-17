#ifndef _SDRAM_T4_h
#define _SDRAM_T4_h


#include "Arduino.h"
#include "smalloc.h"

extern "C" {
void *sdram_malloc(size_t size);
void sdram_free(void *ptr);
void *sdram_calloc(size_t nmemb, size_t size);
void *sdram_realloc(void *ptr, size_t size);
}

class SDRAM_t4 {
public:
    constexpr SDRAM_t4() {  };
    static bool begin(uint8_t external_sdram_size = 32, uint8_t clock = 166, uint8_t NOCAP = 1);
    
private:
    static unsigned int ns_to_clocks(float ns, float freq);
    static void configure_sdram_pins();
    static bool SendIPCommand(uint32_t address, uint16_t command, uint32_t write, uint32_t *read);
    static bool IPCommandComplete();
    static bool check_fixed_pattern(uint32_t pattern);
       
};
#endif