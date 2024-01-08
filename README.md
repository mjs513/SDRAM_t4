# SDRAM_t4

The SDRAM library supports those Teensy development boards that may be created using SDRAM such as that developed by @Dogbone6.  See this tread on the Teensy forum for more info: https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/

The library uses smalloc by Andrew Rys ( https://github.com/strlcat/smalloc)  which has already been incorporated into the Teensy 4 core for PSRAM support.  However, for SDRAM we use it as a static allocator where methods are used at runtime and not at the global level.  It assumes that thw SDRAM address starts at 0x90000000.

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



