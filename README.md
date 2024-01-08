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
NOTE:  The begin method defaults to 32MB for larger chips you can specify the size in begin, i.e., ```begin(128)``` for instance for a 128MB chip.

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

