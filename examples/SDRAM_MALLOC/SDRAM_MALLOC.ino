#include <SDRAM_t4.h>

//constructor for 
SDRAM_t4 sdram;

/******************************************************************************************
 * Example of using Malloc for SDRAM taken from geeksforgeeks
 * https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/?ref=lbp
 *
 * "... used to dynamically allocate a single large block of memory with the specified size. 
 * It returns a pointer of type void which can be cast into a pointer of any form. It doesn’t 
 * Initialize memory at execution time so that it has initialized each block with the default 
 * garbage value initially."
 *
 *********************************************************************************************/
 
void setup()
{
    while(!Serial && millis() < 5000){}
    Serial.begin(115200);

    if(CrashReport) {
      Serial.print(CrashReport);
      while(1);
    }
 
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
      Serial.printf("SDRAM Init Failed!!!\n");
      while(1);
    };

    // This pointer will hold the
    // base address of the block created
    int* ptr;
    int n, i;
 
    // Get the number of elements for the array
    n = 24;
    printf("Entered number of elements: %d\n", n);
 
    // Dynamically allocate memory using sdram_malloc
    // format: (cast-type*) sdram_malloc(byte-size)
    ptr = (int*)sdram_malloc(n * sizeof(int));
 
    // Check if the memory has been successfully
    // allocated by malloc or not
    if (ptr == NULL) {
        Serial.printf("Memory not allocated.\n");
        exit(0);
    }
    else {
 
        // Memory has been successfully allocated
        printf("Memory successfully allocated using malloc.\n");
 
        // Get the elements of the array
        for (i = 0; i < n; ++i) {
            ptr[i] = i + 1;
        }
 
        // Print the elements of the array
        Serial.printf("The elements of the array are: ");
        for (i = 0; i < n; ++i) {
            Serial.printf("%d, %x \n", ptr[i], &ptr[i]);
        }
    }

}

void loop(){}