# SDRAM_t4

The SDRAM library supports those Teensy development boards that may be created using SDRAM such as that developed by @Dogbone6.  See this tread on the Teensy forum for more info: https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/

The library uses smalloc by Andrew Rys ( https://github.com/strlcat/smalloc)  which has already been incorporated into the Teensy 4 core for PSRAM support.  However, for SDRAM and allocates memory at runtime and not at the global level.  It assumes that thw SDRAM address starts at 0x90000000.

To use the library is rather simple.

First include the Library in your sketch and declare a instance of SDRAM such as:
```
#include "SDRAM_t4.h"
SDRAM_t4 sdram;
```
then in setup use the begin method to initialize SDRAM
```
    if(!sdram.begin()) {
      Serial.println("SDRAM FAILED TO INITIALIZE.....");
      while(1);
    }
```
The ```begin ``` can take 3 options:
```
begin(uint8_t external_sdram_size = 32, uint8_t clock = 166, uint8_t NOCAP = 1)
```
The first is the SDRAM size:
The begin method defaults to 32MB for larger chips you can specify the size in begin, i.e., ```begin(128)``` for instance for a 128MB chip. However the library is only configured to use a ISSI IS42S16160J-6BLI 32Meg x 8, 16Meg x16 256Mb SYNCHRONOUS DRAM (datasheet: https://www.mouser.com/datasheet/2/198/42_45S83200J_16160J-462626.pdf).  To use any other chips sdram_t4.cpp chip config and timmings will have to change.

The second option is the SDRAM clock speed,  as you can see it defaults to 166Mhz. The rated maximum clock speed.  In addition the following clock speeds can be set:
```
133Mhz: begin(32, 133);
166Mhz: begin(32, 166);
198Mhz: begin(32, 198); //Overclock - use at your own risk
221Mhz: begin(32, 221); //Overclock - use at your own risk, requires a capcitor at C29 on the Dogbone6 dev board 
```

The third option is unique to the Dogbone6 Development Board.  On the development board there is a 100nf capacitor between EMC_39 (DQS) and ground.  NXP recommends that this pin should be left floating but for overclocking you will need a capacitor ( actually size to be determined).  See https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/post-335532 and https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/post-335536 for begining of discussion the this capacitor.

This third option ```useDQS``` allows for correctly setting DQS to internal timing if a capacitor is present or not.  It is a basically a true or false (1 or 0) if a capacitor is present on this pin.  So for example if we running at 166Mhz and there is no capacitor on EMC_39
```
begin(32, 166, 0);
```

METHODS SUPPORT:
```
sdram_malloc - allocates a single large block of memory with the specified size
                format: ptr = (cast-type*) malloc(byte-size)
sdram_calloc  - dynamically allocate the specified number of blocks of memory of the specified type
                format: ptr = (cast-type*)calloc(n, element-size);
sdram_realloc - dynamically change the memory allocation of a previously allocated memory
                format: ptr = realloc(ptr, newSize);
sdram_free -   dynamically de-allocate the memory
                format: sdram_free(ptr)
```

Simple examples are provided. But for more info on these methods check out: https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/

Currently two additional core files have been changed imxrt.h and startup.c.  These are located in the coreFiles directory of the repository.  When Teensyduino version < 1.59 is used you will need to copy these files to the teensy 4 cores directory on the computer.

