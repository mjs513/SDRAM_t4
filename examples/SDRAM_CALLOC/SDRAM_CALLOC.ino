#include "SDRAM_t4.h"
#include "sdios.h"

//constructor for 
SDRAM_t4 sdram;
// Serial output stream
ArduinoOutStream cout(Serial);

/********************************************************************
 * Example of using calloc with sdram taken from CPPREFERENCE Sit:
 * https://en.cppreference.com/w/cpp/memory/c/calloc
 *
 * Quoting notes on calloc
 * Allocates memory for an array of num objects of size size, initializes it 
 * to all bits zero (implicitly creating objects in the destination area).
 *
 * On success, returns the pointer to the beginning of newly allocated memory.
 * To avoid a memory leak, the returned pointer must be deallocated with 
 * std::free() or std::realloc().
 * On failure, returns a null pointer.
 * 
 * Expected Output:
 * p2[0] == 0
 * p2[1] == 0
 * p2[2] == 0
 * p2[3] == 0
 *****************************************************************************/

void setup() {
    while(!Serial && millis() < 5000);
    Serial.begin(115200);

    /**********************************************************
     * sdram.begin initializes the available SDRAM Module
     * begin defaults to 32mb but you can specify the size
     * from begin
     * begin(uint16 external_memory_size) where size is in MB
     * 
     * In addition the SDRAM clock speed can be set using
     * begin(32, 166);
     * and if the cap has been removed on EMC_39:
     * begin(32, 166, 0);
     * See library readme for more info.
     *********************************************************/
    if(!sdram.begin()) {
      Serial.println("SDRAM FAILED TO INITIALIZE.....");
      while(1);
    }


    int* p1 = (int*)sdram_calloc(4, sizeof(int)); // allocate and zero out an array of 4 int
    int* p2 = (int*)sdram_calloc(1, sizeof(int[4])); // same, naming the array type directly
    int* p3 = (int*)sdram_calloc(4, sizeof *p3); // same, without repeating the type name
 
    if (p2)
        for (int n = 0; n < 4; ++n) // print the array
            cout << "p2[" << n << "] == " << p2[n] << '\n';
 
    sdram_free(p1);
    sdram_free(p2);
    sdram_free(p3);

}

void loop() {
  // put your main code here, to run repeatedly:

}
