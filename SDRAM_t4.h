#ifndef _SDRAM_T4_h
#define _SDRAM_T4_h

// Configure MPU to allow read/write, disallow exec, use cache
//put following lines in startup.c
//	SCB_MPU_RBAR = 0x90000000 | REGION(i++); // SEMC: SDRAM, NAND, SRAM, etc
//	SCB_MPU_RASR = MEM_CACHE_WBWA | READWRITE | NOEXEC | SIZE_1G;
	// SDRAM PCB: https://forum.pjrc.com/index.php?threads/73898/#post-334041
//	SCB_MPU_RBAR = 0x91E00000 | REGION(i++); // SEMC: SDRAM, NAND, SRAM, etc
//	SCB_MPU_RASR = MEM_NOCACHE | READWRITE | NOEXEC | SIZE_2M;


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
    static bool begin(uint8_t external_sdram_size = 32);
    
private:
    static unsigned int ns_to_clocks(float ns, float freq);
    static void configure_sdram_pins();
    static bool SendIPCommand(uint32_t address, uint16_t command, uint32_t write, uint32_t *read);
    static bool IPCommandComplete();
       
};
#endif